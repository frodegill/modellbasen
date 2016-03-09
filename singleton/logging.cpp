#include "logging.h"


Wt::WLogger g_logger;
Wt::WLogEntry Log(const std::string& type)
{
	return g_logger.entry(type);
}
