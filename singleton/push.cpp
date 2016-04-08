#include "push.h"
#include "lock.h"
#include "models.h"
#include "../app/application.h"
#include "../singleton/db.h"
#include "../storage/usermanager.h"
#include "../storage/dbo/messageboard.h"
#include "../storage/dbo/user.h"
#include "../utils/time.h"

#include <Wt/WServer>
#include <Wt/WStandardItem>


std::vector<ClientConnection> g_client_connections;

ClientConnection::ClientConnection(const std::string& session_id, modellbasen::WebApplication* application, const boost::function<void()>& function)
	: m_session_id(session_id), m_application(application), m_function(function)
{}

void connect(modellbasen::WebApplication* application, const boost::function<void()>& function)
{
	boost::mutex::scoped_lock lock(g_global_mutex);
	g_client_connections.push_back(ClientConnection(Wt::WApplication::instance()->sessionId(), application, function));
}

void disconnect(modellbasen::WebApplication* application)
{
	boost::mutex::scoped_lock lock(g_global_mutex);

	std::size_t i;
	for (i=0; i<g_client_connections.size(); ++i) {
		if (g_client_connections[i].m_application == application) {
			g_client_connections.erase(g_client_connections.begin() + i);
			break;
		}
	}
}

bool PostMessageToBoard(const modellbasen::WebApplication* application, const std::string& message)
{
	boost::mutex::scoped_lock lock(g_global_mutex);

	const modellbasen::User* current_user = application->GetUserManager()->GetCurrentUser();
	if (!current_user)
		return false;

	Poco::Data::Session* session_in_transaction;
	if (!DB.CreateSession(session_in_transaction))
		return false;

	if (!modellbasen::MessageBoard::AddMessage(session_in_transaction, current_user->GetId(), message))
	{
		DB.ReleaseSession(session_in_transaction, modellbasen::PocoGlue::ROLLBACK);
		return false;
	}

	DB.ReleaseSession(session_in_transaction, modellbasen::PocoGlue::COMMIT);

	if (!g_messageboard_model.insertRows(0, 1))
		return false;

	std::string posted_time;
	modellbasen::Time::ToString(modellbasen::Time::NowUTC(), posted_time);
	g_messageboard_model.setItem(0, 0, new Wt::WStandardItem(Wt::WString::fromUTF8(posted_time)));
	g_messageboard_model.setItem(0, 1, new Wt::WStandardItem(Wt::WString::fromUTF8(current_user->GetUsername())));
	g_messageboard_model.setItem(0, 2, new Wt::WStandardItem(Wt::WString::fromUTF8(message)));
	if (MAX_MESSAGEBOARD_ROWS < g_messageboard_model.rowCount())
	{
		g_messageboard_model.removeRows(0, 1);
	}

	std::size_t i;
	for (i=0; i<g_client_connections.size(); ++i) {
		ClientConnection& application_connection = g_client_connections[i];
		Wt::WServer::instance()->post(application_connection.m_session_id, application_connection.m_function);
	}
	return true;
}
