#ifndef _ADMINISTRATOR_TAB_H_
#define _ADMINISTRATOR_TAB_H_

#include <Wt/WContainerWidget>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>
#include <Wt/WPushButton>


namespace modellbasen
{

class WebApplication;

class AdministratorTab : public Wt::WContainerWidget
{
public:
	AdministratorTab(WebApplication* app);
	~AdministratorTab();

private:
	void OnImportButtonClicked();
	void OnFileUploaded();
	void OnFileTooLarge();
	bool ImportPostCodeFile(const std::string& filename);

public:
	void OnLoggedIn();
	void OnLoggedOut();

private:
	WebApplication* m_app;

	Wt::WFileUpload*  m_postcodes_fileupload;
	Wt::WPushButton*  m_import_postcodes_button;
	Wt::WProgressBar* m_import_progressbar;
};

} // namespace modellbasen

#endif // _ADMINISTRATOR_TAB_H_
