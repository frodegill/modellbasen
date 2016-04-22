#include "uservisit.h"

#include "../../app/defines.h"


using namespace modellbasen;

UserVisit::UserVisit()
: m_id(INVALID_ID),
  m_user_id(INVALID_ID),
  m_visitor_id(INVALID_ID),
  m_last_visit(EPOCH),
  m_visits(0)
{
}
