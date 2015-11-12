#ifdef USE_PCH
# include "../../pch.h"
#else
#endif

#include "tag.h"
#include "tagvalue.h"
#include "../../app/global.h"


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
