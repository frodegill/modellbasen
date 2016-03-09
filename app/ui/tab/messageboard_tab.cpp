#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>

#include "messageboard_tab.h"
#include "../../application.h"
#include "../../defines.h"
#include "../../../singleton/models.h"
#include "../../../singleton/push.h"


using namespace modellbasen;

MessageBoardTab::MessageBoardTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app)
{
	Wt::WVBoxLayout* tab_container_vbox = new Wt::WVBoxLayout();
	tab_container_vbox->setContentsMargins(0, 9, 0, 0);
	setLayout(tab_container_vbox);

	//Add messageboard text submit widgets
	Wt::WContainerWidget* submit_container = new Wt::WContainerWidget();
	Wt::WHBoxLayout* submit_container_hbox = new Wt::WHBoxLayout();
	submit_container_hbox->setContentsMargins(0, 0, 0, 0);
	submit_container->setLayout(submit_container_hbox);
	
	m_submit_text = new Wt::WLineEdit();
	m_submit_text->enterPressed().connect(this, &MessageBoardTab::OnSubmitTextSubmit);
	submit_container_hbox->addWidget(m_submit_text, 1);
	m_submit_button = new Wt::WPushButton(Wt::WString::tr("Widget.SubmitButton"));
	m_submit_button->clicked().connect(this, &MessageBoardTab::OnSubmitButtonClicked);
	submit_container_hbox->addWidget(m_submit_button, 0);

	tab_container_vbox->addWidget(submit_container);

	//Add main messageboard view
	m_table_view = new Wt::WTableView();
	m_table_view->setModel(&g_messageboard_model);
	m_table_view->setAlternatingRowColors(true);
	m_table_view->setSortingEnabled(false);
	m_table_view->setDragEnabled(false);
	m_table_view->setDropsEnabled(false);
	m_table_view->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	m_table_view->setHidden(false);
	tab_container_vbox->addWidget(m_table_view);
}

MessageBoardTab::~MessageBoardTab()
{
}

void MessageBoardTab::OnSubmitTextSubmit()
{
	const std::string submit_text = m_submit_text->text().toUTF8();
	if (!submit_text.empty())
	{
		::PostMessageToBoard(m_app, submit_text);
	}
}

void MessageBoardTab::OnSubmitButtonClicked(const Wt::WMouseEvent& UNUSED(mouse))
{
	OnSubmitTextSubmit();
}

void MessageBoardTab::OnLoggedIn()
{
}

void MessageBoardTab::OnLoggedOut()
{
}
