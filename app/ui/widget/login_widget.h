#ifndef _LOGIN_WIDGET_H_
#define _LOGIN_WIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>


namespace modellbasen
{

class WebApplication;

class LoginWidget : public Wt::WContainerWidget
{
public:
	LoginWidget(WebApplication* app);
	~LoginWidget();

	void Initialize();

private:
	void UsernameEnterPressed();
	void PasswordEnterPressed();
	void OnLoginButtonClicked(const Wt::WMouseEvent& mouse);
	void RequestLogin();
	void LoginFailed();
	void LogOut();

private:
	WebApplication*  m_app;
	Wt::WLineEdit*   m_username_edit;
	Wt::WLineEdit*   m_password_edit;
	Wt::WPushButton* m_login_button;
	Wt::WAnchor*     m_register_profile_anchor;
	Wt::WAnchor*     m_forgot_password_anchor;
	Wt::WText*       m_login_feedback_text;
};

} // namespace modellbasen

#endif // _LOGIN_WIDGET_H_
