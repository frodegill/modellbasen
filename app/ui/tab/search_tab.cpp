#include "search_tab.h"

#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include "../../application.h"


using namespace modellbasen;

SearchTab::SearchTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app)
{
	Wt::WVBoxLayout* tab_container_vbox = new Wt::WVBoxLayout();
	tab_container_vbox->setContentsMargins(0, 9, 0, 0);
	setLayout(tab_container_vbox);
}

SearchTab::~SearchTab()
{
}
