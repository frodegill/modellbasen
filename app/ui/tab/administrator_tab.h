#ifndef _ADMINISTRATOR_TAB_H_
#define _ADMINISTRATOR_TAB_H_

//#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>
#include <Wt/WPushButton>
//#include <Wt/WStringListModel>
//#include <Wt/WTabWidget>


namespace modellbasen
{

class WebApplication;

class AdministratorTab : public Wt::WContainerWidget
{
public:
	AdministratorTab(WebApplication* app, Wt::WTabWidget* tab_widget);
	~AdministratorTab();

private:
	void OnImportButtonClicked();
	void OnFileUploaded();
	void OnFileTooLarge();
	bool ImportPostCodeFile(const std::string& filename);

private:
	WebApplication* m_app;

	Wt::WContainerWidget* m_administrator_tab_container;

	Wt::WFileUpload*  m_postcodes_fileupload;
	Wt::WPushButton*  m_import_postcodes_button;
	Wt::WProgressBar* m_import_progressbar;
};

} // namespace modellbasen

#endif // _ADMINISTRATOR_TAB_H_
