#include "search_tab.h"

#include <Wt/WGroupBox>
#include <Wt/WPushButton>
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

	Wt::WGroupBox* search_tags = new Wt::WGroupBox(Wt::WString::tr("Widget.SearchTags"));
	tags_container_vbox->addWidget(search_tags, 1);

	Wt::WGroupBox* available_tags = new Wt::WGroupBox(Wt::WString::tr("Widget.AvailableTags"));
	tags_container_vbox->addWidget(available_tags, 1);
	
	std::list<Tag> tags;
	if (m_search.GetAvailableTags(tags))
	{
		for(std::list<Tag>::const_iterator it = tags.begin(); it!=tags.end(); ++it)
		{
			Tag tag = *it;
			Wt::WPushButton* button = new Wt::WPushButton(tag.GetName());
			button->setMargin(Wt::WLength(0.2, Wt::WLength::FontEm));
			available_tags->addWidget(button);
		}
	}

	return tags_container;
}

Wt::WContainerWidget* SearchTab::CreateResultsContainer() const
{
	Wt::WContainerWidget* results_container = new Wt::WContainerWidget();
	Wt::WGridLayout* results_container_grid = new Wt::WGridLayout();
	results_container->setLayout(results_container_grid);
	return results_container;
}
