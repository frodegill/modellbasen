#include "search_tab.h"

#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include "../../application.h"


using namespace modellbasen;

SearchTab::SearchTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app)
{
	Wt::WHBoxLayout* tab_container_hbox = new Wt::WHBoxLayout();
	tab_container_hbox->setContentsMargins(0, 9, 0, 0);
	setLayout(tab_container_hbox);

	m_tags_container = CreateTagsContainer();
	tab_container_hbox->addWidget(m_tags_container);

	m_results_container = CreateResultsContainer();
	tab_container_hbox->addWidget(m_results_container);
}

SearchTab::~SearchTab()
{
}

Wt::WContainerWidget* SearchTab::CreateTagsContainer() const
{
	Wt::WContainerWidget* tags_container = new Wt::WContainerWidget();
	Wt::WVBoxLayout* tags_container_vbox = new Wt::WVBoxLayout();
	tags_container->setLayout(tags_container_vbox);
	return tags_container;
}

Wt::WContainerWidget* SearchTab::CreateResultsContainer() const
{
	Wt::WContainerWidget* results_container = new Wt::WContainerWidget();
	Wt::WGridLayout* results_container_grid = new Wt::WGridLayout();
	results_container->setLayout(results_container_grid);
	return results_container;
}
