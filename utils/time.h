#ifndef _TIME_H_
#define _TIME_H_

#include "../storage/poco_glue.h"
#include "../app/defines.h"


namespace modellbasen
{

class Time
{
public:
	static TimeType NowUTC();
	static void ToString(const TimeType& time, std::string& as_string);
};

} // namespace modellbasen

#endif // _TIME_H_
