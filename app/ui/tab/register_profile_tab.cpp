#include <Wt/WBreak>
#include <Wt/WGridLayout>
#include <Wt/WGroupBox>
#include <Wt/WText>

#include "register_profile_tab.h"
#include "../../application.h"
#include "../../../singleton/db.h"
#include "../../../storage/usermanager.h"
#include "../../../storage/dbo/postcode.h"
#include "../../../storage/dbo/tag.h"


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

	Wt::WGroupBox* profile_type_box = new Wt::WGroupBox(Wt::WString::tr("ProfileType"));
	m_model_checkbox = new Wt::WCheckBox(Wt::WString::tr(TAG_MODEL), profile_type_box);
	profile_type_box->addWidget(new Wt::WBreak());
	m_photographer_checkbox = new Wt::WCheckBox(Wt::WString::tr(TAG_PHOTOGRAPHER), profile_type_box);
	profile_type_box->addWidget(new Wt::WBreak());
	m_mua_checkbox = new Wt::WCheckBox(Wt::WString::tr(TAG_MUA), profile_type_box);
	profile_type_box->addWidget(new Wt::WBreak());
	m_hairdresser_checkbox = new Wt::WCheckBox(Wt::WString::tr(TAG_HAIRDRESSER), profile_type_box);
	profile_type_box->addWidget(new Wt::WBreak());
	m_agency_checkbox = new Wt::WCheckBox(Wt::WString::tr(TAG_AGENCY), profile_type_box);
	profile_type_box->addWidget(new Wt::WBreak());
	profile_grid_layout->addWidget(profile_type_box, row++, 0, 1, 2, Wt::AlignCenter);

	m_register_button = new Wt::WPushButton(Wt::WString::tr("RegisterButton"));
	m_register_button->clicked().connect(this, &RegisterProfileTab::OnRegisterButtonClicked);
	profile_grid_layout->addWidget(m_register_button, row++, 0, 1, 2, Wt::AlignCenter);

	m_feedback_text = new Wt::WText();
	m_feedback_text->setWidth(Wt::WLength(20, Wt::WLength::FontEm));
	m_feedback_text->setStyleClass("feedback");
	m_feedback_text->hide();
	profile_grid_layout->addWidget(m_feedback_text, row++, 0, 1, 2, Wt::AlignCenter);
}

RegisterProfileTab::~RegisterProfileTab()
{
}

void RegisterProfileTab::OnRegisterButtonClicked()
{
	Wt::WFormWidget* widget = GetFirstIncompleteFormElement();
	if (!widget)
	{
		if (m_model_checkbox->hasFocus()) widget=m_photographer_checkbox;
		else if (m_photographer_checkbox->hasFocus()) widget=m_mua_checkbox;
		else if (m_mua_checkbox->hasFocus()) widget=m_hairdresser_checkbox;
		else if (m_hairdresser_checkbox->hasFocus()) widget=m_agency_checkbox;
		else if (m_agency_checkbox->hasFocus()) widget=m_register_button;
	}
	
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

	Poco::UInt32 type_bflag = 0;
	if(m_model_checkbox->isChecked())        type_bflag|=MODEL_BFLAG;
	if(m_photographer_checkbox->isChecked()) type_bflag|=PHOTOGRAPHER_BFLAG;
	if(m_mua_checkbox->isChecked())          type_bflag|=MUA_BFLAG;
	if(m_hairdresser_checkbox->isChecked())  type_bflag|=HAIRDRESSER_BFLAG;
	if(m_agency_checkbox->isChecked())       type_bflag|=AGENCY_BFLAG;

	if (!UserManager::RegisterUser(session_in_transaction, username, password,
	                               m_email_edit->text().trim().toUTF8(), m_postcode_edit->text().trim().toUTF8(), type_bflag))
	{
		DB.ReleaseSession(session_in_transaction, PocoGlue::ROLLBACK);
		return;
	}
	DB.ReleaseSession(session_in_transaction, PocoGlue::COMMIT);

	m_app->GetUserManager()->LogIn(username, password);
}

void RegisterProfileTab::OnActivateTab()
{
	m_username_edit->setFocus();
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
