#ifdef USE_PCH
# include "../../pch.h"
#else
#endif

#include "uservisit.h"


using namespace modellbasen;

UserVisit::UserVisit()
: m_id(0),
  m_user_id(0),
  m_visitor_id(0),
  m_last_visit(0),
  m_visits(0)
{
}
