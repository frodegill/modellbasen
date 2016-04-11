#include "event.h"

#include "../../app/defines.h"


using namespace modellbasen;

Event::Event()
: m_id(INVALID_ID),
  m_starttime(0),
  m_endtime(0),
  m_location_id(INVALID_ID),
  m_owner_id(INVALID_ID)
{
}
