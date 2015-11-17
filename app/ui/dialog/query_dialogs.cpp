#include <Wt/WDatePicker>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WGoogleMap>
#include <Wt/WGridLayout>
#include <Wt/WLength>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WSelectionBox>
#include <Wt/WStringListModel>

#include "query_dialogs.h"


using namespace modellbasen;

QueryDialogs::QueryDialogs(WebApplication* app)
: m_app(app)
{

}

bool QueryDialogs::GetInt(const Wt::WString& title, const Wt::WString& label, Poco::UInt32& value) const
{
	Wt::WDialog dialog(title);

	Wt::WGridLayout* dlg_grid_layout = new Wt::WGridLayout();
	dlg_grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);

	int row = 0;

	dlg_grid_layout->addWidget(new Wt::WText(label), row, 0, Wt::AlignRight);
	Wt::WLineEdit* int_edit = new Wt::WLineEdit();
	int_edit->setTextSize(10);
	int_edit->setInputMask("000000009");
	dlg_grid_layout->addWidget(int_edit, row++, 1, Wt::AlignLeft);

	AppendCommonDialogCode(dialog, dlg_grid_layout, row);

	if (Wt::WDialog::Accepted != dialog.exec())
		return false;

	value = atoi(int_edit->text().toUTF8().c_str());

	return true;
}

bool QueryDialogs::GetString(const Wt::WString& title, const Wt::WString& label, std::string& value) const
{
	Wt::WDialog dialog(title);

	Wt::WGridLayout* dlg_grid_layout = new Wt::WGridLayout();
	dlg_grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);

	int row = 0;

	dlg_grid_layout->addWidget(new Wt::WText(label), row, 0, Wt::AlignRight);
	Wt::WLineEdit* string_edit = new Wt::WLineEdit();
	string_edit->setTextSize(25);
	string_edit->setMaxLength(255);
	dlg_grid_layout->addWidget(string_edit, row++, 1, Wt::AlignLeft);

	AppendCommonDialogCode(dialog, dlg_grid_layout, row);

	if (Wt::WDialog::Accepted != dialog.exec())
		return false;

	value = string_edit->text().toUTF8();

	return true;
}

bool QueryDialogs::GetDatetime(const Wt::WString& title, const Wt::WString& label, Poco::UInt64& value) const
{
	Wt::WDialog dialog(title);

	Wt::WGridLayout* dlg_grid_layout = new Wt::WGridLayout();
	dlg_grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);

	int row = 0;

	dlg_grid_layout->addWidget(new Wt::WText(label), row, 0, Wt::AlignRight);
	Wt::WDatePicker* datetime_edit = new Wt::WDatePicker();
	dlg_grid_layout->addWidget(datetime_edit, row++, 1, Wt::AlignLeft);

	AppendCommonDialogCode(dialog, dlg_grid_layout, row);

	if (Wt::WDialog::Accepted != dialog.exec())
		return false;

	Wt::WDate date = datetime_edit->date();
	if (!date.isValid())
		return false;
	
	value = Wt::WDateTime(date).toTime_t();

	return true;
}

bool QueryDialogs::GetSelect(const Wt::WString& title, const Wt::WString& label, bool multiselect,
                             const Tag& tag, std::list<Poco::UInt32>& selected_values) const
{
	Wt::WDialog dialog(title);

	Wt::WGridLayout* dlg_grid_layout = new Wt::WGridLayout();
	dlg_grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);

	int row = 0;

	dlg_grid_layout->addWidget(new Wt::WText(label), row++, 0, Wt::AlignLeft);
	Wt::WSelectionBox* select_box = new Wt::WSelectionBox();
	Wt::WStringListModel* select_box_model = new Wt::WStringListModel();
	select_box->setModel(select_box_model);
	select_box->setSelectionMode(multiselect ? Wt::ExtendedSelection : Wt::SingleSelection);

	std::list<TagValue> tag_values;
	if (!tag.GetTagValues(tag_values))
		return false;

	select_box->setVerticalSize(MIN(10, tag_values.size()));
	select_box->setMinimumSize(Wt::WLength(10.0, Wt::WLength::FontEm), Wt::WLength::Auto);

	int model_row = 0;
	for(std::list<TagValue>::const_iterator it = tag_values.begin(); it!=tag_values.end(); ++it)
	{
		TagValue tag_value = *it;
		std::string value_text;
		if (tag_value.GetTagValueText(m_app->localizedStrings(), value_text))
		{
			select_box_model->insertString(model_row, value_text);
			Wt::WModelIndex index = select_box_model->index(model_row, 0);
			select_box_model->setData(index, tag_value.GetId(), Wt::UserRole);
		}
	}

	dlg_grid_layout->addWidget(select_box, row++, 0, Wt::AlignLeft);

	AppendCommonDialogCode(dialog, dlg_grid_layout, row);

	if (Wt::WDialog::Accepted != dialog.exec())
		return false;

	selected_values.clear();
	if (multiselect)
	{
		const std::set<int> selected_rows = select_box->selectedIndexes();
		for(std::set<int>::const_iterator it = selected_rows.begin(); it!=selected_rows.end(); ++it)
		{
			int selected_row = *it;
			Wt::WModelIndex model_index = select_box_model->index(selected_row, 0);
			selected_values.push_back(boost::any_cast<Poco::UInt32>(select_box_model->data(model_index, Wt::UserRole)));
		}
	}
	else
	{
		int selected_row = select_box->currentIndex();
		if (-1 != selected_row)
		{
			Wt::WModelIndex model_index = select_box_model->index(selected_row, 0);
			selected_values.push_back(boost::any_cast<Poco::UInt32>(select_box_model->data(model_index, Wt::UserRole)));
		}
	}

	return true;
}

