#ifndef _MAIL_TAB_H_
#define _MAIL_TAB_H_

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WSuggestionPopup>
#include <Wt/WStandardItemModel>
#include <Wt/WStringListModel>
#include <Wt/WTextEdit>
#include <Wt/WTreeView>


namespace modellbasen
{

class WebApplication;

class MailTab : public Wt::WContainerWidget
{
enum MailVisibility {
	HIDDEN,
	READ_ONLY,
	SHOWN
};

public:
	MailTab(WebApplication* app);
	~MailTab();

private:
	Wt::WLineEdit* CreateReceiverEdit();

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
  void OnFilterSuggestions(const Wt::WString& input);

private:
	void RePopulateMailModel();
	void ShowMailContainer(MailVisibility visibility);

private:
	WebApplication* m_app;

	Wt::WPushButton* m_new_button;
	Wt::WPushButton* m_reply_button;
	Wt::WPushButton* m_delete_button;

	Wt::WStandardItemModel* m_mail_model;
	Wt::WTreeView* m_mail_tree_view;
	
	Wt::WContainerWidget* m_mail_container;
	Wt::WLineEdit* m_receiver_edit;
	Wt::WStringListModel* m_receiver_virtmodel;
	Wt::WSuggestionPopup* m_receiver_popup;
	Wt::WLineEdit* m_subject_edit;
	Wt::WTextArea* m_body_edit;
	Wt::WPushButton* m_send_button;
};

} // namespace modellbasen

#endif // _MAIL_TAB_H_
