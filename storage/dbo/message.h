#ifndef _DBO_MESSAGE_H_
#define _DBO_MESSAGE_H_

#include "classes.h"
#include "../poco_glue.h"
#include "../../app/defines.h"
#include <string>


namespace modellbasen
{

class Message {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::Message>;
	bool operator <(const Message& message) const {return m_id < message.m_id;} //For set and multiset support
	IdType operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	Message();

public:
	IdType       GetId() const {return m_id;}

public:
	static bool GetUnreadCount(const IdType& user_id, size_t& count);
	static bool GetUnreadCount(Poco::Data::Session* session, const IdType& user_id, size_t& count);

	static bool GetAllUserMessagesMetadata(Poco::Data::Session* session, const IdType& user_id, std::list<Message>& messages);

private:
	IdType m_id;
	std::string  m_subject;
	std::string  m_text;
	IdType m_sender_id;
	IdType m_recipient_id;
	TimeType  m_sent_time;
	TimeType  m_read_time;
	TimeType  m_replied_time;
	IdType m_in_reply_to_id;
	bool         m_sender_has_deleted;
	bool         m_recipient_has_deleted;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::Message>
{
public:
	static std::size_t size() {return 11;}

	static void bind(std::size_t pos, const modellbasen::Message& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::bind(pos++, obj.m_id, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_subject, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_text, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_sender_id, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_recipient_id, pBinder, dir);
		TypeHandler<TimeType>::bind(pos++, obj.m_sent_time, pBinder, dir);
		TypeHandler<TimeType>::bind(pos++, obj.m_read_time, pBinder, dir);
		TypeHandler<TimeType>::bind(pos++, obj.m_replied_time, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_in_reply_to_id, pBinder, dir);
		TypeHandler<bool>::bind(pos++, obj.m_sender_has_deleted, pBinder, dir);
		TypeHandler<bool>::bind(pos++, obj.m_recipient_has_deleted, pBinder, dir);
	}

	static void prepare(std::size_t pos, const modellbasen::Message& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::prepare(pos++, obj.m_id, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_subject, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_text, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_sender_id, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_recipient_id, pPreparator);
		TypeHandler<TimeType>::prepare(pos++, obj.m_sent_time, pPreparator);
		TypeHandler<TimeType>::prepare(pos++, obj.m_read_time, pPreparator);
		TypeHandler<TimeType>::prepare(pos++, obj.m_replied_time, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_in_reply_to_id, pPreparator);
		TypeHandler<bool>::prepare(pos++, obj.m_sender_has_deleted, pPreparator);
		TypeHandler<bool>::prepare(pos++, obj.m_recipient_has_deleted, pPreparator);
	}

	static void extract(std::size_t pos, modellbasen::Message& obj, const modellbasen::Message& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_subject, defVal.m_subject, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_text, defVal.m_text, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_sender_id, defVal.m_sender_id, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_recipient_id, defVal.m_recipient_id, pExt);
		TypeHandler<TimeType>::extract(pos++, obj.m_sent_time, defVal.m_sent_time, pExt);
		TypeHandler<TimeType>::extract(pos++, obj.m_read_time, defVal.m_read_time, pExt);
		TypeHandler<TimeType>::extract(pos++, obj.m_replied_time, defVal.m_replied_time, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_in_reply_to_id, defVal.m_in_reply_to_id, pExt);
		TypeHandler<bool>::extract(pos++, obj.m_sender_has_deleted, defVal.m_sender_has_deleted, pExt);
		TypeHandler<bool>::extract(pos++, obj.m_recipient_has_deleted, defVal.m_recipient_has_deleted, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_MESSAGE_H_
