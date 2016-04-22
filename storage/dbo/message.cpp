#include "message.h"

#include "../../app/defines.h"


using namespace modellbasen;

Message::Message()
: m_id(INVALID_ID),
  m_sender_id(INVALID_ID),
  m_recipient_id(INVALID_ID),
  m_sent_time(EPOCH),
  m_read_time(EPOCH),
  m_replied_time(EPOCH),
  m_in_reply_to_id(INVALID_ID),
  m_sender_has_deleted(false),
  m_recipient_has_deleted(false)
{
}
