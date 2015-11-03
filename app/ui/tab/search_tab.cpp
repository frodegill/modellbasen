#include "search_tab.h"

#include <Wt/WGroupBox>
#include <Wt/WPushButton>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include "../../application.h"
#include "../dialog/query_dialogs.h"


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

Wt::WContainerWidget* SearchTab::CreateTagsContainer()
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
			std::string tag_text;
			tag.GetTagText(m_app->localizedStrings(), true, tag_text);
			Wt::WPushButton* button = new Wt::WPushButton(tag_text);
			button->setMargin(Wt::WLength(0.2, Wt::WLength::FontEm));
			Poco::UInt32 tag_id = tag.GetId();
			button->clicked().connect(std::bind(&SearchTab::OnAvailableTagButtonClicked, this, tag_id));
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

void SearchTab::OnAvailableTagButtonClicked(Poco::UInt32 tag_id)
{
	Tag tag;
	if (!tag.Initialize(tag_id))
		return;

	QueryDialogs dialogs(m_app);

	switch(tag.GetQueryDataType())
	{
		case Tag::INTEGER:
		case Tag::LOCATION:
		{
			Poco::UInt32 value;
			if (!dialogs.GetInt(value)) return;
			m_search.AddIntegerSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), value);
			break;
		}
		case Tag::STRING:
		{
			std::string value;
			if (!dialogs.GetString(value)) return;
			m_search.AddStringSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), value);
			break;
		}
		case Tag::DATETIME:
		{
			Poco::UInt64 value;
			if (!dialogs.GetDatetime(value)) return;
			m_search.AddDatetimeSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), value);
			break;
		}
		case Tag::BOOLEAN:
		{
			m_search.AddBooleanSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType());
			break;
		}
		case Tag::SINGLESELECT:
		{
			std::list<std::string> values;
			if (!dialogs.GetSingleSelect(values)) return;
			m_search.AddStringListSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), values);
			break;
		}
		case Tag::MULTISELECT:
		{
			std::list<std::string> values;
			if (!dialogs.GetMultiSelect(values)) return;
			m_search.AddStringListSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), values);
			break;
		}
		case Tag::HEIGHT_RANGE:
		case Tag::DAY_RANGE:
		case Tag::AGE_RANGE:
		case Tag::DISTANCE:
		{
			Poco::UInt32 value1, value2;
			if (!dialogs.GetInts(value1, value2)) return;
			m_search.AddIntegerIntegerSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), value1, value2);
			break;
		}
		default: return;
	}
}
