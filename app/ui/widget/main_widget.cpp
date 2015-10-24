#include "main_widget.h"

#include <Wt/WImage>
#include <Wt/WHBoxLayout>
#include "../../application.h"
#include "../tab/administrator_tab.h"


using namespace modellbasen;

MainWidget::MainWidget(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app),
  m_initialized(false),
  m_tab_widget(NULL),
  m_administrator_tab(NULL),
  m_profile_tab(NULL)
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
	main_vbox_layout->setContentsMargins(15, 15, 15, 15);

	AddHeader(main_vbox_layout);

	//Tabs
	m_tab_widget = new Wt::WTabWidget();
	CreateAdministratorTab(m_tab_widget);
	CreateProfileTab(m_tab_widget);
	main_vbox_layout->addWidget(m_tab_widget);

	//Build layout
	setLayout(main_vbox_layout);

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
	appname_text->setStyleClass("modellbasen-appname");
	header_hbox->addWidget(appname_text, 0, Wt::AlignCenter|Wt::AlignMiddle);

	Wt::WContainerWidget* profile_widget = new Wt::WContainerWidget();
	Wt::WVBoxLayout* profile_vbox = new Wt::WVBoxLayout();
	profile_vbox->setContentsMargins(0, 0, 0, 0);
	profile_widget->setLayout(profile_vbox);
	Wt::WPushButton* logout_button = new Wt::WPushButton(Wt::WString::tr("LogoutButton"));
	logout_button->clicked().connect(this, &MainWidget::LogOut);
	profile_vbox->addWidget(logout_button);
	Wt::WPushButton* profile_button = new Wt::WPushButton(Wt::WString::tr("ProfileButton"));
	profile_button->clicked().connect(this, &MainWidget::ShowProfile);
	profile_vbox->addWidget(profile_button);
	header_hbox->addWidget(profile_widget, 0, Wt::AlignRight|Wt::AlignTop);

	layout->addWidget(header_widget);
}

void MainWidget::CreateAdministratorTab(Wt::WTabWidget* tab_widget)
{
	m_administrator_tab = new AdministratorTab(m_app, tab_widget);
}

void MainWidget::CreateProfileTab(Wt::WTabWidget* tab_widget)
{
	m_profile_tab = new Wt::WContainerWidget();

	Wt::WVBoxLayout* profile_vbox = new Wt::WVBoxLayout();
	profile_vbox->setContentsMargins(0, 9, 0, 0);
	   m_profile_tab->setLayout(profile_vbox);

	tab_widget->addTab(m_profile_tab, Wt::WString::tr("ProfileTab"));
}

void MainWidget::ActivateMainWidget()
{
	if (!m_initialized)
		Initialize();

	while (0 < m_app->root()->count())
	{
		Wt::WWidget* widget = m_app->root()->widget(0);
		m_app->root()->removeWidget(widget);
	}
	m_app->root()->addWidget(this);

	int index = m_tab_widget->indexOf(m_administrator_tab);
	bool hidden = !m_app->GetUserManager()->GetCurrentUser()->HasTag(TAG_ADMINISTRATOR);
	m_tab_widget->setTabHidden(index, hidden);
	m_tab_widget->setTabHidden(m_tab_widget->indexOf(m_profile_tab), true);
}

void MainWidget::LogOut()
{
	m_app->GetUserManager()->LogOut();
	m_app->ActivateLoginWidget();
}

void MainWidget::ShowProfile()
{
	int profile_index = m_tab_widget->indexOf(m_profile_tab);
	m_tab_widget->setTabHidden(profile_index, !m_tab_widget->isTabHidden(profile_index));
}
