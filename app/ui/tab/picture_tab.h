#ifndef _PICTURE_TAB_H_
#define _PICTURE_TAB_H_

#include <Wt/WContainerWidget>


namespace modellbasen
{

class WebApplication;

class PictureTab : public Wt::WContainerWidget
{
public:
	PictureTab(WebApplication* app);
	~PictureTab();

public:
	void OnLoggedIn();
	void OnLoggedOut();

private:
	WebApplication* m_app;
};

} // namespace modellbasen

#endif // _PICTURE_TAB_H_
