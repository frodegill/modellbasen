#ifdef USE_PCH
# include "../../pch.h"
#else
#endif

#include "message.h"


using namespace modellbasen;

Message::Message()
: m_id(0),
  m_sender_id(0),
  m_recipient_id(0),
  m_sent_time(0),
  m_read_time(0),
  m_replied_time(0),
  m_in_reply_to_id(0),
  m_sender_has_deleted(false),
  m_recipient_has_deleted(false)
{
}
