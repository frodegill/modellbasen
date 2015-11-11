#ifdef USE_PCH
# include "../../pch.h"
#else
#endif

#include "search.h"
#include "../../app/global.h"


using namespace modellbasen;


bool Search::GetSearchedTagIds(std::list<Poco::UInt32>& result) const
{
	result.clear();
	for(std::list<std::shared_ptr<SearchInstance>>::const_iterator it = m_searchinstances.begin(); it!=m_searchinstances.end(); ++it)
	{
		std::shared_ptr<SearchInstance> search_instance = *it;
		result.push_front(search_instance->GetTagId());
	}
	return true;
}

bool Search::GetAvailableTags(std::list<Tag>& tags) const
{
	tags.clear();

	std::list<Poco::UInt32> searched_tag_ids;
	if (!GetSearchedTagIds(searched_tag_ids))
		return false;

	std::string parent_ids;
	for(std::list<Poco::UInt32>::const_iterator it = searched_tag_ids.begin(); it!=searched_tag_ids.end(); ++it)
	{
		if (!parent_ids.empty())
		{
			parent_ids += ",";
		}
		parent_ids += *it;
	}

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	if (parent_ids.empty())
	{
		*session << "SELECT id,name,insert_datatype,query_datatype,parent,query_only FROM tag WHERE parent IS NULL ORDER BY parent,id;",
			Poco::Data::Keywords::into(tags), Poco::Data::Keywords::now;
	}
	else
	{
		*session << "SELECT id,name,insert_datatype,query_datatype,parent,query_only FROM tag WHERE parent IS NULL OR parent IN (?) ORDER BY parent,id;",
			Poco::Data::Keywords::use(parent_ids), Poco::Data::Keywords::into(tags), Poco::Data::Keywords::now;
	}

	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return true;
}

bool Search::FindMatchingUsers(std::list<User>& result) const
{
	result.clear();

	return false;
}

bool Search::AddIntegerSearchInstance(Poco::UInt32 tag_id, //integer, location
                                      Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, Poco::UInt32 intvalue)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(tag_id, insert_datatype, query_datatype));
	search_instance->SetIntValue(intvalue);
	return AddSearchInstance(search_instance);
}

bool Search::AddStringSearchInstance(Poco::UInt32 tag_id, //string
                                     Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::string& stringvalue)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(tag_id, insert_datatype, query_datatype));
	search_instance->SetStringValue(stringvalue);
	return AddSearchInstance(search_instance);
}

bool Search::AddDatetimeSearchInstance(Poco::UInt32 tag_id, //datetime
                                       Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, Poco::UInt64 datetimevalue)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(tag_id, insert_datatype, query_datatype));
	search_instance->SetDatetimeValue(datetimevalue);
	return AddSearchInstance(search_instance);
}

bool Search::AddBooleanSearchInstance(Poco::UInt32 tag_id, //boolean (exists)
                                      Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(tag_id, insert_datatype, query_datatype));
	return AddSearchInstance(search_instance);
}

bool Search::AddStringListSearchInstance(Poco::UInt32 tag_id, //singleselect, multiselect
                                         Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::list<Poco::UInt32>& selectionvalues)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(tag_id, insert_datatype, query_datatype));
	search_instance->SetSelectionValues(selectionvalues);
	return AddSearchInstance(search_instance);
}

bool Search::AddIntegerIntegerSearchInstance(Poco::UInt32 tag_id, //height_range, day_range, age_range, distance
                                             Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, Poco::UInt32 intvalue, Poco::UInt32 intvalue2)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(tag_id, insert_datatype, query_datatype));
	search_instance->SetIntValues(intvalue, intvalue2);
	return AddSearchInstance(search_instance);
}

bool Search::AddStringIntegerSearchInstance(Poco::UInt32 tag_id, //height_range, day_range, age_range, distance
                                             Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::string& stringvalue, Poco::UInt32 intvalue)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(tag_id, insert_datatype, query_datatype));
	search_instance->SetStringValue(stringvalue);
	search_instance->SetIntValue(intvalue);
	return AddSearchInstance(search_instance);
}

bool Search::AddSearchInstance(std::shared_ptr<SearchInstance>& search_instance)
{
	m_searchinstances.push_front(search_instance);
	return false;
}
