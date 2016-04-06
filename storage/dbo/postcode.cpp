#include "postcode.h"

#include "../../singleton/db.h"


using namespace modellbasen;

PostCode::PostCode()
: m_latitude(0.0),
  m_longitude(0.0)
{
}

bool PostCode::Exists(const std::string& postcode, bool& exists)
{
	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	int exist_count = 0;
	*session << "SELECT COUNT(*) FROM postcode WHERE postcode=?",
		Poco::Data::Keywords::into(exist_count),
		Poco::Data::Keywords::useRef(postcode),
		Poco::Data::Keywords::now;
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);

	exists = 0<exist_count;
	return true;
}
