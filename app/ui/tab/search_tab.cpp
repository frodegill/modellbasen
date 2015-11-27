#include <Wt/WPushButton>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

#include "search_tab.h"
#include "../../application.h"


using namespace modellbasen;

SearchTab::SearchTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app),
  m_search_tags(NULL),
  m_available_tags(NULL)
{
	m_search = new Search(m_app);
	m_current_query_dialog.reset();

	Wt::WVBoxLayout* tab_container_vbox = new Wt::WVBoxLayout();
	tab_container_vbox->setContentsMargins(0, 9, 0, 0);
	setLayout(tab_container_vbox);

	/*
	 * Actual page
	 */
	m_page_container = new Wt::WContainerWidget();
	Wt::WGridLayout* page_container_grid = new Wt::WGridLayout();
	page_container_grid->setContentsMargins(0, 0, 0, 0);
	m_page_container->setLayout(page_container_grid);

	m_tags_container = CreateTagsContainer();
	m_tags_container->setMaximumSize(Wt::WLength(33.3, Wt::WLength::Percentage), Wt::WLength::Auto);
	page_container_grid->addWidget(m_tags_container, 0, 0, Wt::AlignLeft);

	m_results_container = CreateResultsContainer();
	page_container_grid->addWidget(m_results_container, 0, 1, Wt::AlignJustify);

	tab_container_vbox->addWidget(m_page_container);

	/*
	 * Dialog hack
	 */
	m_dialog_container = new Wt::WContainerWidget();
	m_dialog_container_vbox = new Wt::WVBoxLayout();
	m_dialog_container_vbox->setContentsMargins(0, 0, 0, 0);
	m_dialog_container->setLayout(m_dialog_container_vbox);

	tab_container_vbox->addWidget(m_dialog_container, 0, Wt::AlignCenter);

	/* Show page, hide dialog */
	m_page_container->setHidden(false);
	m_dialog_container->setHidden(true);
}

SearchTab::~SearchTab()
{
	delete m_search;
}

void SearchTab::OnQueryConfirmed()
{
	if (!m_current_query_dialog.get())
		return;

	Tag* query_tag = m_current_query_dialog->GetQueryTag();
	if (!query_tag)
		return;

	Poco::UInt32 tag_id = query_tag->GetId();
	if (!tag_id)
		return;

	Tag::TagDataType query_datatype = query_tag->GetQueryDataType();
	switch(query_datatype)
	{
		case Tag::INTEGER:
		{
			Poco::UInt32 value;
			if (m_current_query_dialog->GetInt(value))
				m_search->AddIntegerSearchInstance(tag_id, query_tag->GetInsertDataType(), query_tag->GetQueryDataType(), value);

			break;
		}
		case Tag::STRING:
		case Tag::LOCATION:
		{
			std::string value;
			if (m_current_query_dialog->GetString(value))
				m_search->AddStringSearchInstance(tag_id, query_tag->GetInsertDataType(), query_tag->GetQueryDataType(), value);

			break;
		}
		case Tag::DATETIME:
		{
			Poco::UInt64 value;
			if (m_current_query_dialog->GetDatetime(value))
				m_search->AddDatetimeSearchInstance(tag_id, query_tag->GetInsertDataType(), query_tag->GetQueryDataType(), value);

			break;
		}
		case Tag::BOOLEAN:
		{
			m_search->AddBooleanSearchInstance(tag_id, query_tag->GetInsertDataType(), query_tag->GetQueryDataType());
			break;
		}
		case Tag::SINGLESELECT:
		{
			Poco::UInt32 value;
			if (m_current_query_dialog->GetSingleSelect(value))
				m_search->AddIntegerSearchInstance(tag_id, query_tag->GetInsertDataType(), query_tag->GetQueryDataType(), value);

			break;
		}
		case Tag::MULTISELECT:
		{
			std::list<Poco::UInt32> values;
			if (m_current_query_dialog->GetMultiSelect(values))
				m_search->AddStringListSearchInstance(tag_id, query_tag->GetInsertDataType(), query_tag->GetQueryDataType(), values);

			break;
		}
		case Tag::HEIGHT_RANGE:
		case Tag::DAY_RANGE:
		case Tag::AGE_RANGE:
		{
			Poco::UInt32 value1, value2;
			if (m_current_query_dialog->GetInts(value1, value2))
				m_search->AddIntegerIntegerSearchInstance(tag_id, query_tag->GetInsertDataType(), query_tag->GetQueryDataType(), value1, value2);

			break;
		}
		case Tag::DISTANCE:
		{
			std::string value1;
			Poco::UInt32 value2;
			if (m_current_query_dialog->GetDistance(value1, value2))
				m_search->AddStringIntegerSearchInstance(tag_id, query_tag->GetInsertDataType(), query_tag->GetQueryDataType(), value1, value2);

			break;
		}
		default: break;
	}
	m_current_query_dialog.reset();
	PopulateTagsContainers();
	m_page_container->setHidden(false);
	m_dialog_container->setHidden(true);
}

void SearchTab::OnQueryCancelled()
{
	m_current_query_dialog.reset();
	m_page_container->setHidden(false);
	m_dialog_container->setHidden(true);
}

Wt::WContainerWidget* SearchTab::CreateTagsContainer()
{
	Wt::WContainerWidget* tags_container = new Wt::WContainerWidget();
	Wt::WVBoxLayout* tags_container_vbox = new Wt::WVBoxLayout();
	tags_container_vbox->setContentsMargins(0, 0, 0, 0);
	tags_container->setLayout(tags_container_vbox);

	m_search_tags = new Wt::WGroupBox(Wt::WString::tr("Widget.SearchTags"));
	m_search_tags->setMaximumSize(Wt::WLength::Auto, Wt::WLength(50.0, Wt::WLength::Percentage));
	tags_container_vbox->addWidget(m_search_tags, 1);

	m_available_tags = new Wt::WGroupBox(Wt::WString::tr("Widget.AvailableTags"));
	m_available_tags->setMaximumSize(Wt::WLength::Auto, Wt::WLength(50.0, Wt::WLength::Percentage));
	tags_container_vbox->addWidget(m_available_tags, 1);
	
	PopulateTagsContainers();

	return tags_container;
}

Wt::WContainerWidget* SearchTab::CreateResultsContainer() const
{
	Wt::WContainerWidget* results_container = new Wt::WContainerWidget();
	Wt::WGridLayout* results_container_grid = new Wt::WGridLayout();
	results_container_grid->setContentsMargins(0, 0, 0, 0);
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

void SearchTab::OnSearchInstanceTagButtonClicked(Poco::UInt32 tag_id)
{
	if (m_search->InvertOrRemoveSearchInstance(tag_id))
		PopulateTagsContainers();
}

void SearchTab::OnAvailableTagButtonClicked(Poco::UInt32 tag_id)
{
	m_current_query_dialog = std::shared_ptr<QueryDialogs>(new QueryDialogs(m_app, this));
	if (!m_current_query_dialog->Initialize(tag_id) ||
	    !m_current_query_dialog->ExecuteAsync())
	{
		OnQueryCancelled();
		return;
	}
}
