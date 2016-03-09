#ifndef _SINGLETON_PUSH_H_
#define _SINGLETON_PUSH_H_

#include <boost/function.hpp>
#include <string>
#include <vector>


namespace modellbasen
{
class WebApplication;
};

struct ClientConnection {
	ClientConnection(const std::string& session_id, modellbasen::WebApplication* application, const boost::function<void()>& function);
	std::string m_session_id;
	modellbasen::WebApplication* m_application;
	boost::function<void()> m_function;
};
extern std::vector<ClientConnection> g_client_connections;

void connect(modellbasen::WebApplication* application, const boost::function<void()>& function);
void disconnect(modellbasen::WebApplication* application);
bool PostMessageToBoard(const modellbasen::WebApplication* application, const std::string& message);

#endif // _SINGLETON_PUSH_H_
