#include "push.h"
#include "lock.h"
#include "models.h"
#include "../app/application.h"
#include "../singleton/db.h"
#include "../storage/usermanager.h"
#include "../storage/dbo/messageboard.h"
#include "../storage/dbo/user.h"
#include "../utils/time.h"

#include <boost/locale.hpp>
#include <Wt/WServer>
#include <Wt/WStandardItem>


std::vector<ClientConnection> g_client_connections;

ClientConnection::ClientConnection(const std::string& session_id, modellbasen::WebApplication* application)
	: m_session_id(session_id), m_application(application), m_func_bflag(0)
{
}

void RegisterConnection(modellbasen::WebApplication* application)
{
	boost::mutex::scoped_lock lock(g_global_mutex);
	g_client_connections.push_back(ClientConnection(Wt::WApplication::instance()->sessionId(), application));
}

void UnregisterConnection(modellbasen::WebApplication* application)
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

void ConnectRefreshFunction(modellbasen::WebApplication* application, const boost::function<void()>& function)
{
	boost::mutex::scoped_lock lock(g_global_mutex);
	std::size_t i;
	for (i=0; i<g_client_connections.size(); ++i) {
		if (g_client_connections[i].m_application == application) {
			g_client_connections[i].m_refresh_function = function;
			g_client_connections[i].m_func_bflag |= POST_MESSAGE_TO_BOARD_FUNC;
			break;
		}
	}
}

void ConnectRefreshMessagecountFunction(modellbasen::WebApplication* application, const boost::function<void()>& function)
{
	boost::mutex::scoped_lock lock(g_global_mutex);
	std::size_t i;
	for (i=0; i<g_client_connections.size(); ++i) {
		if (g_client_connections[i].m_application == application) {
			g_client_connections[i].m_refresh_messagecount_function = function;
			g_client_connections[i].m_func_bflag |= NOTIFY_MESSAGE_TO_USER;
			break;
		}
	}
}

void DisconnectFunction(modellbasen::WebApplication* application, uint32_t function_type)
{
	boost::mutex::scoped_lock lock(g_global_mutex);
	std::size_t i;
	for (i=0; i<g_client_connections.size(); ++i) {
		if (g_client_connections[i].m_application == application) {
			g_client_connections[i].m_func_bflag &= ~function_type;
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
		if (0==(application_connection.m_func_bflag&POST_MESSAGE_TO_BOARD_FUNC))
			continue;

		Wt::WServer::instance()->post(application_connection.m_session_id, application_connection.m_refresh_function);
	}
	return true;
}

bool NotifyMessageToUser(const std::string& username)
{
	std::string username_lowercase = boost::locale::to_lower(username);

	boost::mutex::scoped_lock lock(g_global_mutex);

	const modellbasen::UserManager* usermanager;
	const modellbasen::User* user;
	std::size_t i;
	for (i=0; i<g_client_connections.size(); ++i) {
		ClientConnection& application_connection = g_client_connections[i];
		if (0==(application_connection.m_func_bflag&NOTIFY_MESSAGE_TO_USER))
			continue;

		if (!application_connection.m_application)
			continue;
		
		usermanager = application_connection.m_application->GetUserManager();
		if (!usermanager)
			continue;
		
		user = usermanager->GetCurrentUser();
		if (!user || username_lowercase!=boost::locale::to_lower(user->GetUsername()))
			continue;

		Wt::WServer::instance()->post(application_connection.m_session_id, application_connection.m_refresh_messagecount_function);
	}
	return true;
}
