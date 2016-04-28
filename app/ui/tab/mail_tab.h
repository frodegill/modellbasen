#ifndef _MAIL_TAB_H_
#define _MAIL_TAB_H_

#include <Wt/WContainerWidget>


namespace modellbasen
{

class WebApplication;

class MailTab : public Wt::WContainerWidget
{
public:
	MailTab(WebApplication* app);
	~MailTab();

public:
	void OnLoggedIn();
	void OnLoggedOut();

	void OnPushedRefreshMessagecount();

private:
	WebApplication* m_app;
};

} // namespace modellbasen

#endif // _MAIL_TAB_H_
