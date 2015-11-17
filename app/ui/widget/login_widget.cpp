#include <Wt/WGridLayout>

#include "login_widget.h"
#include "../../application.h"
#include "../../defines.h"
#include "../../../storage/usermanager.h"


using namespace modellbasen;

LoginWidget::LoginWidget(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app),
  m_initialized(false),
  m_username_edit(NULL),
  m_password_edit(NULL),
  m_login_button(NULL),
  m_login_feedback_text(NULL)
{
}

LoginWidget::~LoginWidget()
{
}

void LoginWidget::Initialize()
{
	if (m_initialized)
		return;

	Wt::WGridLayout* login_grid_layout = new Wt::WGridLayout();
	login_grid_layout->setContentsMargins(50, 50, 50, 50);

	//Create username widget
	login_grid_layout->addWidget(new Wt::WText(Wt::WString::tr("Username")), 0, 0, Wt::AlignRight);
	if (!m_username_edit)
	{
		m_username_edit = new Wt::WLineEdit();
		m_username_edit->setObjectName("username");
		m_username_edit->setAutoComplete(true);
		m_username_edit->setTextSize(20);
		m_username_edit->setWidth(Wt::WLength(20, Wt::WLength::FontEm));
		m_username_edit->enterPressed().connect(this, &LoginWidget::UsernameEnterPressed);
	}
	login_grid_layout->addWidget(m_username_edit, 0, 1, Wt::AlignLeft);

	//Create password widget
	login_grid_layout->addWidget(new Wt::WText(Wt::WString::tr("Password")), 1, 0, Wt::AlignRight);
	if (!m_password_edit)
	{
		m_password_edit = new Wt::WLineEdit();
		m_username_edit->setObjectName("password");
		m_username_edit->setAutoComplete(true);
		m_password_edit->setTextSize(20);
		m_password_edit->setWidth(Wt::WLength(20, Wt::WLength::FontEm));
		m_password_edit->setEchoMode(Wt::WLineEdit::Password);
		m_password_edit->enterPressed().connect(this, &LoginWidget::PasswordEnterPressed);
	}
	m_password_edit->setText("");
	login_grid_layout->addWidget(m_password_edit, 1, 1, Wt::AlignLeft);

	//Create login button
	if (!m_login_button)
	{
		m_login_button = new Wt::WPushButton(Wt::WString::tr("LoginButton"));
		m_login_button->clicked().connect(this, &LoginWidget::OnLoginButtonClicked);
	}
	login_grid_layout->addWidget(m_login_button, 2, 1, Wt::AlignLeft);

	//Create login feedback text
	if (!m_login_feedback_text)
	{
		m_login_feedback_text = new Wt::WText();
		m_login_feedback_text->setWidth(Wt::WLength(20, Wt::WLength::FontEm));
	}
	m_login_feedback_text->hide();
	login_grid_layout->addWidget(m_login_feedback_text, 3, 1, Wt::AlignLeft);

	//Build layout
	setLayout(login_grid_layout);

	m_initialized = true;
}

void LoginWidget::ActivateLoginWidget()
{
	if (!m_initialized)
		Initialize();

	while (0 < m_app->root()->count())
	{
		Wt::WWidget* widget = m_app->root()->widget(0);
		m_app->root()->removeWidget(widget);
	}

	m_app->root()->addWidget(this);

	//Set focus
#ifdef DEBUG
	m_username_edit->setText("admin");
	m_password_edit->setText("admin");
#else
	m_username_edit->setText("");
	m_password_edit->setText("");
#endif
	m_username_edit->setFocus();
}

void LoginWidget::UsernameEnterPressed()
{
	if (m_username_edit->text().empty())
	{
		return;
	}
	else if (m_password_edit->displayText().empty())
	{
		m_password_edit->setFocus();
	}
	else
	{
		RequestLogin();
	}
}

void LoginWidget::PasswordEnterPressed()
{
	if (m_username_edit->text().empty())
	{
		m_username_edit->setFocus();
	}
	else if (m_password_edit->displayText().empty())
	{
		return;
	}
	else
	{
		RequestLogin();
	}
}

void LoginWidget::OnLoginButtonClicked(const Wt::WMouseEvent& UNUSED(mouse))
{
	if (m_username_edit->text().empty())
	{
		m_username_edit->setFocus();
	}
	else if (m_password_edit->displayText().empty())
	{
		m_password_edit->setFocus();
	}
	else
	{
		RequestLogin();
	}
}

void LoginWidget::RequestLogin()
{
	std::string username = m_username_edit->text().toUTF8();
	std::string password = m_password_edit->text().toUTF8();

	m_app->GetUserManager()->LogIn(username, password);
	if (!m_app->GetUserManager()->GetCurrentUser())
	{
		LoginFailed();
	}
	else
	{
		m_login_feedback_text->hide();
		m_app->ActivateMainWidget();
	}
}

void LoginWidget::LoginFailed()
{
	m_login_feedback_text->setText(Wt::WString::tr("InvalidUsernameOrPassword"));
	m_login_feedback_text->show();
	m_username_edit->setFocus();
}
