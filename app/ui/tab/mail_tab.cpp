#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

#include "mail_tab.h"
#include "../../application.h"
#include "../../defines.h"


using namespace modellbasen;

MailTab::MailTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app),
  m_new_button(nullptr),
  m_reply_button(nullptr),
  m_delete_button(nullptr),
  m_mail_model(nullptr),
  m_mail_tree_view(nullptr)
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
	m_mail_tree_view->setRootIsDecorated(false);
	m_mail_tree_view->setSelectionMode(Wt::SingleSelection);
	m_mail_tree_view->setDragEnabled(false);
  m_mail_tree_view->setDropsEnabled(false);
  m_mail_tree_view->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	tab_container_vbox->addWidget(m_mail_tree_view);
	
}

MailTab::~MailTab()
{
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

void MailTab::RePopulateMailModel()
{
	m_mail_model->clear();
	if (3 > m_mail_model->columnCount())
	{
		m_mail_model->insertColumns(0, 3 - m_mail_model->columnCount());
		m_mail_model->setHeaderData(0, Wt::Horizontal, boost::any(Wt::WString::tr("SubjectHeader")));
		m_mail_model->setHeaderData(1, Wt::Horizontal, boost::any(Wt::WString::tr("FromHeader")));
		m_mail_model->setHeaderData(2, Wt::Horizontal, boost::any(Wt::WString::tr("SentTimeHeader")));
	}
}
