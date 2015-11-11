#ifdef USE_PCH
# include "../../pch.h"
#else
#endif

#include "user.h"
#include "../../app/defines.h"
#include "../../app/global.h"


using namespace modellbasen;

User::User()
: m_id(0),
  m_profile_picture_id(0)
{
}

bool User::HasTag(const std::string& tag) const
{
	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	uint16_t tag_count = 0;
	*session << "SELECT COUNT(*) FROM taginstance ti, tag t WHERE ti.owner=? AND ti.tag=t.id AND t.name=?",
			Poco::Data::Keywords::into(tag_count,(const uint16_t)0), Poco::Data::Keywords::useRef(m_id), Poco::Data::Keywords::useRef(tag), Poco::Data::Keywords::now;

	DB.ReleaseSession(session, PocoGlue::IGNORE);

	return (0 < tag_count);
}
