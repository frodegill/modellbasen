#ifdef USE_PCH
# include "../../pch.h"
#else
#endif

#include "event.h"


using namespace modellbasen;

Event::Event()
: m_id(0),
  m_starttime(0),
  m_endtime(0),
  m_location_id(0),
  m_owner_id(0)
{
}
