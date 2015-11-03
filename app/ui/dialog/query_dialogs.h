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
	bool GetInt(const std::string& title, const std::string& label, Poco::UInt32& value) const;
	bool GetString(const std::string& title, const std::string& label, std::string& value) const;
	bool GetDatetime(const std::string& title, const std::string& label, Poco::UInt64& value) const;
	bool GetSelect(const std::string& title, const std::string& label, bool multiselect, std::list<std::string>& value) const;
	bool GetInts(const std::string& title, const std::string& label1, const std::string& label2, Poco::UInt32& value1, Poco::UInt32& value2) const;
	bool GetStringInt(const std::string& title, const std::string& label1, const std::string& label2, std::string& value1, Poco::UInt32& value2) const;

private:
	WebApplication* m_app;
};

} // namespace modellbasen

#endif // _QUERY_DIALOGS_H_
