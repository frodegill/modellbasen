#include <Wt/WGridLayout>
#include <Wt/WPushButton>
#include <Wt/WText>

#include "register_profile_tab.h"
#include "../../application.h"
#include "../../../singleton/db.h"
#include "../../../storage/usermanager.h"
#include "../../../storage/dbo/postcode.h"


using namespace modellbasen;

RegisterProfileTab::RegisterProfileTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app)
{
	Wt::WGridLayout* profile_grid_layout = new Wt::WGridLayout();
	profile_grid_layout->setContentsMargins(0, 0, 0, 0);
	setLayout(profile_grid_layout);

	int row = 0;

	//Create username widget
	profile_grid_layout->addWidget(new Wt::WText(Wt::WString::tr("Username")), row, 0, Wt::AlignRight);
	m_username_edit = new Wt::WLineEdit();
	m_username_edit->enterPressed().connect(this, &RegisterProfileTab::OnRegisterButtonClicked);
	m_username_edit->setText("");
	profile_grid_layout->addWidget(m_username_edit, row++, 1, Wt::AlignLeft);

	profile_grid_layout->addWidget(new Wt::WText(Wt::WString::tr("Password")), row, 0, Wt::AlignRight);
	m_password_edit = new Wt::WLineEdit();
	m_password_edit->setEchoMode(Wt::WLineEdit::Password);
	m_password_edit->enterPressed().connect(this, &RegisterProfileTab::OnRegisterButtonClicked);
	m_password_edit->setText("");
	profile_grid_layout->addWidget(m_password_edit, row++, 1, Wt::AlignLeft);

	profile_grid_layout->addWidget(new Wt::WText(Wt::WString::tr("Email")), row, 0, Wt::AlignRight);
	m_email_edit = new Wt::WLineEdit();
	m_email_edit->enterPressed().connect(this, &RegisterProfileTab::OnRegisterButtonClicked);
	m_email_edit->setText("");
	profile_grid_layout->addWidget(m_email_edit, row++, 1, Wt::AlignLeft);

	profile_grid_layout->addWidget(new Wt::WText(Wt::WString::tr("Postcode")), row, 0, Wt::AlignRight);
	m_postcode_edit = new Wt::WLineEdit();
	m_postcode_edit->enterPressed().connect(this, &RegisterProfileTab::OnRegisterButtonClicked);
	m_postcode_edit->setText("");
	profile_grid_layout->addWidget(m_postcode_edit, row++, 1, Wt::AlignLeft);

	Wt::WPushButton* register_button = new Wt::WPushButton(Wt::WString::tr("RegisterButton"));
	register_button->clicked().connect(this, &RegisterProfileTab::OnRegisterButtonClicked);
	profile_grid_layout->addWidget(register_button, row++, 0, 1, 2, Wt::AlignCenter);

	m_feedback_text = new Wt::WText();
	m_feedback_text->setWidth(Wt::WLength(20, Wt::WLength::FontEm));
	m_feedback_text->setStyleClass("feedback");
	m_feedback_text->hide();
	profile_grid_layout->addWidget(m_feedback_text, row++, 0, 1, 2, Wt::AlignLeft);
}

RegisterProfileTab::~RegisterProfileTab()
{
}

void RegisterProfileTab::OnRegisterButtonClicked()
{
	Wt::WFormWidget* widget = GetFirstIncompleteFormElement();
	if (widget)
	{
		widget->setFocus();
		return;
	}

	Poco::Data::Session* session_in_transaction;
	if (!DB.CreateSession(session_in_transaction))
		return;

	std::string username = m_username_edit->text().trim().toUTF8();
	std::string password = m_password_edit->text().toUTF8();
	if (!UserManager::RegisterUser(session_in_transaction, username, password,
	                               m_email_edit->text().trim().toUTF8(), m_postcode_edit->text().trim().toUTF8()))
	{
		DB.ReleaseSession(session_in_transaction, PocoGlue::ROLLBACK);
		return;
	}

	m_app->GetUserManager()->LogIn(username, password);
	DB.ReleaseSession(session_in_transaction, PocoGlue::COMMIT);
}

Wt::WFormWidget* RegisterProfileTab::GetFirstIncompleteFormElement() const
{
	m_feedback_text->hide();

	const std::string username = m_username_edit->text().trim().toUTF8();
	if (username.empty())
	{
		return m_username_edit;
	}

	bool exists;
	if (!UserManager::Exists(username, exists))
	{
		m_feedback_text->setText(Wt::WString::tr("UserExistsFailed"));
		m_feedback_text->show();
		return m_username_edit;
	}
	if (exists)
	{
		m_feedback_text->setText(Wt::WString::tr("UserExists"));
		m_feedback_text->show();
		return m_username_edit;
	}

	const std::string password = m_password_edit->displayText().toUTF8();
	if (password.empty())
	{
		return m_password_edit;
	}
	
	const std::string email = m_email_edit->text().trim().toUTF8();
	if (email.empty())
	{
		return m_email_edit;
	}

	const std::string postcode = m_postcode_edit->text().trim().toUTF8();
	if (postcode.empty())
	{
		return m_postcode_edit;
	}

	if (!PostCode::Exists(postcode, exists))
	{
		m_feedback_text->setText(Wt::WString::tr("PostcodeExistsFailed"));
		m_feedback_text->show();
		return m_postcode_edit;
	}
	if (!exists)
	{
		m_feedback_text->setText(Wt::WString::tr("UnknownPostcode"));
		m_feedback_text->show();
		return m_postcode_edit;
	}
	
	return nullptr;
}
