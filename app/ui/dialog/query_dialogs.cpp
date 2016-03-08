#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WGridLayout>
#include <Wt/WLength>

#include "query_dialogs.h"
#include "../tab/search_tab.h"
#include "../../defines.h"


using namespace modellbasen;

QueryDialogs::QueryDialogs(WebApplication* app, SearchTab* search_tab)
: Wt::WObject(),
  m_app(app),
  m_search_tab(search_tab),
  m_int_edit(nullptr),
  m_int2_edit(nullptr),
  m_string_edit(nullptr),
  m_datetime_edit(nullptr),
  m_singleselect_box(nullptr),
  m_multiselect_box(nullptr),
  m_select_box_model(nullptr),
  m_location_gmap(nullptr),
  m_ok_button(nullptr),
  m_cancel_button(nullptr)
{
	m_query_tag.reset();
}

QueryDialogs::~QueryDialogs()
{
}

bool QueryDialogs::Initialize(Poco::UInt32 tag_id)
{
	m_query_tag = std::shared_ptr<Tag>(new Tag());
	if (!m_query_tag->Initialize(tag_id))
	{
		m_query_tag.reset();
		return false;
	}
	return true;
}

bool QueryDialogs::ExecuteAsync()
{
	m_search_tab->GetPageContainer()->setHidden(true);
	m_search_tab->GetDialogContainer()->setHidden(false);
	Wt::WVBoxLayout* container_vbox_layout = m_search_tab->GetDialogLayout();
	container_vbox_layout->clear();

	Tag::TagDataType query_datatype = m_query_tag->GetQueryDataType();
	bool ret = true;
	switch(query_datatype)
	{
		case Tag::INTEGER:
		{
			ret = AddIntControls(container_vbox_layout,
			                     Wt::WString::tr("tag.query.title"),
			                     Wt::WString::tr("tag.query.integer.label"));
			break;
		}
		case Tag::STRING:
		case Tag::LOCATION:
		{
			ret = AddStringControls(container_vbox_layout,
			                        Wt::WString::tr("tag.query.title"),
			                        Wt::WString::tr(Tag::STRING==query_datatype?"tag.query.string.label":"tag.query.location.label"));
			break;
		}
		case Tag::DATETIME:
		{
			ret = AddDatetimeControls(container_vbox_layout,
			                          Wt::WString::tr("tag.query.title"),
			                          Wt::WString::tr("tag.query.datetime.label"));
			break;
		}
		case Tag::BOOLEAN:
		{
			m_search_tab->OnQueryConfirmed();
			return true;
		}
		case Tag::SINGLESELECT:
		{
			ret = AddSingleSelectControls(container_vbox_layout,
			                              Wt::WString::tr("tag.query.title"),
			                              Wt::WString::tr("tag.query.singleselect.label"));
			break;
		}
		case Tag::MULTISELECT:
		{
			ret = AddMultiSelectControls(container_vbox_layout,
			                             Wt::WString::tr("tag.query.title"),
			                             Wt::WString::tr("tag.query.multiselect.label"));
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
			ret = AddIntIntControls(container_vbox_layout,
			                        Wt::WString::tr("tag.query.title"),
			                        label1, label2);
			break;
		}
		case Tag::DISTANCE:
		{
			ret = AddDistanceControls(container_vbox_layout,
			                          Wt::WString::tr("tag.query.title"),
			                          Wt::WString::tr("tag.query.distance.label1"),
			                          Wt::WString::tr("tag.query.distance.label2"));
			break;
		}
		default: ret=false; break;
	}

	if (ret)
	{
		ret = AppendCommonControls(container_vbox_layout);
	}

	return ret;
}

bool QueryDialogs::GetInt(Poco::UInt32& value) const
{
	value = atoi(m_int_edit->text().toUTF8().c_str());
	return true;
}

bool QueryDialogs::GetString(std::string& value) const
{
	value = m_string_edit->text().toUTF8();
	return true;
}

bool QueryDialogs::GetDatetime(Poco::UInt64& value) const
{
	Wt::WDate date = m_datetime_edit->date();
	if (!date.isValid())
		return false;
	
	value = Wt::WDateTime(date).toTime_t();
	return true;
}

bool QueryDialogs::GetSingleSelect(Poco::UInt32& selected_value) const
{
	selected_value = 0;

	int selected_row = m_singleselect_box->currentIndex();
	Wt::WModelIndex model_index = m_select_box_model->index(selected_row, 0);
	selected_value = boost::any_cast<Poco::UInt32>(m_select_box_model->data(model_index, Wt::UserRole));
	return true;
}

bool QueryDialogs::GetMultiSelect(std::list<Poco::UInt32>& selected_values) const
{
	selected_values.clear();

	const std::set<int> selected_rows = m_multiselect_box->selectedIndexes();
	for(std::set<int>::const_iterator it = selected_rows.begin(); it!=selected_rows.end(); ++it)
	{
		int selected_row = *it;
		Wt::WModelIndex model_index = m_select_box_model->index(selected_row, 0);
		selected_values.push_back(boost::any_cast<Poco::UInt32>(m_select_box_model->data(model_index, Wt::UserRole)));
	}
	return true;
}

