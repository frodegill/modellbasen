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
	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	bool ret = Exists(session, username, exists);
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return ret;
}

bool UserManager::Exists(Poco::Data::Session* session, const std::string& username, bool& exists)
{
	if (!session)
		return false;

	std::string username_lowercase = boost::locale::to_lower(username);

	int exist_count = 0;
	DEBUG_TRY_CATCH(*session << "SELECT COUNT(*) FROM user WHERE username=?",
		Poco::Data::Keywords::into(exist_count),
		Poco::Data::Keywords::use(username_lowercase),
		Poco::Data::Keywords::now;)
	
	exists = 0<exist_count;
	return true;
}

bool UserManager::RegisterUser(Poco::Data::Session* session_in_transaction,
															 const std::string& username, const std::string& password,
                               const std::string& email, const std::string& postcode,
															 Poco::UInt32 type_bflag)
{
	bool username_exists, postcode_exists;
	if (!session_in_transaction ||
	    username.empty() ||
	    !Exists(session_in_transaction, username, username_exists) || username_exists ||
	    !PostCode::Exists(session_in_transaction, postcode, postcode_exists) || !postcode_exists ||
	    email.empty())
		return false;

	std::string hash;
	ComputeHash(username, password, hash);

	DEBUG_TRY_CATCH(*session_in_transaction << "INSERT INTO user (username, bcrypt_password_hash, email) VALUE (?, ?, ?)",
		Poco::Data::Keywords::useRef(username),
		Poco::Data::Keywords::use(hash),
		Poco::Data::Keywords::useRef(email),
		Poco::Data::Keywords::now;)

	IdType user_id;
	if (!GetUserId(session_in_transaction, username, user_id) || INVALID_ID==user_id)
		return false;

	Poco::Nullable<std::string> null_stringvalue;
	Poco::Nullable<Poco::UInt32> null_intvalue;
	Poco::Nullable<TimeType> null_timevalue;

	Poco::Nullable<std::string> postcode_stringvalue(postcode);
	bool ret = Tag::SetUserTag(session_in_transaction, user_id, TAG_POSTCODE, postcode_stringvalue, null_intvalue, null_timevalue);
	if (ret && (0!=(type_bflag&MODEL_BFLAG)))        ret=Tag::SetUserTag(session_in_transaction, user_id, TAG_MODEL, null_stringvalue, null_intvalue, null_timevalue);
	if (ret && (0!=(type_bflag&PHOTOGRAPHER_BFLAG))) ret=Tag::SetUserTag(session_in_transaction, user_id, TAG_PHOTOGRAPHER, null_stringvalue, null_intvalue, null_timevalue);
	if (ret && (0!=(type_bflag&MUA_BFLAG)))          ret=Tag::SetUserTag(session_in_transaction, user_id, TAG_MUA, null_stringvalue, null_intvalue, null_timevalue);
	if (ret && (0!=(type_bflag&HAIRDRESSER_BFLAG)))  ret=Tag::SetUserTag(session_in_transaction, user_id, TAG_HAIRDRESSER, null_stringvalue, null_intvalue, null_timevalue);
	if (ret && (0!=(type_bflag&AGENCY_BFLAG)))       ret=Tag::SetUserTag(session_in_transaction, user_id, TAG_AGENCY, null_stringvalue, null_intvalue, null_timevalue);
	return ret;
}

bool UserManager::GetUserId(const std::string& username, IdType& user_id)
{
	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	bool ret = GetUserId(session, username, user_id);
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return ret;
}

bool UserManager::GetUserId(Poco::Data::Session* session, const std::string& username, IdType& user_id)
{
	if (!session)
		return false;

	std::string username_lowercase = boost::locale::to_lower(username);

	IF_NO_ROWS(stmt, *session,
		"SELECT id FROM user WHERE username=?",
			Poco::Data::Keywords::into(user_id),
			Poco::Data::Keywords::use(username_lowercase))
	{
		user_id = INVALID_ID;
	}
	
	return true;
}

bool UserManager::GetUsername(Poco::Data::Session* session, IdType id, std::string& username)
{
	if (!session || INVALID_ID==id)
		return false;

	//Check in cache
	std::unordered_map<IdType, std::string>::iterator iter = m_username_cache.find(id);
	if (m_username_cache.end() != iter)
	{
		username = m_username_cache[id];
		return true;
	}
	
	//Check in database
	IF_NO_ROWS(stmt, *session,
		"SELECT username FROM user WHERE id=?",
			Poco::Data::Keywords::into(username),
			Poco::Data::Keywords::use(id))
	{
		return false;
	}

	//Found. Add to cache
	m_username_cache[id] = username;
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

	IF_NO_ROWS(stmt, *session,
		"SELECT id, bcrypt_password_hash FROM user WHERE username=?",
			Poco::Data::Keywords::into(user->m_id),
			Poco::Data::Keywords::into(user->m_bcrypt_password_hash),
			Poco::Data::Keywords::use(username_lowercase))
	{
		delete user;
		return false;
	}
	
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

	DEBUG_TRY_CATCH(*session_in_transaction << "UPDATE user SET bcrypt_password_hash=? WHERE id=?",
			Poco::Data::Keywords::use(hash),
			Poco::Data::Keywords::use(m_current_user->m_id),
			Poco::Data::Keywords::now;)

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
	DEBUG_TRY_CATCH(*session_in_transaction << "SELECT COUNT(*) FROM user WHERE username=?",
			Poco::Data::Keywords::into(count),
			Poco::Data::Keywords::use(username_lowercase),
			Poco::Data::Keywords::now;)

	if (0 == count)
		return true;

	std::string hash;
	ComputeHash(username_lowercase, new_password, hash);

	DEBUG_TRY_CATCH(*session_in_transaction << "UPDATE user SET bcrypt_password_hash=? WHERE username=?",
			Poco::Data::Keywords::use(hash),
			Poco::Data::Keywords::use(username_lowercase),
			Poco::Data::Keywords::now;)

	return true;
}

void UserManager::ComputeHash(const std::string& username, const std::string& password, std::string& hash)
{
	Wt::Auth::BCryptHashFunction func(10);
	hash = func.compute(username + password, PASSWORD_SALT);
}
