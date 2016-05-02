#include "tagvalue.h"

#include "../../app/defines.h"
#include "../../singleton/db.h"


using namespace modellbasen;

TagValue::TagValue()
: m_id(INVALID_ID),
  m_pos(0),
  m_tag_id(INVALID_ID)
{
}

bool TagValue::Initialize(IdType id)
{
	Reset();

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	IF_NO_ROWS(stmt, *session,
		"SELECT id, value, pos, tag FROM tagvalue WHERE id=?;",
			Poco::Data::Keywords::use(id), Poco::Data::Keywords::into(*this))
	{
		m_id = INVALID_ID;
	}

	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return true;
}

bool TagValue::GetTagValueText(Wt::WLocalizedStrings* localized_strings, std::string& text) const
{
	std::string key = GetValue();
	return localized_strings->resolveKey(key, text);
}
