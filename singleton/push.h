#ifndef _SINGLETON_PUSH_H_
#define _SINGLETON_PUSH_H_

#include <boost/function.hpp>
#include <string>
#include <vector>


namespace modellbasen
{
class WebApplication;
};

#define POST_MESSAGE_TO_BOARD_FUNC (1<<0)
#define NOTIFY_MESSAGE_TO_USER     (1<<1)

struct ClientConnection {
	ClientConnection(const std::string& session_id, modellbasen::WebApplication* application);
	std::string m_session_id;
	modellbasen::WebApplication* m_application;
	boost::function<void()> m_refresh_function;
	boost::function<void()> m_refresh_messagecount_function;
	uint32_t m_func_bflag;
};
extern std::vector<ClientConnection> g_client_connections;

void RegisterConnection(modellbasen::WebApplication* application);
void UnregisterConnection(modellbasen::WebApplication* application);
void ConnectRefreshFunction(modellbasen::WebApplication* application, const boost::function<void()>& function);
void ConnectRefreshMessagecountFunction(modellbasen::WebApplication* application, const boost::function<void()>& function);
void DisconnectFunction(modellbasen::WebApplication* application, uint32_t function_type);

bool PostMessageToBoard(const modellbasen::WebApplication* application, const std::string& message);
bool NotifyMessageToUser(const std::string& username);

#endif // _SINGLETON_PUSH_H_
