#include <boost/locale.hpp>
#include "Wt/Auth/HashFunction"

#include "usermanager.h"
#include "dbo/postcode.h"
#include "dbo/tag.h"
#include "../app/application.h"


using namespace modellbasen;

UserManager::UserManager(WebApplication* app)
: m_app(app),
  m_current_user(nullptr)
{
}

UserManager::~UserManager()
{
	delete m_current_user;
}

bool UserManager::Exists(const std::string& username, bool& exists)
{
	std::string username_lowercase = boost::locale::to_lower(username);

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	int exist_count = 0;
	*session << "SELECT COUNT(*) FROM user WHERE username=?",
		Poco::Data::Keywords::into(exist_count),
		Poco::Data::Keywords::use(username_lowercase),
		Poco::Data::Keywords::now;
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);

	exists = 0<exist_count;
	return true;
}

bool UserManager::RegisterUser(Poco::Data::Session* session_in_transaction,
															 const std::string& username, const std::string& password,
                               const std::string& email, const std::string& postcode)
{
	bool username_exists, postcode_exists;
	if (!session_in_transaction ||
		  username.empty() ||
	    !Exists(username, username_exists) || username_exists ||
	    !PostCode::Exists(postcode, postcode_exists) || !postcode_exists ||
	    email.empty())
		return false;

	std::string hash;
	ComputeHash(username, password, hash);

	*session_in_transaction << "INSERT INTO user (username, bcrypt_password_hash, email) VALUE (?, ?, ?)",
		Poco::Data::Keywords::useRef(username), Poco::Data::Keywords::use(hash), Poco::Data::Keywords::useRef(email), Poco::Data::Keywords::now;

	return Tag::SetUserTag(session_in_transaction, username, TAG_POSTCODE, postcode, 0, 0);
}

bool UserManager::GetUserId(const std::string& username, Poco::UInt32& user_id)
{
	std::string username_lowercase = boost::locale::to_lower(username);

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	*session << "SELECT id FROM user WHERE username=?",
		Poco::Data::Keywords::into(user_id, Poco::Data::Position(0), INVALID_ID),
		Poco::Data::Keywords::use(username_lowercase),
		Poco::Data::Keywords::now;
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);

	return true;
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
	
	m_app->OnLoggedIn();
	return true;
}

bool UserManager::LogOut()
{
	delete m_current_user;
	m_current_user = nullptr;
	m_app->OnLoggedOut();
	return true;
}

bool UserManager::ChangePassword(Poco::Data::Session* session_in_transaction,
                                 const std::string& old_password, const std::string& new_password)
{
	if (!session_in_transaction || !m_current_user)
		return false;

	std::string hash;
	ComputeHash(m_current_user->GetUsername(), old_password, hash);
	if (EQUAL != hash.compare(m_current_user->GetPasswordHash()))
		return false;

	ComputeHash(m_current_user->GetUsername(), new_password, hash);

	*session_in_transaction << "UPDATE user SET bcrypt_password_hash=? WHERE id=?",
			Poco::Data::Keywords::use(hash),
			Poco::Data::Keywords::use(m_current_user->m_id),
			Poco::Data::Keywords::now;

	m_current_user->SetPasswordHash(hash);
	return true;
}

bool UserManager::AdminChangePassword(Poco::Data::Session* session_in_transaction,
																			const std::string& username, const std::string& new_password)
{
	if (!session_in_transaction || !m_current_user || !m_current_user->HasTag(TAG_ADMINISTRATOR))
		return false;

	std::string username_lowercase = boost::locale::to_lower(username);

	uint count;
	*session_in_transaction << "SELECT COUNT(*) FROM user WHERE username=?",
			Poco::Data::Keywords::into(count),
			Poco::Data::Keywords::use(username_lowercase),
			Poco::Data::Keywords::now;

	if (0 == count)
		return true;

	std::string hash;
	ComputeHash(username_lowercase, new_password, hash);

	*session_in_transaction << "UPDATE user SET bcrypt_password_hash=? WHERE username=?",
			Poco::Data::Keywords::use(hash),
			Poco::Data::Keywords::use(username_lowercase),
			Poco::Data::Keywords::now;

	return true;
}

void UserManager::ComputeHash(const std::string& username, const std::string& password, std::string& hash)
{
	Wt::Auth::BCryptHashFunction func(10);
	hash = func.compute(username + password, PASSWORD_SALT);
}
