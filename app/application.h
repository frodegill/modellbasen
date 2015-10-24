#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "ui/widget/login_widget.h"
#include "ui/widget/main_widget.h"
#include "../storage/usermanager.h"

#include <Wt/WApplication>
#include <Wt/WEnvironment>


namespace modellbasen
{

class WebApplication : public Wt::WApplication
{
public:
	WebApplication(const Wt::WEnvironment& environment);
	~WebApplication();

	bool Initialize();

public:
	void ActivateLoginWidget();
	void ActivateMainWidget();

public:
	UserManager*           GetUserManager() const {return m_usermanager;}

private:
	LoginWidget*           m_login_widget;
	MainWidget*            m_main_widget;

	UserManager*           m_usermanager;
};

} // namespace modellbasen

#endif // _APPLICATION_H_
