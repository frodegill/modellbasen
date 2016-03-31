#include "banner.h"

#include "../../singleton/db.h"
#include "../../utils/time.h"


using namespace modellbasen;

Banner::Banner()
: m_id(0),
  m_valid_from(0),
  m_valid_to(0),
  m_hits(0)
{
}

bool Banner::GetDisplayBanner(Banner& banner)
{
	Poco::Data::Session* session_in_transaction;
	if (!DB.CreateSession(session_in_transaction))
		return false;

	banner.Reset();
	Poco::UInt64 now = Time::NowUTC();

	*session_in_transaction << "SELECT id, valid_from, valid_to, responsible_email, text, url, hits FROM banner WHERE valid_from<=? AND valid_to>=? ORDER BY hits ASC LIMIT 1",
	Poco::Data::Keywords::into(banner), Poco::Data::Keywords::use(now), Poco::Data::Keywords::use(now), Poco::Data::Keywords::now;

	if (banner.IsValid())
	{
		Poco::UInt32 id = banner.GetId();
		*session_in_transaction << "UPDATE banner SET hits=hits+1 WHERE id=?",
				Poco::Data::Keywords::use(id),
				Poco::Data::Keywords::now;
	}

	DB.ReleaseSession(session_in_transaction, PocoGlue::COMMIT);

	return banner.IsValid();
}
