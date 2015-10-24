#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "../storage/poco_glue.h"
#include <Wt/WApplication>


// Logging
extern Wt::WLogger g_logger;
Wt::WLogEntry Log(const std::string& type);

// Database
extern modellbasen::PocoGlue DB;

// Application
Wt::WApplication* createApplication(const Wt::WEnvironment& env);
int main(int argc, char** argv);

#endif // _GLOBAL_H_
