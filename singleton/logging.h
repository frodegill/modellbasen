#ifndef _SINGLETON_LOGGING_H_
#define _SINGLETON_LOGGING_H_

#include <Wt/WLogger>


extern Wt::WLogger g_logger;
Wt::WLogEntry Log(const std::string& type);

#endif // _SINGLETON_LOGGING_H_
