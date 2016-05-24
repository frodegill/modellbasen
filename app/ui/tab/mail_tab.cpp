#include <Wt/WStandardItem>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

#include "mail_tab.h"
#include "../../application.h"
#include "../../../singleton/push.h"
#include "../../../storage/usermanager.h"
#include "../../../storage/dbo/message.h"
#include "../../../utils/time.h"

/*
 * See also: http://www.webtoolkit.eu/wt/blog/2010/03/02/javascript_that_is_c__
 */
#define INLINE_JAVASCRIPT(...) #__VA_ARGS__


using namespace modellbasen;

MailTab::MailTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app),
  m_new_button(nullptr),
  m_reply_button(nullptr),
  m_delete_button(nullptr),
  m_mail_model(nullptr),
  m_mail_tree_view(nullptr),
  m_selected_mail_id(INVALID_ID),
  m_mail_container(nullptr),
  m_receiver_edit(nullptr),
  m_receiver_virtmodel(nullptr),
  m_receiver_popup(nullptr),
  m_subject_edit(nullptr),
  m_body_edit(nullptr),
  m_send_button(nullptr)
{
	Wt::WVBoxLayout* tab_container_vbox = new Wt::WVBoxLayout();
	tab_container_vbox->setContentsMargins(0, 9, 0, 0);
	setLayout(tab_container_vbox);

	Wt::WContainerWidget* buttons_container = new Wt::WContainerWidget();
	Wt::WHBoxLayout* buttons_container_hbox = new Wt::WHBoxLayout();
	buttons_container_hbox->setContentsMargins(0, 0, 0, 0);
	buttons_container->setLayout(buttons_container_hbox);

	m_new_button = new Wt::WPushButton(Wt::WString::tr("NewButton"));
	m_new_button->clicked().connect(this, &MailTab::OnNewButtonClicked);
	buttons_container_hbox->addWidget(m_new_button);

	m_reply_button = new Wt::WPushButton(Wt::WString::tr("ReplyButton"));
	m_reply_button->clicked().connect(this, &MailTab::OnReplyButtonClicked);
	buttons_container_hbox->addWidget(m_reply_button);

	m_delete_button = new Wt::WPushButton(Wt::WString::tr("DeleteButton"));
	m_delete_button->clicked().connect(this, &MailTab::OnDeleteButtonClicked);
	buttons_container_hbox->addWidget(m_delete_button);

	tab_container_vbox->addWidget(buttons_container, 0, Wt::AlignLeft);

	m_mail_model = new Wt::WStandardItemModel();
	m_mail_tree_view = new Wt::WTreeView();
	m_mail_tree_view->setModel(m_mail_model);
	m_mail_tree_view->setRootIsDecorated(true);
	m_mail_tree_view->setSelectionMode(Wt::SingleSelection);
	m_mail_tree_view->setDragEnabled(false);
  m_mail_tree_view->setDropsEnabled(false);
  m_mail_tree_view->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	m_mail_tree_view->selectionChanged().connect(this, &MailTab::OnSelectionChanged);
	tab_container_vbox->addWidget(m_mail_tree_view);
	
  m_mail_container = new Wt::WContainerWidget();
	Wt::WGridLayout* mail_container_grid = new Wt::WGridLayout();
	mail_container_grid->setContentsMargins(0, 9, 0, 0);
	m_mail_container->setLayout(mail_container_grid);
	mail_container_grid->setColumnStretch(0, NO_STRETCH);
	mail_container_grid->setColumnStretch(1, STRETCH);

	int row = 0;
	mail_container_grid->addWidget(new Wt::WText(Wt::WString::tr("Receiver")), row, 0, Wt::AlignRight);
	mail_container_grid->addWidget(CreateReceiverEdit(), row++, 1, Wt::AlignLeft);
 
	mail_container_grid->addWidget(new Wt::WText(Wt::WString::tr("Subject")), row, 0, Wt::AlignRight);
	m_subject_edit = new Wt::WLineEdit();
	mail_container_grid->addWidget(m_subject_edit, row++, 1, Wt::AlignJustify);

  m_body_edit = new Wt::WTextArea();
	mail_container_grid->setRowStretch(row, true);
	mail_container_grid->addWidget(m_body_edit, row++, 0, 1, 2, Wt::AlignJustify);

	m_send_button = new Wt::WPushButton(Wt::WString::tr("SendButton"));
	m_send_button->clicked().connect(this, &MailTab::OnSendButtonClicked);
	mail_container_grid->addWidget(m_send_button, row++, 0, 1, 2, Wt::AlignCenter);
	
	tab_container_vbox->addWidget(m_mail_container);

	ShowMailContainer(HIDDEN);
}

MailTab::~MailTab()
{
}

