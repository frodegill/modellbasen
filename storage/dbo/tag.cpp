#include "tag.h"


using namespace modellbasen;

Tag::Tag()
: m_id(0),
  m_insert_datatype(0),
  m_query_datatype(0),
  m_parent_id(0),
  m_query_only(true)
{
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
