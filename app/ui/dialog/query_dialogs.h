#ifndef _QUERY_DIALOGS_H_
#define _QUERY_DIALOGS_H_

#include <Wt/WDialog>
#include "../../application.h"


namespace modellbasen
{

class QueryDialogs
{
public:
	QueryDialogs(WebApplication* app);

public:
	bool GetInt(const Wt::WString& title, const Wt::WString& label, Poco::UInt32& value) const;
	bool GetString(const Wt::WString& title, const Wt::WString& label, std::string& value) const;
	bool GetDatetime(const Wt::WString& title, const Wt::WString& label, Poco::UInt64& value) const;
	bool GetSelect(const Wt::WString& title, const Wt::WString& label, bool multiselect, std::list<std::string>& value) const;
	bool GetInts(const Wt::WString& title, const Wt::WString& label1, const Wt::WString& label2, Poco::UInt32& value1, Poco::UInt32& value2) const;
	bool GetStringInt(const Wt::WString& title, const Wt::WString& label1, const Wt::WString& label2, std::string& value1, Poco::UInt32& value2) const;

private:
	void AppendCommonDialogCode(Wt::WDialog& dialog, Wt::WGridLayout* layout, int row) const;

private:
	WebApplication* m_app;
};

} // namespace modellbasen

#endif // _QUERY_DIALOGS_H_
