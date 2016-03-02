#include "time.h"

#include <Wt/WDateTime>
#include <Wt/WLocalDateTime>
#include "../app/defines.h"


using namespace modellbasen;

Poco::UInt64 Time::NowUTC()
{
	return Wt::WDateTime::currentDateTime().toTime_t();
}

void Time::ToString(const Poco::UInt64& time, std::string& as_string)
{
	Wt::WDateTime datetime;
	datetime.setTime_t(time);
	Wt::WLocalDateTime local_datetime = datetime.toLocalTime(Wt::WLocale(SERVER_LOCALE));
	as_string = local_datetime.toString(Wt::WString::tr("DateFormat")).toUTF8();
}