Wt::WLineEdit* MailTab::CreateReceiverEdit()
{
	std::string matcherJS = INLINE_JAVASCRIPT
	(
		function(edit) {
			var value = edit.value;
			return function(suggestion) {
				if (!suggestion)
					return {match:false, suggestion:""};
				return {match:true, suggestion:suggestion};
			}
		}
	);

	std::string replacerJS = INLINE_JAVASCRIPT
	(
		function(edit, suggestionText, suggestionValue) {edit.value=suggestionValue;}
	);

	m_receiver_edit = new Wt::WLineEdit();
	m_receiver_virtmodel = new Wt::WStringListModel();
	m_receiver_popup = new Wt::WSuggestionPopup(matcherJS, replacerJS);
	m_receiver_popup->setModel(m_receiver_virtmodel);
	m_receiver_popup->setFilterLength(-1);
	m_receiver_popup->setDropDownIconUnfiltered(true);
	m_receiver_popup->filterModel().connect(this, &MailTab::OnFilterSuggestions);
	m_receiver_popup->setMinimumSize(150, Wt::WLength::Auto);
	m_receiver_popup->setMaximumSize(Wt::WLength::Auto, 300);
	m_receiver_popup->forEdit(m_receiver_edit, Wt::WSuggestionPopup::Editing);
	return m_receiver_edit;
}

void MailTab::OnLoggedIn()
{
	RePopulateMailModel();
}

void MailTab::OnLoggedOut()
{
}

void MailTab::OnPushedRefreshMessagecount()
{
	//TODO
}

void MailTab::OnNewButtonClicked(const Wt::WMouseEvent& UNUSED(mouse))
{
}

void MailTab::OnReplyButtonClicked(const Wt::WMouseEvent& UNUSED(mouse))
{
}

void MailTab::OnDeleteButtonClicked(const Wt::WMouseEvent& UNUSED(mouse))
{
}

void MailTab::OnSendButtonClicked(const Wt::WMouseEvent& UNUSED(mouse))
{
	std::string receiver_username = m_receiver_edit->text().toUTF8();
	IdType receiver_id;
	if (receiver_username.empty() ||
	    !m_app->GetUserManager()->GetUserId(receiver_username, receiver_id) ||
	    INVALID_ID==receiver_id)
		return;

	std::string subject = m_subject_edit->text().toUTF8();
	std::string body = m_body_edit->text().toUTF8();
	if (body.empty())
		return;

	Poco::Data::Session* session_in_transaction;
	if (!DB.CreateSession(session_in_transaction))
		return;

	bool ret = Message::SendMessage(session_in_transaction,
	                                subject, body,
	                                m_app->GetUserManager()->GetCurrentUser()->GetId(),
	                                receiver_id,
	                                m_selected_mail_id);

	DB.ReleaseSession(session_in_transaction, ret ? PocoGlue::COMMIT : PocoGlue::ROLLBACK);

	::NotifyMessageToUser(receiver_username);
}

void MailTab::OnSelectionChanged()
{
	m_selected_mail_id = INVALID_ID;
	Wt::WModelIndexSet selected_indexes = m_mail_tree_view->selectedIndexes();
	if (!selected_indexes.empty())
	{
		m_selected_mail_id = boost::any_cast<IdType>(selected_indexes.begin()->data(IdRole));
	}

	if (INVALID_ID==m_selected_mail_id)
	{
		ShowMailContainer(HIDDEN);
		return;
	}

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return;

	Message message;
	if (!Message::GetMessage(session, m_selected_mail_id, message))
	{
		m_receiver_edit->setText("");
		m_subject_edit->setText("");
		m_body_edit->setText("");
	}
	else
	{
		std::string receiver;
		if (m_app->GetUserManager()->GetUsername(session, message.GetRecipientId(), receiver))
			m_receiver_edit->setText(Wt::WString::fromUTF8(receiver));
		
		m_subject_edit->setText(message.GetSubject());
		m_body_edit->setText(message.GetText());
	}
	ShowMailContainer(READ_ONLY);

	DB.ReleaseSession(session, PocoGlue::IGNORE);
}

void MailTab::OnFilterSuggestions(const Wt::WString& input)
{
	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return;

	UserManager::GetMatchingUsernames(session, input.toUTF8(), *m_receiver_virtmodel);
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);
}

