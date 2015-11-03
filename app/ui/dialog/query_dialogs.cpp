#include "query_dialogs.h"

//#include <Wt/WDialog>
//#include <Wt/WGridLayout>
//#include <Wt/WLineEdit>
//#include <Wt/WPushButton>
//#include "../../defines.h"
//#include "../../../storage/database_update.h"


using namespace modellbasen;

QueryDialogs::QueryDialogs(WebApplication* app)
: m_app(app)
{

}

bool QueryDialogs::GetInt(Poco::UInt32& UNUSED(value)) const
{
	return false;
}

bool QueryDialogs::GetString(std::string& UNUSED(value)) const
{
	return false;
}

bool QueryDialogs::GetDatetime(Poco::UInt64& UNUSED(value)) const
{
	return false;
}

bool QueryDialogs::GetSingleSelect(std::list<std::string>& UNUSED(value)) const
{
	return false;
}

bool QueryDialogs::GetMultiSelect(std::list<std::string>& UNUSED(value)) const
{
	return false;
}

bool QueryDialogs::GetInts(Poco::UInt32& UNUSED(value1), Poco::UInt32& UNUSED(value2)) const
{
	return false;
}

#if 0
	Wt::WDialog dialog(Wt::WString::tr("CreateLanguage"));

	Wt::WGridLayout* dlg_grid_layout = new Wt::WGridLayout();
	dlg_grid_layout->setContentsMargins(DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING, DEFAULT_PADDING);

	int row = 0;

	//Create language name widget
	dlg_grid_layout->addWidget(new Wt::WText(Wt::WString::tr("LanguageNameHeader")), row, 0, Wt::AlignRight);
	Wt::WLineEdit* language_name_edit = new Wt::WLineEdit();
	language_name_edit->setTextSize(20);
	language_name_edit->setMaxLength(NAME_LEN);
	dlg_grid_layout->addWidget(language_name_edit, row++, 1, Wt::AlignLeft);

	//Create language longname widget
	dlg_grid_layout->addWidget(new Wt::WText(Wt::WString::tr("LanguageLongnameHeader")), row, 0, Wt::AlignRight);
	Wt::WLineEdit* language_longname_edit = new Wt::WLineEdit();
	language_longname_edit->setTextSize(20);
	language_longname_edit->setMaxLength(LONGNAME_LEN);
	dlg_grid_layout->addWidget(language_longname_edit, row++, 1, Wt::AlignLeft);

	//Create language description widget
	dlg_grid_layout->addWidget(new Wt::WText(Wt::WString::tr("LanguageDescriptionHeader")), row, 0, Wt::AlignRight);
	Wt::WLineEdit* language_description_edit = new Wt::WLineEdit();
	language_description_edit->setTextSize(20);
	language_description_edit->setMaxLength(DESCRIPTION_LEN);
	dlg_grid_layout->addWidget(language_description_edit, row++, 1, Wt::AlignLeft);

	Wt::WPushButton* ok_button = new Wt::WPushButton(Wt::WString::tr("Ok"));
	ok_button->clicked().connect(&dialog, &Wt::WDialog::accept);
	dlg_grid_layout->addWidget(ok_button, row, 0, Wt::AlignCenter);

	Wt::WPushButton* cancel_button = new Wt::WPushButton(Wt::WString::tr("Cancel"));
	cancel_button->clicked().connect(&dialog, &Wt::WDialog::reject);
	dlg_grid_layout->addWidget(cancel_button, row++, 1, Wt::AlignCenter);

	//Build layout
	dialog.contents()->setLayout(dlg_grid_layout);
	if (Wt::WDialog::Accepted != dialog.exec())
		return false;

	app->GetRepositoryCodelibrary()->AddLanguage(language_name_edit->text().toUTF8(), language_longname_edit->text().toUTF8(), language_description_edit->text().toUTF8());

	return true;
#endif
