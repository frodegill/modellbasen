#ifdef USE_PCH
# include "../../pch.h"
#else
#endif

#include "messageboard.h"


using namespace modellbasen;

MessageBoard::MessageBoard()
: m_id(0),
  m_posted_time(0),
  m_user_id(0)
{
}
