#include "query_dialogs.h"

#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WGridLayout>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>


using namespace modellbasen;

QueryDialogs::QueryDialogs(WebApplication* app)
: m_app(app)
{

}

bool QueryDialogs::GetInt(const Wt::WString& title, const Wt::WString& label, Poco::UInt32& UNUSED(value)) const
{
	Wt::WDialog dialog(title);

	Wt::WGridLayout* dlg_grid_layout = new Wt::WGridLayout();
	dlg_grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);

	int row = 0;

	dlg_grid_layout->addWidget(new Wt::WText(label), row, 0, Wt::AlignRight);
	Wt::WLineEdit* int_edit = new Wt::WLineEdit();
	int_edit->setTextSize(10);
	int_edit->setMaxLength(10);
	dlg_grid_layout->addWidget(int_edit, row++, 1, Wt::AlignLeft);

	AppendCommonDialogCode(dialog, dlg_grid_layout, row);

	if (Wt::WDialog::Accepted != dialog.exec())
		return false;

	//TODO Get value

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
	string_edit->setTextSize(50);
	string_edit->setMaxLength(255);
	dlg_grid_layout->addWidget(string_edit, row++, 1, Wt::AlignLeft);

	AppendCommonDialogCode(dialog, dlg_grid_layout, row);

	if (Wt::WDialog::Accepted != dialog.exec())
		return false;

	value = string_edit->text().toUTF8();

	return true;
}

bool QueryDialogs::GetDatetime(const Wt::WString& UNUSED(title), const Wt::WString& UNUSED(label), Poco::UInt64& UNUSED(value)) const
{
	return false;
}

bool QueryDialogs::GetSelect(const Wt::WString& UNUSED(title), const Wt::WString& UNUSED(label), bool UNUSED(multiselect), std::list<std::string>& UNUSED(value)) const
{
	return false;
}

bool QueryDialogs::GetInts(const Wt::WString& UNUSED(title), const Wt::WString& UNUSED(label1), const Wt::WString& UNUSED(label2), Poco::UInt32& UNUSED(value1), Poco::UInt32& UNUSED(value2)) const
{
	return false;
}

bool QueryDialogs::GetStringInt(const Wt::WString& UNUSED(title), const Wt::WString& UNUSED(label1), const Wt::WString& UNUSED(label2), std::string& UNUSED(value1), Poco::UInt32& UNUSED(value2)) const
{
	return false;
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
