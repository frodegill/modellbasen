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
	void InitializeNotLoggedInContainer();
	void InitializeLoggedInContainer();

private:
	void UsernameEnterPressed();
	void PasswordEnterPressed();
	void OnLoginButtonClicked(const Wt::WMouseEvent& mouse);
	void OnLogoutButtonClicked(const Wt::WMouseEvent& mouse);
	void LogOut();
	void RequestLogin();
	void LoginFailed();

public:
	void RequestLogin(const std::string& username, const std::string& password);
	void OnLoggedIn();
	void OnLoggedOut();

	void OnPushedRefreshMessagecount();

private:
	WebApplication*  m_app;
	Wt::WContainerWidget* m_not_logged_in_container;
	Wt::WContainerWidget* m_logged_in_container;

	Wt::WLineEdit*   m_username_edit;
	Wt::WLineEdit*   m_password_edit;
	Wt::WPushButton* m_login_button;
	Wt::WAnchor*     m_register_profile_anchor;
	Wt::WAnchor*     m_forgot_password_anchor;
	Wt::WText*       m_login_feedback_text;

	Wt::WPushButton* m_logout_button;

	Wt::WText*       m_username_text;
	Wt::WText*       m_unread_messages_text;
};

} // namespace modellbasen

#endif // _LOGIN_WIDGET_H_