bool QueryDialogs::GetInts(Poco::UInt32& value1, Poco::UInt32& value2) const
{
	value1 = atoi(m_int_edit->text().toUTF8().c_str());
	value2 = atoi(m_int2_edit->text().toUTF8().c_str());
	return true;
}

bool QueryDialogs::GetStringInt(std::string& value1, Poco::UInt32& value2) const
{
	value1 = m_string_edit->text().toUTF8();
	value2 = atoi(m_int_edit->text().toUTF8().c_str());
	return true;
}

bool QueryDialogs::GetDistance(std::string& postcode, Poco::UInt32& distance) const
{
	postcode = "string1_edit->text().toUTF8()";
	distance = atoi(m_int_edit->text().toUTF8().c_str());
	return true;
}

bool QueryDialogs::AddIntControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label)
{
	Wt::WContainerWidget* controls_container = new Wt::WContainerWidget();
	Wt::WGridLayout* grid_layout = new Wt::WGridLayout();
	grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);
	controls_container->setLayout(grid_layout);

	grid_layout->addWidget(new Wt::WText(title), 0, 0, 1, 2, Wt::AlignCenter);

	grid_layout->addWidget(new Wt::WText(label), 1, 0, Wt::AlignRight);
	m_int_edit = new Wt::WLineEdit();
	m_int_edit->setTextSize(10);
	m_int_edit->setInputMask("000000009");
	grid_layout->addWidget(m_int_edit, 1, 1, Wt::AlignLeft);

	layout->addWidget(controls_container);
	m_int_edit->setFocus();
	return true;
}

bool QueryDialogs::AddStringControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label)
{
	Wt::WContainerWidget* controls_container = new Wt::WContainerWidget();
	Wt::WGridLayout* grid_layout = new Wt::WGridLayout();
	grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);
	controls_container->setLayout(grid_layout);

	grid_layout->addWidget(new Wt::WText(title), 0, 0, 1, 2, Wt::AlignCenter);

	grid_layout->addWidget(new Wt::WText(label), 1, 0, Wt::AlignRight);
	m_string_edit = new Wt::WLineEdit();
	m_string_edit->setTextSize(25);
	m_string_edit->setMaxLength(255);
	grid_layout->addWidget(m_string_edit, 1, 1, Wt::AlignLeft);

	layout->addWidget(controls_container);
	m_string_edit->setFocus();
	return true;
}

bool QueryDialogs::AddDatetimeControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label)
{
	Wt::WContainerWidget* controls_container = new Wt::WContainerWidget();
	Wt::WGridLayout* grid_layout = new Wt::WGridLayout();
	grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);
	controls_container->setLayout(grid_layout);

	grid_layout->addWidget(new Wt::WText(title), 0, 0, 1, 2, Wt::AlignCenter);

	grid_layout->addWidget(new Wt::WText(label), 1, 0, Wt::AlignRight);
	m_datetime_edit = new Wt::WDatePicker();
	grid_layout->addWidget(m_datetime_edit, 1, 1, Wt::AlignLeft);

	layout->addWidget(controls_container);
	return true;
}

bool QueryDialogs::AddSingleSelectControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label)
{
	Wt::WContainerWidget* controls_container = new Wt::WContainerWidget();
	Wt::WGridLayout* grid_layout = new Wt::WGridLayout();
	grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);
	controls_container->setLayout(grid_layout);

	grid_layout->addWidget(new Wt::WText(title), 0, 0, 1, 2, Wt::AlignCenter);

	grid_layout->addWidget(new Wt::WText(label), 1, 0, Wt::AlignLeft);

	std::list<TagValue> tag_values;
	if (!m_query_tag->GetTagValues(tag_values))
		return false;

	m_singleselect_box = new Wt::WComboBox();
	m_select_box_model = new Wt::WStringListModel();
	m_singleselect_box->setModel(m_select_box_model);
	m_singleselect_box->setMinimumSize(Wt::WLength(10.0, Wt::WLength::FontEm), Wt::WLength::Auto);

	PopulateSelectModel(tag_values);

	grid_layout->addWidget(m_singleselect_box, 2, 0, Wt::AlignLeft);

	layout->addWidget(controls_container);
	m_singleselect_box->setFocus();
	return true;
}

