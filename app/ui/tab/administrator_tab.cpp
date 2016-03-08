#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

#include "administrator_tab.h"
#include "../../application.h"
#include "../../../storage/import_postcodes.h"
#include "../../../storage/usermanager.h"
#include "../../../storage/dbo/tag.h"


using namespace modellbasen;

AdministratorTab::AdministratorTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app),
  m_postcodes_fileupload(nullptr),
  m_import_postcodes_button(nullptr),
  m_import_progressbar(nullptr)
{
	Wt::WVBoxLayout* tab_container_vbox = new Wt::WVBoxLayout();
	tab_container_vbox->setContentsMargins(0, 9, 0, 0);
	setLayout(tab_container_vbox);

	const User* current_user = m_app->GetUserManager()->GetCurrentUser();
	if (current_user && current_user->HasTag(TAG_ADMINISTRATOR))
	{
		m_postcodes_fileupload = new Wt::WFileUpload();
		m_postcodes_fileupload->setMultiple(false);
		m_postcodes_fileupload->setFileTextSize(MAX_UPLOAD_SIZE);
		m_postcodes_fileupload->setMargin(10, Wt::Right);
		m_import_progressbar = new Wt::WProgressBar();
		m_postcodes_fileupload->setProgressBar(m_import_progressbar);
		tab_container_vbox->addWidget(m_postcodes_fileupload);

		m_import_postcodes_button = new Wt::WPushButton(Wt::WString::tr("Import.PostCodes"));
		m_import_postcodes_button->clicked().connect(this, &AdministratorTab::OnImportButtonClicked);
		tab_container_vbox->addWidget(m_import_postcodes_button);
	}
}

AdministratorTab::~AdministratorTab()
{
}

void AdministratorTab::OnImportButtonClicked()
{
	if (m_postcodes_fileupload->isUploaded())
	{
		OnFileUploaded();
	}
	else if (m_postcodes_fileupload->canUpload())
	{
		m_postcodes_fileupload->disable();

		m_postcodes_fileupload->uploaded().connect(std::bind([=] () { OnFileUploaded(); }));
		m_postcodes_fileupload->fileTooLarge().connect(std::bind([=] () { OnFileTooLarge(); }));

		m_postcodes_fileupload->upload();
	}
}

void AdministratorTab::OnFileUploaded()
{
	ImportPostCodeFile(m_postcodes_fileupload->spoolFileName());
}

void AdministratorTab::OnFileTooLarge()
{
	m_import_progressbar->setFormat(Wt::WString::tr("Import.FileTooLarge"));
}

bool AdministratorTab::ImportPostCodeFile(const std::string& filename)
{
	Wt::WString status_text;
	PostCodesImporter importer;
	bool ret = importer.Import(filename, m_app, m_import_progressbar, status_text);
	m_import_progressbar->setFormat(status_text);
	return ret;
}

void AdministratorTab::OnLoggedIn()
{
}

void AdministratorTab::OnLoggedOut()
{
}
