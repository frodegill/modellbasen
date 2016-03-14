#include "application.h"
#include "ui/widget/main_widget.h"
#include "../storage/poco_glue.h"
#include "../storage/usermanager.h"
#include "../singleton/db.h"
#include "../singleton/lock.h"
#include "../singleton/push.h"


using namespace modellbasen;

WebApplication::WebApplication(const Wt::WEnvironment& environment)
: Wt::WApplication(environment),
  m_main_widget(nullptr)
{
	messageResourceBundle().use(appRoot() + "strings");
	useStyleSheet(Wt::WLink("modellbasen.css"));
	setTitle(Wt::WString::tr("AppName"));
	internalPathChanged().connect(this, &WebApplication::onInternalPathChange);
	enableUpdates(true);
	::connect(this, boost::bind(&WebApplication::serverPush, this));
}

WebApplication::~WebApplication()
{
	delete m_main_widget;

	delete m_usermanager;

	::disconnect(this);
	enableUpdates(false);
}

bool WebApplication::Initialize()
{
	if (!DB.Initialize(Wt::WString::tr(DB_CONNECTION_STRING_KEY).toUTF8()))
		return false;

	m_usermanager = new UserManager(this);
	if (!m_usermanager)
		return false;

	if (!m_main_widget)
	{
		m_main_widget = new MainWidget(this);
	}
	m_main_widget->Initialize();

	return true;
}

void WebApplication::onInternalPathChange(const std::string& url)
{
	if (EQUAL == url.compare(Wt::WString("RegisterProfileInternalLink").toUTF8()))
	{
	}
	else if (EQUAL == url.compare(Wt::WString("ForgotPasswordInternalLink").toUTF8()))
	{
	}
	setInternalPath("/");
}

void WebApplication::OnLoggedIn()
{
	if (m_main_widget)
		m_main_widget->OnLoggedIn();
}

void WebApplication::OnLoggedOut()
{
	if (m_main_widget)
		m_main_widget->OnLoggedIn();
}

void WebApplication::serverPush()
{
	boost::mutex::scoped_lock lock(g_global_mutex);
	Wt::WApplication::instance()->triggerUpdate();
}