bool QueryDialogs::GetInts(const Wt::WString& title, const Wt::WString& label1, const Wt::WString& label2, Poco::UInt32& value1, Poco::UInt32& value2) const
{
	Wt::WDialog dialog(title);

	Wt::WGridLayout* dlg_grid_layout = new Wt::WGridLayout();
	dlg_grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);

	int row = 0;

	dlg_grid_layout->addWidget(new Wt::WText(label1), row, 0, Wt::AlignRight);
	Wt::WLineEdit* int1_edit = new Wt::WLineEdit();
	dlg_grid_layout->addWidget(int1_edit, row++, 1, Wt::AlignLeft);

	dlg_grid_layout->addWidget(new Wt::WText(label2), row, 0, Wt::AlignRight);
	Wt::WLineEdit* int2_edit = new Wt::WLineEdit();
	dlg_grid_layout->addWidget(int2_edit, row++, 1, Wt::AlignLeft);

	AppendCommonDialogCode(dialog, dlg_grid_layout, row);

	if (Wt::WDialog::Accepted != dialog.exec())
		return false;

	value1 = atoi(int1_edit->text().toUTF8().c_str());
	value2 = atoi(int2_edit->text().toUTF8().c_str());

	return true;
}

bool QueryDialogs::GetStringInt(const Wt::WString& title, const Wt::WString& label1, const Wt::WString& label2, std::string& value1, Poco::UInt32& value2) const
{
	Wt::WDialog dialog(title);

	Wt::WGridLayout* dlg_grid_layout = new Wt::WGridLayout();
	dlg_grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);

	int row = 0;

	dlg_grid_layout->addWidget(new Wt::WText(label1), row, 0, Wt::AlignRight);
	Wt::WLineEdit* string1_edit = new Wt::WLineEdit();
	string1_edit->setTextSize(25);
	string1_edit->setMaxLength(255);
	dlg_grid_layout->addWidget(string1_edit, row++, 1, Wt::AlignLeft);

	dlg_grid_layout->addWidget(new Wt::WText(label2), row, 0, Wt::AlignRight);
	Wt::WLineEdit* int2_edit = new Wt::WLineEdit();
	dlg_grid_layout->addWidget(int2_edit, row++, 1, Wt::AlignLeft);

	AppendCommonDialogCode(dialog, dlg_grid_layout, row);

	if (Wt::WDialog::Accepted != dialog.exec())
		return false;

	value1 = string1_edit->text().toUTF8();
	value2 = atoi(int2_edit->text().toUTF8().c_str());

	return true;
}

bool QueryDialogs::GetDistance(const Wt::WString& title, const Wt::WString& UNUSED(location_label), const Wt::WString& UNUSED(distance_label),
															 std::string& UNUSED(postcode), Poco::UInt32& UNUSED(distance)) const
{
	Wt::WDialog dialog(title);

#if 1
	dialog.setHeight(600);

	Wt::WVBoxLayout* dlg_grid_layout = new Wt::WVBoxLayout();
	dlg_grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);

	Wt::WGoogleMap* location_gmap = new Wt::WGoogleMap(Wt::WGoogleMap::Version3);
	location_gmap->setMapTypeControl(Wt::WGoogleMap::DefaultControl);
	location_gmap->enableScrollWheelZoom();
	dlg_grid_layout->addWidget(location_gmap);
	dialog.contents()->setLayout(dlg_grid_layout);
	dialog.exec();
#else
	Wt::WGridLayout* dlg_grid_layout = new Wt::WGridLayout();
	dlg_grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);

	int row = 0;

	dlg_grid_layout->addWidget(new Wt::WText(location_label), row, 0, Wt::AlignRight);
	Wt::WGoogleMap* location_gmap = new Wt::WGoogleMap(Wt::WGoogleMap::Version3);
	location_gmap->setMapTypeControl(Wt::WGoogleMap::DefaultControl);
	location_gmap->enableScrollWheelZoom();
	dlg_grid_layout->addWidget(location_gmap, row++, 1, Wt::AlignLeft);

	dlg_grid_layout->addWidget(new Wt::WText(distance_label), row, 0, Wt::AlignRight);
	Wt::WLineEdit* distance_edit = new Wt::WLineEdit();
	dlg_grid_layout->addWidget(distance_edit, row++, 1, Wt::AlignLeft);

	AppendCommonDialogCode(dialog, dlg_grid_layout, row);

	if (Wt::WDialog::Accepted != dialog.exec())
		return false;

	postcode = "string1_edit->text().toUTF8()";
	distance = atoi(distance_edit->text().toUTF8().c_str());
#endif
	return true;
}

void QueryDialogs::AppendCommonDialogCode(Wt::WDialog& dialog, Wt::WGridLayout* layout, int row) const
{
	Wt::WContainerWidget* buttons_container = new Wt::WContainerWidget();
	Wt::WHBoxLayout* buttons_container_hbox = new Wt::WHBoxLayout();
	buttons_container->setLayout(buttons_container_hbox);

	Wt::WPushButton* ok_button = new Wt::WPushButton(Wt::WString::tr("Ok"));
	ok_button->setDefault(true);
	ok_button->clicked().connect(&dialog, &Wt::WDialog::accept);
	buttons_container_hbox->addWidget(ok_button);

	Wt::WPushButton* cancel_button = new Wt::WPushButton(Wt::WString::tr("Cancel"));
	cancel_button->clicked().connect(&dialog, &Wt::WDialog::reject);
	buttons_container_hbox->addWidget(cancel_button);

	layout->addWidget(buttons_container, row, 0, 1, 2, Wt::AlignCenter);

	dialog.contents()->setLayout(layout);
}
