#ifndef _MAIL_TAB_H_
#define _MAIL_TAB_H_

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WSelectionBox>
#include <Wt/WStandardItemModel>
#include <Wt/WTextEdit>
#include <Wt/WTreeView>


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

public:
	void OnNewButtonClicked(const Wt::WMouseEvent& mouse);
	void OnReplyButtonClicked(const Wt::WMouseEvent& mouse);
	void OnDeleteButtonClicked(const Wt::WMouseEvent& mouse);
	void OnSendButtonClicked(const Wt::WMouseEvent& mouse);

	void OnSelectionChanged();

private:
	void RePopulateMailModel();

private:
	WebApplication* m_app;

	Wt::WPushButton* m_new_button;
	Wt::WPushButton* m_reply_button;
	Wt::WPushButton* m_delete_button;

	Wt::WStandardItemModel* m_mail_model;
	Wt::WTreeView* m_mail_tree_view;
	
	Wt::WContainerWidget* m_mail_container;
	Wt::WSelectionBox* m_receiver_selectbox;
	Wt::WLineEdit* m_subject_edit;
	Wt::WTextEdit* m_body_edit;
	Wt::WPushButton* m_send_button;
};

} // namespace modellbasen

#endif // _MAIL_TAB_H_
