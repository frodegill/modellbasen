#ifndef _DBO_MESSAGEBOARD_H_
#define _DBO_MESSAGEBOARD_H_

#include "classes.h"
#include "../poco_glue.h"
#include "../../app/defines.h"
#include "../../singleton/db.h"

#include <string>

#define MAX_MESSAGEBOARD_ROWS (25)


namespace modellbasen
{

class MessageBoard {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::MessageBoard>;
	bool operator <(const MessageBoard& messageboard) const {return m_id < messageboard.m_id;} //For set and multiset support
	IdType operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	MessageBoard();

	static bool InitializeGlobalMessageboardList();
	static bool AddMessage(Poco::Data::Session* session_in_transaction, IdType user_id, const std::string& message);

public:
	IdType       GetId() const {return m_id;}

private:
	IdType m_id;
	std::string  m_message;
	TimeType  m_posted_time;
	IdType m_user_id;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::MessageBoard>
{
public:
	static std::size_t size() {return 4;}

	static void bind(std::size_t pos, const modellbasen::MessageBoard& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::bind(pos++, obj.m_id, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_message, pBinder, dir);
		TypeHandler<TimeType>::bind(pos++, obj.m_posted_time, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_user_id, pBinder, dir);
	}

	static void prepare(std::size_t pos, const modellbasen::MessageBoard& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::prepare(pos++, obj.m_id, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_message, pPreparator);
		TypeHandler<TimeType>::prepare(pos++, obj.m_posted_time, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_user_id, pPreparator);
	}

	static void extract(std::size_t pos, modellbasen::MessageBoard& obj, const modellbasen::MessageBoard& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_message, defVal.m_message, pExt);
		TypeHandler<TimeType>::extract(pos++, obj.m_posted_time, defVal.m_posted_time, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_user_id, defVal.m_user_id, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_MESSAGEBOARD_H_
