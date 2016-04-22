#include "user.h"
#include "../../app/defines.h"
#include "../../singleton/db.h"


using namespace modellbasen;

User::User()
: m_id(INVALID_ID),
  m_profile_picture_id(INVALID_ID)
{
}

bool User::HasTag(const std::string& tag) const
{
	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	uint16_t tag_count = 0;
	DEBUG_TRY_CATCH(*session << "SELECT COUNT(*) FROM taginstance ti, tag t WHERE ti.owner=? AND ti.tag=t.id AND t.name=?",
		Poco::Data::Keywords::into(tag_count),
		Poco::Data::Keywords::useRef(m_id),
		Poco::Data::Keywords::useRef(tag),
		Poco::Data::Keywords::now;)

	DB.ReleaseSession(session, PocoGlue::IGNORE);

	return (0 < tag_count);
}
