#ifndef _DBO_SEARCH_H_
#define _DBO_SEARCH_H_

/* This is not a persisted object */

//#include "classes.h"
//#include "../poco_glue.h"
//#include <string>
//#include <Poco/Timestamp.h>
#include <memory>
#include "searchinstance.h"
#include "user.h"


namespace modellbasen
{

class Search {
public:
	bool GetSearchedTagIds(std::list<Poco::UInt32>& result) const;
	bool GetAvailableTags(std::list<Tag>& tags) const;

	bool FindMatchingUsers(std::list<User>& result) const;

public:
	bool AddIntegerSearchInstance(Poco::UInt32 tag_id, //integer
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, Poco::UInt32 intvalue);

	bool AddStringSearchInstance(Poco::UInt32 tag_id, //string, location
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::string& stringvalue);

	bool AddDatetimeSearchInstance(Poco::UInt32 tag_id, //datetime
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, Poco::UInt64 datetimevalue);

	bool AddBooleanSearchInstance(Poco::UInt32 tag_id, //boolean (exists)
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype);

	bool AddStringListSearchInstance(Poco::UInt32 tag_id, //singleselect, multiselect
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::list<std::string>& selectionvalues);

	bool AddIntegerIntegerSearchInstance(Poco::UInt32 tag_id, //height_range, day_range, age_range
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, Poco::UInt32 intvalue, Poco::UInt32 intvalue2);

	bool AddStringIntegerSearchInstance(Poco::UInt32 tag_id, //distance
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::string& stringvalue, Poco::UInt32 intvalue);

private:
	bool AddSearchInstance(std::shared_ptr<SearchInstance>& search_instance);

private:
	std::list<std::shared_ptr<SearchInstance>> m_searchinstances;
};

} // namespace modellbasen

#endif // _DBO_SEARCH_H_
