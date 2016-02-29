#ifndef _TIME_H_
#define _TIME_H_

#include "../storage/poco_glue.h"


namespace modellbasen
{

class Time
{
public:
	static Poco::UInt64 NowUTC();
	static void ToString(const Poco::UInt64& time, std::string& as_string);
};

} // namespace modellbasen

#endif // _TIME_H_
