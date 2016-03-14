#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <Wt/WApplication>
#include <Wt/WEnvironment>


namespace modellbasen
{

class MainWidget;
class UserManager;

class WebApplication : public Wt::WApplication
{
public:
	WebApplication(const Wt::WEnvironment& environment);
	~WebApplication();

	bool Initialize();

public:
	void onInternalPathChange(const std::string& url);

	void OnLoggedIn();
	void OnLoggedOut();

public:
	void serverPush();

public:
	UserManager*           GetUserManager() const {return m_usermanager;}

private:
	MainWidget*            m_main_widget;

	UserManager*           m_usermanager;
};

} // namespace modellbasen

#endif // _APPLICATION_H_