bool QueryDialogs::AddMultiSelectControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label)
{
	Wt::WContainerWidget* controls_container = new Wt::WContainerWidget();
	Wt::WGridLayout* grid_layout = new Wt::WGridLayout();
	grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);
	controls_container->setLayout(grid_layout);

	grid_layout->addWidget(new Wt::WText(title), 0, 0, 1, 2, Wt::AlignCenter);

	grid_layout->addWidget(new Wt::WText(label), 1, 0, Wt::AlignLeft);

	std::list<TagValue> tag_values;
	if (!m_query_tag->GetTagValues(tag_values))
		return false;

	m_multiselect_box = new Wt::WSelectionBox();
	m_select_box_model = new Wt::WStringListModel();
	m_multiselect_box->setModel(m_select_box_model);
	m_multiselect_box->setSelectionMode(Wt::ExtendedSelection);
	m_multiselect_box->setVerticalSize(MIN(10, tag_values.size()));
	m_multiselect_box->setMinimumSize(Wt::WLength(10.0, Wt::WLength::FontEm), Wt::WLength::Auto);

	PopulateSelectModel(tag_values);

	grid_layout->addWidget(m_multiselect_box, 2, 0, Wt::AlignLeft);

	layout->addWidget(controls_container);
	m_multiselect_box->setFocus();
	return true;
}

bool QueryDialogs::AddIntIntControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label1, const Wt::WString& label2)
{
	Wt::WContainerWidget* controls_container = new Wt::WContainerWidget();
	Wt::WGridLayout* grid_layout = new Wt::WGridLayout();
	grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);
	controls_container->setLayout(grid_layout);

	grid_layout->addWidget(new Wt::WText(title), 0, 0, 1, 2, Wt::AlignCenter);

	grid_layout->addWidget(new Wt::WText(label1), 1, 0, Wt::AlignRight);
	m_int_edit = new Wt::WLineEdit();
	grid_layout->addWidget(m_int_edit, 1, 1, Wt::AlignLeft);

	grid_layout->addWidget(new Wt::WText(label2), 2, 0, Wt::AlignRight);
	m_int2_edit = new Wt::WLineEdit();
	grid_layout->addWidget(m_int2_edit, 2, 1, Wt::AlignLeft);

	layout->addWidget(controls_container);
	m_int_edit->setFocus();
	return true;
}

bool QueryDialogs::AddDistanceControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& location_label, const Wt::WString& distance_label)
{
	Wt::WContainerWidget* controls_container = new Wt::WContainerWidget();
	Wt::WVBoxLayout* vbox_layout = new Wt::WVBoxLayout();
	vbox_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);
	controls_container->setLayout(vbox_layout);

	vbox_layout->addWidget(new Wt::WText(title), Wt::AlignCenter);

	vbox_layout->addWidget(new Wt::WText(location_label), Wt::AlignCenter);

	m_location_gmap = new Wt::WGoogleMap(Wt::WGoogleMap::Version3);
	m_location_gmap->setMapTypeControl(Wt::WGoogleMap::DefaultControl);
	m_location_gmap->enableScrollWheelZoom();
	vbox_layout->addWidget(m_location_gmap);
	
	vbox_layout->addWidget(new Wt::WText(distance_label), Wt::AlignCenter);
	m_int_edit = new Wt::WLineEdit();
	vbox_layout->addWidget(m_int_edit, Wt::AlignLeft);

	layout->addWidget(controls_container);
	m_int_edit->setFocus();
	return true;
}

bool QueryDialogs::AppendCommonControls(Wt::WVBoxLayout* layout)
{
	Wt::WContainerWidget* buttons_container = new Wt::WContainerWidget();
	Wt::WHBoxLayout* buttons_container_hbox = new Wt::WHBoxLayout();
	buttons_container->setLayout(buttons_container_hbox);

	m_ok_button = new Wt::WPushButton(Wt::WString::tr("Ok"));
	m_ok_button->setDefault(true);
	m_ok_button->clicked().connect(this, &QueryDialogs::OnOK);
	buttons_container_hbox->addWidget(m_ok_button);

	m_cancel_button = new Wt::WPushButton(Wt::WString::tr("Cancel"));
	m_cancel_button->clicked().connect(this, &QueryDialogs::OnCancel);
	buttons_container_hbox->addWidget(m_cancel_button);

	layout->addWidget(buttons_container, 0, Wt::AlignCenter);
	return true;
}

void QueryDialogs::OnOK()
{
	m_search_tab->OnQueryConfirmed();
}

void QueryDialogs::OnCancel()
{
	m_search_tab->OnQueryCancelled();
}

void QueryDialogs::PopulateSelectModel(const std::list<TagValue>& tag_values)
{
	m_select_box_model->removeRows(0, m_select_box_model->rowCount());

	int model_row = 0;
	for(std::list<TagValue>::const_iterator it = tag_values.begin(); it!=tag_values.end(); ++it)
	{
		TagValue tag_value = *it;
		std::string value_text;
		if (tag_value.GetTagValueText(m_app->localizedStrings(), value_text))
		{
			m_select_box_model->insertString(model_row, value_text);
			Wt::WModelIndex index = m_select_box_model->index(model_row, 0);
			m_select_box_model->setData(index, tag_value.GetId(), Wt::UserRole);
		}
	}
}
