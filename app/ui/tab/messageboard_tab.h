#ifndef _MESSAGEBOARD_TAB_H_
#define _MESSAGEBOARD_TAB_H_

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WTableView>


namespace modellbasen
{

class WebApplication;

class MessageBoardTab : public Wt::WContainerWidget
{
public:
	MessageBoardTab(WebApplication* app);
	~MessageBoardTab();

private:
	void OnSubmitTextSubmit();
	void OnSubmitButtonClicked(const Wt::WMouseEvent& mouse);

public:
	void OnLoggedIn();
	void OnLoggedOut();

private:
	WebApplication*  m_app;

	Wt::WTableView*  m_table_view;
	Wt::WLineEdit*   m_submit_text;
	Wt::WPushButton* m_submit_button;
};

} // namespace modellbasen

#endif // _MESSAGEBOARD_TAB_H_
