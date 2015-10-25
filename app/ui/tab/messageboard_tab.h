#ifndef _MESSAGEBOARD_TAB_H_
#define _MESSAGEBOARD_TAB_H_

#include <Wt/WContainerWidget>


namespace modellbasen
{

class WebApplication;

class MessageBoardTab : public Wt::WContainerWidget
{
public:
	MessageBoardTab(WebApplication* app);
	~MessageBoardTab();

private:
	WebApplication* m_app;
};

} // namespace modellbasen

#endif // _MESSAGEBOARD_TAB_H_
