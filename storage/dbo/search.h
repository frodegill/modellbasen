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
	Search(WebApplication* app);

	bool GetSearchedTagIds(std::list<IdType>& result) const;
	bool GetSearchedTags(std::list<std::shared_ptr<SearchInstance>>& instances) const {instances=m_searchinstances; return true;}
	bool GetAvailableTags(std::list<Tag>& tags) const;

	bool FindMatchingUsers(std::list<User>& result) const;

public:
	bool AddIntegerSearchInstance(IdType tag_id, //integer
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, Poco::UInt32 intvalue);

	bool AddStringSearchInstance(IdType tag_id, //string, location
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::string& stringvalue);

	bool AddDatetimeSearchInstance(IdType tag_id, //datetime
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, TimeType datetimevalue);

	bool AddBooleanSearchInstance(IdType tag_id, //boolean (exists)
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype);

	bool AddStringListSearchInstance(IdType tag_id, //singleselect, multiselect
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::list<IdType>& selectionvalues);

	bool AddIntegerIntegerSearchInstance(IdType tag_id, //height_range, day_range, age_range
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, Poco::UInt32 intvalue, Poco::UInt32 intvalue2);

	bool AddStringIntegerSearchInstance(IdType tag_id, //distance
	                              Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::string& stringvalue, Poco::UInt32 intvalue);

	bool InvertOrRemoveSearchInstance(IdType tag_id);

private:
	bool AddSearchInstance(std::shared_ptr<SearchInstance>& search_instance);

private:
	WebApplication* m_app;
	std::list<std::shared_ptr<SearchInstance>> m_searchinstances;
};

} // namespace modellbasen

#endif // _DBO_SEARCH_H_
