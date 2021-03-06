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

	bool ret = Exists(session, postcode, exists);
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return ret;
}

bool PostCode::Exists(Poco::Data::Session* session, const std::string& postcode, bool& exists)
{
	if (!session)
		return false;

	int exist_count = 0;
	DEBUG_TRY_CATCH(*session << "SELECT COUNT(*) FROM postcode WHERE postcode=?",
		Poco::Data::Keywords::into(exist_count),
		Poco::Data::Keywords::useRef(postcode),
		Poco::Data::Keywords::now;)

	exists = 0<exist_count;
	return true;
}

bool PostCode::GetPlace(const std::string& postcode, std::string& place)
{
	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	bool ret = GetPlace(session, postcode, place);
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return ret;
}

bool PostCode::GetPlace(Poco::Data::Session* session, const std::string& postcode, std::string& place)
{
	if (!session)
		return false;

	IF_NO_ROWS(stmt, *session, "SELECT place FROM postcode WHERE postcode=?",
		Poco::Data::Keywords::into(place),
		Poco::Data::Keywords::useRef(postcode),
		Poco::Data::Keywords::now)
	{
		return false;
	}

	return true;
}
