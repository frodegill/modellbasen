#ifdef USE_PCH
# include "../pch.h"
#else
#endif

#include "application.h"
#include "global.h"
#include "../storage/poco_glue.h"


using namespace modellbasen;

WebApplication::WebApplication(const Wt::WEnvironment& environment)
: Wt::WApplication(environment),
  m_login_widget(NULL),
  m_main_widget(NULL)
{
	messageResourceBundle().use(appRoot() + "strings");
	useStyleSheet(Wt::WLink("modellbasen.css"));
	setTitle(Wt::WString::tr("AppName"));
	enableUpdates();
}

WebApplication::~WebApplication()
{
	delete m_login_widget;
	delete m_main_widget;

	delete m_usermanager;
}

bool WebApplication::Initialize()
{
	if (!DB.Initialize(Wt::WString::tr(DB_CONNECTION_STRING_KEY).toUTF8()))
		return false;

	m_usermanager = new UserManager();
	if (!m_usermanager)
		return false;

	if (!m_usermanager->GetCurrentUser())
	{
		ActivateLoginWidget();
	}
	return true;
}

void WebApplication::ActivateLoginWidget()
{
	if (!m_login_widget)
	{
		m_login_widget = new LoginWidget(this);
	}

	m_login_widget->ActivateLoginWidget();
}

void WebApplication::ActivateMainWidget()
{
	if (!m_main_widget)
	{
		m_main_widget = new MainWidget(this);
	}

	m_main_widget->ActivateMainWidget();
}