void MailTab::RePopulateMailModel()
{
	//Get current selection
	IdType selected_id = INVALID_ID;
	Wt::WModelIndexSet selected_indexes = m_mail_tree_view->selectedIndexes();
	if (!selected_indexes.empty())
	{
		selected_id = boost::any_cast<IdType>(selected_indexes.begin()->data(IdRole));
	}
	
	//Clear model
	m_mail_model->clear();
	if (4 > m_mail_model->columnCount())
	{
		m_mail_model->insertColumns(0, 4 - m_mail_model->columnCount());
		m_mail_model->setHeaderData(0, Wt::Horizontal, boost::any(Wt::WString::tr("SubjectHeader")));
		m_mail_model->setHeaderData(1, Wt::Horizontal, boost::any(Wt::WString::tr("FromHeader")));
		m_mail_model->setHeaderData(2, Wt::Horizontal, boost::any(Wt::WString::tr("ToHeader")));
		m_mail_model->setHeaderData(3, Wt::Horizontal, boost::any(Wt::WString::tr("SentTimeHeader")));
	}

	//Regenerate model content
	UserManager* user_manager = m_app->GetUserManager();
	const User* user = user_manager->GetCurrentUser();
	Poco::Data::Session* session;
	if (!user || !DB.CreateSession(session))
		return;

	IdType user_id = user->GetId();

	IdType id;
	std::string subject;
	IdType sender_id;
	IdType recipient_id;
	TimeType sent_time;
	TimeType read_time;
	IdType in_reply_to;
	bool sender_deleted;
	bool recipient_deleted;
	Poco::Data::Statement statement(*session);
	statement << "SELECT id, subject, sender, recipient, sent_time, read_time, in_reply_to, sender_deleted, recipient_deleted "\
	                           "FROM message "\
	                           "WHERE ((sender=? AND NOT sender_deleted) OR (recipient=? AND NOT recipient_deleted)) "\
	                           "ORDER BY id",
		Poco::Data::Keywords::into(id),
		Poco::Data::Keywords::into(subject),
		Poco::Data::Keywords::into(sender_id),
		Poco::Data::Keywords::into(recipient_id),
		Poco::Data::Keywords::into(sent_time),
		Poco::Data::Keywords::into(read_time),
		Poco::Data::Keywords::into(in_reply_to),
		Poco::Data::Keywords::into(sender_deleted),
		Poco::Data::Keywords::into(recipient_deleted),
		Poco::Data::Keywords::use(user_id),
		Poco::Data::Keywords::use(user_id),
		Poco::Data::Keywords::range<Poco::Data::Limit::SizeT>(0,1);


	Poco::Data::Session* username_session;
	if (!DB.CreateSession(username_session))
	{
		DB.ReleaseSession(session, PocoGlue::IGNORE); //Cleanup already created session
		return;
	}

	std::unordered_map<IdType, Wt::WStandardItem*> item_cache;
	std::string sender_username;
	std::string recipient_username;
	std::string posted_time;
	int row = 0;
	while (!statement.done() && 0<statement.execute())
	{
		Wt::WStandardItem* mail_item = new Wt::WStandardItem(Wt::WString::fromUTF8(subject));
		mail_item->setData(boost::any((const IdType)id), IdRole);
		bool unread = EPOCH==read_time;
		mail_item->setStyleClass(unread ? "unread" : "read");

		if (in_reply_to && item_cache.end()!=item_cache.find(in_reply_to))
		{
			Wt::WStandardItem* parent_item = item_cache[in_reply_to];
			int child_row = parent_item->rowCount();
			parent_item->setChild(child_row, 0, mail_item);
			if (user_manager->GetUsername(username_session, sender_id, sender_username))
			{
				parent_item->setChild(child_row, 1, new Wt::WStandardItem(Wt::WString::fromUTF8(sender_username)));
			}
			if (user_manager->GetUsername(username_session, recipient_id, recipient_username))
			{
				parent_item->setChild(child_row, 2, new Wt::WStandardItem(Wt::WString::fromUTF8(recipient_username)));
			}
			Time::ToString(sent_time, posted_time);
			parent_item->setChild(child_row, 3, new Wt::WStandardItem(Wt::WString::fromUTF8(posted_time)));
			
			if (unread)
			{
				Wt::WStandardItem* parent_to_expand = parent_item;
				while (parent_to_expand)
				{
					m_mail_tree_view->expand(parent_to_expand->index());
					parent_to_expand = parent_to_expand->parent();
				}
			}
		}
		else {
			m_mail_model->setItem(row, 0, mail_item);
			if (user_manager->GetUsername(username_session, sender_id, sender_username))
			{
				m_mail_model->setItem(row, 1, new Wt::WStandardItem(Wt::WString::fromUTF8(sender_username)));
			}
			if (user_manager->GetUsername(username_session, recipient_id, recipient_username))
			{
				m_mail_model->setItem(row, 2, new Wt::WStandardItem(Wt::WString::fromUTF8(recipient_username)));
			}
			Time::ToString(sent_time, posted_time);
			m_mail_model->setItem(row++, 3, new Wt::WStandardItem(Wt::WString::fromUTF8(posted_time)));
		}
		item_cache[id] = mail_item;
		
		if (id==selected_id)
		{
			Wt::WModelIndexSet select_indexes;
			select_indexes.insert(mail_item->index());
			m_mail_tree_view->setSelectedIndexes(select_indexes);
		}
	}
	DB.ReleaseSession(username_session, PocoGlue::IGNORE);

	DB.ReleaseSession(session, PocoGlue::IGNORE);

	m_mail_model->sort(0);
}

void MailTab::ShowMailContainer(MailVisibility visibility)
{
	switch(visibility)
	{
		case HIDDEN:
		{
			m_mail_container->hide();
			break;
		}
		case READ_ONLY:
		{
			m_mail_container->show();
			m_receiver_edit->setReadOnly(true);
			m_subject_edit->setReadOnly(true);
			m_body_edit->setReadOnly(true);
			m_send_button->hide();
			break;
		}
		case SHOWN:
		{
			m_mail_container->show();
			m_receiver_edit->setReadOnly(false);
			m_subject_edit->setReadOnly(false);
			m_body_edit->setReadOnly(false);
			m_send_button->show();
			break;
		}
	};
}
