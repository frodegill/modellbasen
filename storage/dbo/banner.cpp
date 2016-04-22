#include "banner.h"

#include "../../app/defines.h"
#include "../../utils/time.h"


using namespace modellbasen;

Banner::Banner()
: m_id(INVALID_ID),
  m_valid_from(EPOCH),
  m_valid_to(EPOCH),
  m_hits(0)
{
}

bool Banner::GetDisplayBanner(Poco::Data::Session* session_in_transaction, Banner& banner)
{
	if (!session_in_transaction)
		return false;

	banner.Reset();
	Poco::UInt64 now = Time::NowUTC();

	*session_in_transaction << "SELECT id, valid_from, valid_to, responsible_email, text, url, hits FROM banner WHERE valid_from<=? AND valid_to>=? ORDER BY hits ASC LIMIT 1",
	Poco::Data::Keywords::into(banner), Poco::Data::Keywords::use(now), Poco::Data::Keywords::use(now), Poco::Data::Keywords::now;

	if (banner.IsValid())
	{
		Poco::UInt32 id = banner.GetId();
		DEBUG_TRY_CATCH(*session_in_transaction << "UPDATE banner SET hits=hits+1 WHERE id=?",
				Poco::Data::Keywords::use(id),
				Poco::Data::Keywords::now;)
	}

	return banner.IsValid();
}
