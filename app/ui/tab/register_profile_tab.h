#ifndef _REGISTER_PROFILE_TAB_H_
#define _REGISTER_PROFILE_TAB_H_

#include <Wt/WContainerWidget>


namespace modellbasen
{

class WebApplication;

class RegisterProfileTab : public Wt::WContainerWidget
{
public:
	RegisterProfileTab(WebApplication* app);
	~RegisterProfileTab();

public:
	void OnLoggedIn();
	void OnLoggedOut();

private:
	WebApplication* m_app;
};

} // namespace modellbasen

#endif // _REGISTER_PROFILE_TAB_H_
