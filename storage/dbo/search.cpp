#include "search.h"

#include "../../singleton/db.h"


using namespace modellbasen;


Search::Search(WebApplication* app)
: m_app(app)
{
}

bool Search::GetSearchedTagIds(std::list<IdType>& result) const
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

	std::list<IdType> searched_tag_ids;
	if (!GetSearchedTagIds(searched_tag_ids))
		return false;

	std::string parent_ids;
	for(std::list<IdType>::const_iterator it = searched_tag_ids.begin(); it!=searched_tag_ids.end(); ++it)
	{
		if (!parent_ids.empty())
		{
			parent_ids += ",";
		}
		parent_ids += std::to_string(*it);
	}

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	if (parent_ids.empty())
	{
		DEBUG_TRY_CATCH(*session << "SELECT id,name,insert_datatype,query_datatype,parent,query_only FROM tag WHERE parent IS NULL ORDER BY parent,id;",
			Poco::Data::Keywords::into(tags), Poco::Data::Keywords::now;)
	}
	else
	{
		DEBUG_TRY_CATCH(*session << "SELECT id,name,insert_datatype,query_datatype,parent,query_only FROM tag WHERE id NOT IN ("+parent_ids+") AND (parent IS NULL OR parent IN ("+parent_ids+")) ORDER BY parent,id;",
			Poco::Data::Keywords::into(tags), Poco::Data::Keywords::now;)
	}

	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return true;
}

bool Search::FindMatchingUsers(std::list<User>& result) const
{
	result.clear();

	return false;
}

bool Search::AddIntegerSearchInstance(IdType tag_id, //integer, location
                                      Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, Poco::UInt32 intvalue)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(m_app, tag_id, insert_datatype, query_datatype));
	search_instance->SetIntValue(intvalue);
	return AddSearchInstance(search_instance);
}

bool Search::AddStringSearchInstance(IdType tag_id, //string
                                     Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::string& stringvalue)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(m_app, tag_id, insert_datatype, query_datatype));
	search_instance->SetStringValue(stringvalue);
	return AddSearchInstance(search_instance);
}

bool Search::AddDatetimeSearchInstance(IdType tag_id, //datetime
                                       Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, TimeType datetimevalue)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(m_app, tag_id, insert_datatype, query_datatype));
	search_instance->SetDatetimeValue(datetimevalue);
	return AddSearchInstance(search_instance);
}

bool Search::AddBooleanSearchInstance(IdType tag_id, //boolean (exists)
                                      Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(m_app, tag_id, insert_datatype, query_datatype));
	return AddSearchInstance(search_instance);
}

bool Search::AddStringListSearchInstance(IdType tag_id, //singleselect, multiselect
                                         Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::list<IdType>& selectionvalues)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(m_app, tag_id, insert_datatype, query_datatype));
	search_instance->SetSelectionValues(selectionvalues);
	return AddSearchInstance(search_instance);
}

bool Search::AddIntegerIntegerSearchInstance(IdType tag_id, //height_range, day_range, age_range, distance
                                             Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, Poco::UInt32 intvalue, Poco::UInt32 intvalue2)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(m_app, tag_id, insert_datatype, query_datatype));
	search_instance->SetIntValues(intvalue, intvalue2);
	return AddSearchInstance(search_instance);
}

bool Search::AddStringIntegerSearchInstance(IdType tag_id, //height_range, day_range, age_range, distance
                                             Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype, const std::string& stringvalue, Poco::UInt32 intvalue)
{
	std::shared_ptr<SearchInstance> search_instance(new SearchInstance(m_app, tag_id, insert_datatype, query_datatype));
	search_instance->SetStringValue(stringvalue);
	search_instance->SetIntValue(intvalue);
	return AddSearchInstance(search_instance);
}

bool Search::InvertOrRemoveSearchInstance(IdType tag_id)
{
	for(std::list<std::shared_ptr<SearchInstance>>::const_iterator it = m_searchinstances.begin(); it!=m_searchinstances.end(); ++it)
	{
		std::shared_ptr<SearchInstance> search_instance = *it;
		if (search_instance->GetTagId() != tag_id)
			continue;

		if (!search_instance->IsInverted())
		{
			search_instance->Invert();
			return true;
		}
		else
		{
			m_searchinstances.erase(it);
			return true;
		}
	}
	return false;
}

bool Search::AddSearchInstance(std::shared_ptr<SearchInstance>& search_instance)
{
	m_searchinstances.push_back(search_instance);
	return true;
}
