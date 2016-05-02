#include <Wt/WVBoxLayout>

#include "mail_tab.h"
#include "../../application.h"


using namespace modellbasen;

MailTab::MailTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app)
{
	Wt::WVBoxLayout* tab_container_vbox = new Wt::WVBoxLayout();
	tab_container_vbox->setContentsMargins(0, 9, 0, 0);
	setLayout(tab_container_vbox);
}

MailTab::~MailTab()
{
}

void MailTab::OnLoggedIn()
{
}

void MailTab::OnLoggedOut()
{
}

void MailTab::OnPushedRefreshMessagecount()
{
	//TODO
}