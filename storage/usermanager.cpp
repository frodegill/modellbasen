#ifdef USE_PCH
# include "../pch.h"
#else
# include <boost/locale.hpp>
# include "Wt/Auth/HashFunction"
#endif

#include "usermanager.h"
#include "dbo/tag.h"
#include "../app/global.h"


using namespace modellbasen;

UserManager::UserManager()
: m_current_user(NULL)
{
}

UserManager::~UserManager()
{
	delete m_current_user;
}

bool UserManager::LogIn(const std::string& username, const std::string& password)
{
	std::string username_lowercase = boost::locale::to_lower(username);

	User* user = new User();
	user->m_username = username;

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
	{
		delete user;
		return false;
	}

	*session << "SELECT id, bcrypt_password_hash FROM user WHERE username=?",
		Poco::Data::Keywords::into(user->m_id),
		Poco::Data::Keywords::into(user->m_bcrypt_password_hash),
		Poco::Data::Keywords::use(username_lowercase),
		Poco::Data::Keywords::now;
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);

	std::string hash;
	ComputeHash(username, password, hash);

	if (EQUAL != hash.compare(user->m_bcrypt_password_hash))
	{
		delete user;
		return false;
	}

	delete m_current_user;
	m_current_user = user;
	return true;
}

bool UserManager::LogOut()
{
	delete m_current_user;
	m_current_user = NULL;
	return true;
}

bool UserManager::ChangePassword(const std::string& old_password, const std::string& new_password)
{
	if (!m_current_user)
	{
		return false;
	}

	std::string hash;
	ComputeHash(m_current_user->GetUsername(), old_password, hash);
	if (EQUAL != hash.compare(m_current_user->GetPasswordHash()))
	{
		return false;
	}

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
	{
		return false;
	}

	ComputeHash(m_current_user->GetUsername(), new_password, hash);

	*session << "UPDATE user SET bcrypt_password_hash=? WHERE id=?",
			Poco::Data::Keywords::use(hash),
			Poco::Data::Keywords::use(m_current_user->m_id),
			Poco::Data::Keywords::now;

	DB.ReleaseSession(session, PocoGlue::COMMIT);

	m_current_user->SetPasswordHash(hash);
	return true;
}

bool UserManager::AdminChangePassword(const std::string& username, const std::string& new_password)
{
	if (!m_current_user || !m_current_user->HasTag(TAG_ADMINISTRATOR))
	{
		return false;
	}

	std::string username_lowercase = boost::locale::to_lower(username);

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
	{
		return false;
	}

	uint count;
	*session << "SELECT COUNT(*) FROM user WHERE username=?",
			Poco::Data::Keywords::into(count),
			Poco::Data::Keywords::use(username_lowercase),
			Poco::Data::Keywords::now;

	if (0 == count)
	{
		DB.ReleaseSession(session, PocoGlue::IGNORE);
		return false;
	}

	std::string hash;
	ComputeHash(username_lowercase, new_password, hash);

	*session << "UPDATE user SET bcrypt_password_hash=? WHERE username=?",
			Poco::Data::Keywords::use(hash),
			Poco::Data::Keywords::use(username_lowercase),
			Poco::Data::Keywords::now;

	DB.ReleaseSession(session, PocoGlue::COMMIT);

	return true;
}

void UserManager::ComputeHash(const std::string& username, const std::string& password, std::string& hash)
{
	Wt::Auth::BCryptHashFunction func(10);
	hash = func.compute(username + password, PASSWORD_SALT);
}
