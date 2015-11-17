#include <Wt/WPushButton>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

#include "search_tab.h"
#include "../dialog/query_dialogs.h"
#include "../../application.h"


using namespace modellbasen;

SearchTab::SearchTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app),
  m_search_tags(NULL),
  m_available_tags(NULL)
{
	m_search = new Search(m_app);

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
	delete m_search;
}

Wt::WContainerWidget* SearchTab::CreateTagsContainer()
{
	Wt::WContainerWidget* tags_container = new Wt::WContainerWidget();
	Wt::WVBoxLayout* tags_container_vbox = new Wt::WVBoxLayout();
	tags_container->setLayout(tags_container_vbox);

	m_search_tags = new Wt::WGroupBox(Wt::WString::tr("Widget.SearchTags"));
	tags_container_vbox->addWidget(m_search_tags, 1);

	m_available_tags = new Wt::WGroupBox(Wt::WString::tr("Widget.AvailableTags"));
	tags_container_vbox->addWidget(m_available_tags, 1);
	
	PopulateTagsContainers();

	return tags_container;
}

Wt::WContainerWidget* SearchTab::CreateResultsContainer() const
{
	Wt::WContainerWidget* results_container = new Wt::WContainerWidget();
	Wt::WGridLayout* results_container_grid = new Wt::WGridLayout();
	results_container->setLayout(results_container_grid);
	return results_container;
}

void SearchTab::PopulateTagsContainers()
{
	m_search_tags->clear();
	std::list<std::shared_ptr<SearchInstance>> searched_instances;
	if (m_search->GetSearchedTags(searched_instances))
	{
		for(std::list<std::shared_ptr<SearchInstance>>::const_iterator it = searched_instances.begin(); it!=searched_instances.end(); ++it)
		{
			std::shared_ptr<SearchInstance> search_instance = *it;
			std::string tag_text;
			if (search_instance->ToString(tag_text))
			{
				Wt::WPushButton* button = new Wt::WPushButton(tag_text);
				button->setMargin(Wt::WLength(0.2, Wt::WLength::FontEm));
				Poco::UInt32 tag_id = search_instance->GetTagId();
				button->clicked().connect(std::bind(&SearchTab::OnSearchInstanceTagButtonClicked, this, tag_id));
				m_search_tags->addWidget(button);
			}
		}
	}

	m_available_tags->clear();
	std::list<Tag> tags;
	if (m_search->GetAvailableTags(tags))
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
			m_available_tags->addWidget(button);
		}
	}
}

void SearchTab::OnSearchInstanceTagButtonClicked(Poco::UInt32 UNUSED(tag_id))
{
}

void SearchTab::OnAvailableTagButtonClicked(Poco::UInt32 tag_id)
{
	Tag tag;
	if (!tag.Initialize(tag_id))
		return;

	QueryDialogs dialogs(m_app);

	Tag::TagDataType query_datatype = tag.GetQueryDataType();
	switch(query_datatype)
	{
		case Tag::INTEGER:
		{
			Poco::UInt32 value;
			if (!dialogs.GetInt(Wt::WString::tr("tag.query.title"),
			                    Wt::WString::tr("tag.query.integer.label"),
			                    value)) return;
			m_search->AddIntegerSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), value);
			break;
		}
		case Tag::STRING:
		case Tag::LOCATION:
		{
			std::string value;
			if (!dialogs.GetString(Wt::WString::tr("tag.query.title"),
			                       Wt::WString::tr(Tag::STRING==query_datatype?"tag.query.string.label":"tag.query.location.label"),
			                       value)) return;
			m_search->AddStringSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), value);
			break;
		}
		case Tag::DATETIME:
		{
			Poco::UInt64 value;
			if (!dialogs.GetDatetime(Wt::WString::tr("tag.query.title"),
			                         Wt::WString::tr("tag.query.datetime.label"),
			                         value)) return;
			m_search->AddDatetimeSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), value);
			break;
		}
		case Tag::BOOLEAN:
		{
			m_search->AddBooleanSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType());
			break;
		}
		case Tag::SINGLESELECT:
		case Tag::MULTISELECT:
		{
			bool multiselect = Tag::MULTISELECT==query_datatype;
			std::list<Poco::UInt32> values;
			if (!dialogs.GetSelect(Wt::WString::tr("tag.query.title"),
			                       Wt::WString::tr(Tag::STRING==query_datatype?"tag.query.singleselect.label":"tag.query.multiselect.label"),
			                       multiselect,
			                       tag,
			                       values)) return;
			m_search->AddStringListSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), values);
			break;
		}
		case Tag::HEIGHT_RANGE:
		case Tag::DAY_RANGE:
		case Tag::AGE_RANGE:
		{
			Wt::WString label1, label2;
			if (Tag::HEIGHT_RANGE==query_datatype)
			{
				label1 = Wt::WString::tr("tag.query.heightrange.label1");
				label2 = Wt::WString::tr("tag.query.heightrange.label2");
			}
			else if (Tag::DAY_RANGE==query_datatype)
			{
				label1 = Wt::WString::tr("tag.query.dayrange.label1");
				label2 = Wt::WString::tr("tag.query.dayrange.label2");
			}
			else
			{
				label1 = Wt::WString::tr("tag.query.agerange.label1");
				label2 = Wt::WString::tr("tag.query.agerange.label2");
			}
			Poco::UInt32 value1, value2;
			if (!dialogs.GetInts(Wt::WString::tr("tag.query.title"),
			                     label1,
			                     label2,
			                     value1, value2)) return;
			m_search->AddIntegerIntegerSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), value1, value2);
			break;
		}
		case Tag::DISTANCE:
		{
			std::string value1;
			Poco::UInt32 value2;
			if (!dialogs.GetDistance(Wt::WString::tr("tag.query.title"),
			                         Wt::WString::tr("tag.query.distance.label1"),
			                         Wt::WString::tr("tag.query.distance.label2"),
			                         value1, value2)) return;
			m_search->AddStringIntegerSearchInstance(tag_id, tag.GetInsertDataType(), tag.GetQueryDataType(), value1, value2);
			break;
		}
		default: break;
	}

	PopulateTagsContainers();
}
