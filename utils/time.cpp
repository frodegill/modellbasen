#include "time.h"

#include <Wt/WDateTime>


using namespace modellbasen;

Poco::UInt64 Time::NowUTC()
{
	return Wt::WDateTime::currentDateTime().toTime_t();
}

void Time::ToString(const Poco::UInt64& time, std::string& as_string)
{
	Wt::WDateTime datetime;
	datetime.setTime_t(time);
	as_string = datetime.toString(Wt::WString::tr("DateFormat")).toUTF8();
}
