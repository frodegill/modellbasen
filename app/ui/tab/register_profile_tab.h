#ifndef _REGISTER_PROFILE_TAB_H_
#define _REGISTER_PROFILE_TAB_H_

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>


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

	void OnLoggedIn() {}
	void OnLoggedOut() {}

private:
	Wt::WFormWidget* GetFirstIncompleteFormElement() const;

private:
	WebApplication* m_app;

	Wt::WLineEdit* m_username_edit;
	Wt::WLineEdit* m_password_edit;
	Wt::WLineEdit* m_email_edit;
	Wt::WLineEdit* m_postcode_edit;
	Wt::WText*     m_feedback_text;
};

} // namespace modellbasen

#endif // _REGISTER_PROFILE_TAB_H_
