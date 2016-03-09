#include "tagvalue.h"
#include "../../singleton/db.h"


using namespace modellbasen;

TagValue::TagValue()
: m_id(0),
  m_pos(0),
  m_tag_id(0)
{
}

bool TagValue::Initialize(Poco::UInt32 id)
{
	Reset();

	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	*session << "SELECT id, value, pos, tag FROM tagvalue WHERE id=?;",
		Poco::Data::Keywords::use(id), Poco::Data::Keywords::into(*this), Poco::Data::Keywords::now;

	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return true;
}

bool TagValue::GetTagValueText(Wt::WLocalizedStrings* localized_strings, std::string& text) const
{
	std::string key = GetValue();
	return localized_strings->resolveKey(key, text);
}
