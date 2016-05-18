#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#include "../app/defines.h"
#include "../singleton/db.h"
#include "dbo/user.h"

#include <unordered_map>
#include <Wt/WStringListModel>


namespace modellbasen
{

#define PASSWORD_SALT "modellbasen"

#define ROLE_ADMIN    "Administrator"


class WebApplication;

class UserManager {
public:
	UserManager(WebApplication* app);
	~UserManager();

public:
	static bool Exists(const std::string& username, bool& exists);
	static bool Exists(Poco::Data::Session* session, const std::string& username, bool& exists);

	static bool RegisterUser(Poco::Data::Session* session_in_transaction,
													 const std::string& username, const std::string& password,
													 const std::string& email, const std::string& postcode,
													 Poco::UInt32 type_bflag);

	static bool GetUserId(const std::string& username, IdType& user_id);
	static bool GetUserId(Poco::Data::Session* session, const std::string& username, IdType& user_id);

	bool GetUsername(Poco::Data::Session* session, IdType id, std::string& username);

	static bool GetMatchingUsernames(Poco::Data::Session* session, const std::string& filter, Wt::WStringListModel& username_model);

public:
	bool LogIn(const std::string& username, const std::string& password);
	const User* GetCurrentUser() const {return m_current_user;}
	bool LogOut();

	bool IsLoggedIn() const {return nullptr!=m_current_user;}
	
	bool ChangePassword(Poco::Data::Session* session_in_transaction, const std::string& old_password, const std::string& new_password);
	bool AdminChangePassword(Poco::Data::Session* session_in_transaction, const std::string& username, const std::string& new_password);

public:
	static void ComputeHash(const std::string& username, const std::string& password, std::string& hash);

private:
	WebApplication* m_app;
	User* m_current_user;

	std::unordered_map<IdType, std::string> m_username_cache;
};

} // namespace modellbasen


#endif // _USERMANAGER_H_
