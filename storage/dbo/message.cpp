#include "message.h"

#include "../../app/defines.h"
#include "../../singleton/db.h"


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

bool Message::GetMessage(IdType id, Message& message)
{
	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	bool ret = GetMessage(session, id, message);
	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return ret;
}

bool Message::GetMessage(Poco::Data::Session* session, IdType id, Message& message)
{
	if (!session || INVALID_ID==id)
		return false;

	*session << "SELECT id,subject,message,sender,recipient,sent_time,read_time,replied_time,"\
	             "in_reply_to,sender_deleted,recipient_deleted FROM message WHERE id=?",
		Poco::Data::Keywords::into(message), Poco::Data::Keywords::use(id), Poco::Data::Keywords::now;
	return true;
}

bool Message::GetUnreadCount(const IdType& user_id, size_t& count)
{
	Poco::Data::Session* session;
	if (!DB.CreateSession(session))
		return false;

	bool ret = GetUnreadCount(session, user_id, count);
	
	DB.ReleaseSession(session, PocoGlue::IGNORE);
	return ret;
}

bool Message::GetUnreadCount(Poco::Data::Session* session, const IdType& user_id, size_t& count)
{
	if (!session)
		return false;

	TimeType epoch = EPOCH;
	IF_NO_ROWS(stmt, *session, "SELECT COUNT(*) FROM message WHERE recipient=? AND read_time=? AND recipient_deleted=false",
		Poco::Data::Keywords::into(count),
		Poco::Data::Keywords::useRef(user_id),
		Poco::Data::Keywords::use(epoch),
		Poco::Data::Keywords::now)
	{
		count = 0;
	}

	return true;
}
