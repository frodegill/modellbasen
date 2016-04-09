#include "tag.h"
#include "tagvalue.h"
#include "../usermanager.h"

#include "../../app/defines.h"


using namespace modellbasen;

Tag::Tag()
: m_id(0),
  m_insert_datatype(0),
  m_query_datatype(0),
  m_parent_id(0),
  m_query_only(true)
{
}

bool Tag::Initialize(Poco::UInt32 id)
{
	Reset();

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	*session << "SELECT id,name,insert_datatype,query_datatype,parent,query_only FROM tag WHERE id=?;",
		Poco::Data::Keywords::use(id), Poco::Data::Keywords::into(*this), Poco::Data::Keywords::now;

	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return true;
}

bool Tag::GetTagValues(std::list<TagValue>& tag_values) const
{
	tag_values.clear();

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	*session << "SELECT id,value,pos,tag FROM tagvalue WHERE tag=? ORDER BY pos;",
		Poco::Data::Keywords::useRef(m_id), Poco::Data::Keywords::into(tag_values), Poco::Data::Keywords::now;

	DB.ReleaseSession(session, PocoGlue::IGNORE);

	return true;
}

bool Tag::GetTagText(Wt::WLocalizedStrings* localized_strings, bool is_query, std::string& text) const
{
	std::string key = GetName();
	if (is_query)
	{
		if (localized_strings->resolveKey(key+".query", text))
			return true;
	}

	return localized_strings->resolveKey(key, text);
}

bool Tag::GetId(const std::string& tagname, Poco::UInt32& id)
{
	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	bool ret = GetId(session, tagname, id);
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return ret;
}

bool Tag::GetId(Poco::Data::Session* session, const std::string& tagname, Poco::UInt32& id)
{
	if (!session)
		return false;

	IF_NO_ROWS(stmt, *session,
	           "SELECT id FROM tag WHERE name=?;",
		           Poco::Data::Keywords::useRef(tagname), Poco::Data::Keywords::into(id))
	{
		id = INVALID_ID;
	}

	return true;
}

bool Tag::SetUserTag(Poco::Data::Session* session_in_transaction,
										 const std::string& username, const std::string& tag_name, const std::string& string_value, int int_value, Poco::UInt64 time_value)
{
	Poco::UInt32 user_id;
	if (!UserManager::GetUserId(session_in_transaction, username, user_id) || INVALID_ID==user_id)
		return false;

	return SetTag(session_in_transaction, user_id, INVALID_ID, tag_name, string_value, int_value, time_value);
}

bool Tag::SetEventTag(Poco::Data::Session* session_in_transaction,
											Poco::UInt32 event_id, Poco::UInt32 participant_id, const std::string& tag_name, const std::string& string_value, int int_value, Poco::UInt64 time_value)
{
	if (!session_in_transaction || INVALID_ID==event_id || INVALID_ID==participant_id)
		return false;

	Poco::UInt32 event_participant_id;

	IF_NO_ROWS(stmt, *session_in_transaction,
	           "SELECT id FROM eventparticipant WHERE event=? AND participant=?;",
		           Poco::Data::Keywords::use(event_id), Poco::Data::Keywords::use(participant_id),
		           Poco::Data::Keywords::into(event_participant_id))
	{
		event_participant_id = INVALID_ID;
	}

	if (INVALID_ID == event_participant_id)
		return false;

	return SetTag(session_in_transaction, INVALID_ID, event_participant_id, tag_name, string_value, int_value, time_value);
}

bool Tag::SetTag(Poco::Data::Session* session_in_transaction,
								 Poco::UInt32 user_id, Poco::UInt32 event_participant_id,
								 const std::string& tag_name,
								 const std::string& string_value, int int_value, Poco::UInt64 time_value)
{
	if (!session_in_transaction ||
		  (INVALID_ID==user_id && INVALID_ID==event_participant_id) || //neither user nor event
	    (INVALID_ID!=user_id && INVALID_ID!=event_participant_id)) //both user and event
		return false;

	Poco::UInt32 tag_id;
	if (!GetId(session_in_transaction, tag_name, tag_id) || INVALID_ID==tag_id)
		return false;

	//Check if tag already exists
	Poco::UInt32 taginstance_id;
	if (INVALID_ID!=user_id)
	{
		IF_NO_ROWS(stmt, *session_in_transaction,
		           "SELECT id FROM taginstance WHERE tag=? AND owner=?;",
			           Poco::Data::Keywords::use(tag_id), Poco::Data::Keywords::use(user_id),
			           Poco::Data::Keywords::into(taginstance_id))
		{
			taginstance_id = INVALID_ID;
		}
	}
	else
	{
		IF_NO_ROWS(stmt, *session_in_transaction,
			"SELECT id FROM taginstance WHERE tag=? AND eventparticipant=?;",
				Poco::Data::Keywords::use(tag_id), Poco::Data::Keywords::use(event_participant_id),
				Poco::Data::Keywords::into(taginstance_id))
		{
			taginstance_id = INVALID_ID;
		}
	}

	if (INVALID_ID!=taginstance_id) //Exists. Update
	{
		*session_in_transaction << "UPDATE taginstance SET stringvalue=?, intvalue=?, timevalue=? WHERE id=?",
				Poco::Data::Keywords::useRef(string_value),
				Poco::Data::Keywords::use(int_value),
				Poco::Data::Keywords::use(time_value),
				Poco::Data::Keywords::now;
	}
	else //Does not exist. Create
	{
		Poco::Nullable<Poco::UInt32> user_id_value;
		if (INVALID_ID != user_id)
			user_id_value.assign(user_id);

		Poco::Nullable<Poco::UInt32> event_participant_id_value;
		if (INVALID_ID != event_participant_id)
			event_participant_id_value.assign(event_participant_id);

		DEBUG_TRY_CATCH(*session_in_transaction << "INSERT INTO taginstance (stringvalue, intvalue, timevalue, tag, owner, eventparticipant) "
		                                           "VALUE (?, ?, ?, ?, ?, ?)",
			Poco::Data::Keywords::useRef(string_value), Poco::Data::Keywords::use(int_value), Poco::Data::Keywords::use(time_value),
			Poco::Data::Keywords::use(tag_id), Poco::Data::Keywords::use(user_id_value), Poco::Data::Keywords::use(event_participant_id_value),
			Poco::Data::Keywords::now;)
	}

	return true;
}
