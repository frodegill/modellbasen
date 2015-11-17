#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

#include "messageboard_tab.h"
#include "../../application.h"


using namespace modellbasen;

MessageBoardTab::MessageBoardTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app)
{
	Wt::WVBoxLayout* tab_container_vbox = new Wt::WVBoxLayout();
	tab_container_vbox->setContentsMargins(0, 9, 0, 0);
	setLayout(tab_container_vbox);
}

MessageBoardTab::~MessageBoardTab()
{
}
