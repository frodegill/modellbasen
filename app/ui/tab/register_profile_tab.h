#ifndef _REGISTER_PROFILE_TAB_H_
#define _REGISTER_PROFILE_TAB_H_

#include <Wt/WCheckBox>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>


namespace modellbasen
{

class WebApplication;

class RegisterProfileTab : public Wt::WContainerWidget
{
public:
	RegisterProfileTab(WebApplication* app);
	~RegisterProfileTab();

public:
	void OnRegisterButtonClicked();

	void OnActivateTab();
	void OnLoggedIn() {}
	void OnLoggedOut() {}

private:
	Wt::WFormWidget* GetFirstIncompleteFormElement() const;

private:
	WebApplication* m_app;

	Wt::WLineEdit*   m_username_edit;
	Wt::WLineEdit*   m_password_edit;
	Wt::WLineEdit*   m_email_edit;
	Wt::WLineEdit*   m_postcode_edit;
	Wt::WCheckBox*   m_model_checkbox;
	Wt::WCheckBox*   m_photographer_checkbox;
	Wt::WCheckBox*   m_mua_checkbox;
	Wt::WCheckBox*   m_hairdresser_checkbox;
	Wt::WCheckBox*   m_agency_checkbox;
	Wt::WPushButton* m_register_button;
	Wt::WText*       m_feedback_text;
};

} // namespace modellbasen

#endif // _REGISTER_PROFILE_TAB_H_
