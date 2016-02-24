#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "../storage/poco_glue.h"
#include <boost/thread/mutex.hpp>
#include <Wt/WApplication>
#include <Wt/WStandardItemModel>

#define MAX_MESSAGEBOARD_ROWS (25)


namespace modellbasen
{
class WebApplication;
};

extern boost::mutex g_global_mutex;

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


// Logging
extern Wt::WLogger g_logger;
Wt::WLogEntry Log(const std::string& type);

//Time
Poco::UInt64 GetTimeUTC();

// Database
extern modellbasen::PocoGlue DB;

// Global state
extern Wt::WStandardItemModel g_messageboard_model;
bool InitializePersistedGlobalResources();

// Application
Wt::WApplication* createApplication(const Wt::WEnvironment& env);
int main(int argc, char** argv);

#endif // _GLOBAL_H_
