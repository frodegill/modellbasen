#ifndef _QUERY_DIALOGS_H_
#define _QUERY_DIALOGS_H_

#include "../../application.h"


namespace modellbasen
{

class QueryDialogs
{
public:
	QueryDialogs(WebApplication* app);

public:
	bool GetInt(Poco::UInt32& value) const;
	bool GetString(std::string& value) const;
	bool GetDatetime(Poco::UInt64& value) const;
	bool GetSingleSelect(std::list<std::string>& value) const;
	bool GetMultiSelect(std::list<std::string>& value) const;
	bool GetInts(Poco::UInt32& value1, Poco::UInt32& value2) const;

private:
	WebApplication* m_app;
};

} // namespace modellbasen

#endif // _QUERY_DIALOGS_H_
