#include <Wt/WImage>
#include <Wt/WHBoxLayout>

#include "main_widget.h"
#include "login_widget.h"
#include "../tab/administrator_tab.h"
#include "../tab/messageboard_tab.h"
#include "../tab/profile_tab.h"
#include "../tab/register_profile_tab.h"
#include "../tab/search_tab.h"
#include "../../application.h"
#include "../../../storage/usermanager.h"


using namespace modellbasen;

MainWidget::MainWidget(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app),
  m_initialized(false),
  m_tab_widget(nullptr),
  m_profile_tab(nullptr),
  m_register_profile_tab(nullptr),
  m_messageboard_tab(nullptr),
  m_search_tab(nullptr),
  m_administrator_tab(nullptr)
{
}

MainWidget::~MainWidget()
{
}

void MainWidget::Initialize()
{
	if (m_initialized)
		return;

	Wt::WVBoxLayout* main_vbox_layout = new Wt::WVBoxLayout();
	main_vbox_layout->setContentsMargins(0, 0, 0, 0);
	setLayout(main_vbox_layout);

	AddHeader(main_vbox_layout);

	//Tabs
	m_tab_widget = new Wt::WTabWidget();
	CreateProfileTab(m_tab_widget);
	CreateRegisterProfileTab(m_tab_widget);
	CreateMessageBoardTab(m_tab_widget);
	CreateSearchTab(m_tab_widget);
	CreateAdministratorTab(m_tab_widget);
	main_vbox_layout->addWidget(m_tab_widget);

	//Replace root()
	while (0 < m_app->root()->count())
	{
		Wt::WWidget* widget = m_app->root()->widget(0);
		m_app->root()->removeWidget(widget);
	}
	m_app->root()->addWidget(this);

	UpdateTabVisibility();
	m_initialized = true;
}

void MainWidget::AddHeader(Wt::WVBoxLayout* layout)
{
	//Header
	Wt::WContainerWidget* header_widget = new Wt::WContainerWidget();
	Wt::WHBoxLayout* header_hbox = new Wt::WHBoxLayout();
	header_hbox->setContentsMargins(0, 0, 0, 0);
	header_widget->setLayout(header_hbox);

	Wt::WImage* logo = new Wt::WImage("images/logo.png");
	header_hbox->addWidget(logo, 0, Wt::AlignLeft|Wt::AlignTop);

	Wt::WText* appname_text = new Wt::WText(Wt::WString::tr("AppName"));
	appname_text->setStyleClass("appname");
	header_hbox->addWidget(appname_text, 0, Wt::AlignCenter|Wt::AlignMiddle);

	LoginWidget* login_widget = new LoginWidget(m_app);
	if (login_widget)
	{
		login_widget->Initialize();
		header_hbox->addWidget(login_widget, 0, Wt::AlignRight|Wt::AlignTop);
	}

	layout->addWidget(header_widget);
}

void MainWidget::CreateProfileTab(Wt::WTabWidget* tab_widget)
{
	m_profile_tab = new ProfileTab(m_app);
	tab_widget->addTab(m_profile_tab, Wt::WString::tr("Tab.ProfileTab"));
}

void MainWidget::CreateRegisterProfileTab(Wt::WTabWidget* tab_widget)
{
	m_register_profile_tab = new RegisterProfileTab(m_app);
	tab_widget->addTab(m_register_profile_tab, Wt::WString::tr("Tab.RegisterProfileTab"));
}

void MainWidget::CreateMessageBoardTab(Wt::WTabWidget* tab_widget)
{
	m_messageboard_tab = new MessageBoardTab(m_app);
	tab_widget->addTab(m_messageboard_tab, Wt::WString::tr("Tab.MessageBoardTab"));
}

void MainWidget::CreateSearchTab(Wt::WTabWidget* tab_widget)
{
	m_search_tab = new SearchTab(m_app);
	tab_widget->addTab(m_search_tab, Wt::WString::tr("Tab.SearchTab"));
}

void MainWidget::CreateAdministratorTab(Wt::WTabWidget* tab_widget)
{
	m_administrator_tab = new AdministratorTab(m_app);
	tab_widget->addTab(m_administrator_tab, Wt::WString::tr("Tab.AdministratorTab"));
}

void MainWidget::UpdateTabVisibility()
{
	bool is_logged_in = m_app->GetUserManager()->IsLoggedIn();
	const User* current_user = m_app->GetUserManager()->GetCurrentUser();
	bool is_administrator = current_user && current_user->HasTag(TAG_ADMINISTRATOR);
	m_tab_widget->setTabHidden(m_tab_widget->indexOf(m_profile_tab), !is_logged_in);
	m_tab_widget->setTabHidden(m_tab_widget->indexOf(m_register_profile_tab), true);
	m_tab_widget->setTabHidden(m_tab_widget->indexOf(m_administrator_tab), !is_administrator);
}

void MainWidget::ActivateTab(const TabType& tab_type)
{
	Wt::WWidget* widget = nullptr;
	switch(tab_type)
	{
		case PROFILE:          widget = m_profile_tab; break;
		case REGISTER_PROFILE: widget = m_register_profile_tab; break;
		case MESSAGEBOARD:     widget = m_messageboard_tab; break;
		case SEARCH:           widget = m_search_tab; break;
		case ADMINISTRATOR:    widget = m_administrator_tab; break;
	}
	
	int index;
	if (widget && -1!=(index=m_tab_widget->indexOf(widget))) {
		if (m_tab_widget->isTabHidden(index))
		{
			m_tab_widget->setTabHidden(index, false);
		}
		m_tab_widget->setCurrentWidget(widget);
	}
}

void MainWidget::OnLoggedIn()
{
	m_profile_tab->OnLoggedIn();
	m_messageboard_tab->OnLoggedIn();
	m_search_tab->OnLoggedIn();
	m_administrator_tab->OnLoggedIn();
	UpdateTabVisibility();
}

void MainWidget::OnLoggedOut()
{
	UpdateTabVisibility();
	m_profile_tab->OnLoggedOut();
	m_messageboard_tab->OnLoggedOut();
	m_search_tab->OnLoggedOut();
	m_administrator_tab->OnLoggedOut();
}

void MainWidget::OnRegisterNewProfile()
{
}
