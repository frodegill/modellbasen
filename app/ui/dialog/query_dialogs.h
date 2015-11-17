#ifndef _QUERY_DIALOGS_H_
#define _QUERY_DIALOGS_H_

#include <Wt/WDialog>
#include "../../application.h"
#include "../../../storage/dbo/tag.h"
#include "../../../storage/dbo/tagvalue.h"


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
	bool GetSelect(const Wt::WString& title, const Wt::WString& label, bool multiselect,
	               const Tag& tag, std::list<Poco::UInt32>& selected_values) const;
	bool GetInts(const Wt::WString& title, const Wt::WString& label1, const Wt::WString& label2, Poco::UInt32& value1, Poco::UInt32& value2) const;
	bool GetStringInt(const Wt::WString& title, const Wt::WString& label1, const Wt::WString& label2, std::string& value1, Poco::UInt32& value2) const;
	bool GetDistance(const Wt::WString& title, const Wt::WString& location_label, const Wt::WString& distance_label, std::string& postcode, Poco::UInt32& distance) const;

private:
	void AppendCommonDialogCode(Wt::WDialog& dialog, Wt::WGridLayout* layout, int row) const;

private:
	WebApplication* m_app;
};

} // namespace modellbasen

#endif // _QUERY_DIALOGS_H_
