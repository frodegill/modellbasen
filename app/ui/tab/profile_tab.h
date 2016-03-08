#ifndef _PROFILE_TAB_H_
#define _PROFILE_TAB_H_

#include <Wt/WContainerWidget>


namespace modellbasen
{

class WebApplication;

class ProfileTab : public Wt::WContainerWidget
{
public:
	ProfileTab(WebApplication* app);
	~ProfileTab();

public:
	void OnLoggedIn();
	void OnLoggedOut();

private:
	WebApplication* m_app;
};

} // namespace modellbasen

#endif // _PROFILE_TAB_H_
