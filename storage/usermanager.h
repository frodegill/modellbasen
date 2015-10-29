#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#include "../app/defines.h"
#include "dbo/user.h"


namespace modellbasen
{

#define PASSWORD_SALT "modellbasen"

#define ROLE_ADMIN    "Administrator"


class UserManager {
public:
	UserManager();
	~UserManager();

public:
	bool LogIn(const std::string& username, const std::string& password);
	const User* GetCurrentUser() const {return m_current_user;}
	bool LogOut();

	bool ChangePassword(const std::string& old_password, const std::string& new_password);
	bool AdminChangePassword(const std::string& username, const std::string& new_password);

public:
	static void ComputeHash(const std::string& username, const std::string& password, std::string& hash);

private:
	User* m_current_user;
};

} // namespace modellbasen


#endif // _USERMANAGER_H_
