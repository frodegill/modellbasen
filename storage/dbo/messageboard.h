#ifndef _DBO_MESSAGEBOARD_H_
#define _DBO_MESSAGEBOARD_H_

#include "classes.h"
#include "../poco_glue.h"
#include <string>


namespace modellbasen
{

class MessageBoard {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::MessageBoard>;
	bool operator <(const MessageBoard& messageboard) const {return m_id < messageboard.m_id;} //For set and multiset support
	Poco::UInt32 operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	MessageBoard();

public:
	Poco::UInt32       GetId() const {return m_id;}

private:
	Poco::UInt32 m_id;
	std::string  m_message;
	Poco::Int64  m_posted_time;
	Poco::UInt32 m_user_id;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::MessageBoard>
{
public:
	static std::size_t size() {return 4;}

	static void bind(std::size_t pos, const modellbasen::MessageBoard& obj, AbstractBinder* pBinder)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_id, pBinder);
		TypeHandler<std::string>::bind(pos++, obj.m_message, pBinder);
		TypeHandler<Poco::Int64>::bind(pos++, obj.m_posted_time, pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_user_id, pBinder);
	}

	static void prepare(std::size_t pos, const modellbasen::MessageBoard& obj, AbstractPreparation* pPrepare)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_id, pPrepare);
		TypeHandler<std::string>::prepare(pos++, obj.m_message, pPrepare);
		TypeHandler<Poco::Int64>::prepare(pos++, obj.m_posted_time, pPrepare);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_user_id, pPrepare);
	}

	static void extract(std::size_t pos, modellbasen::MessageBoard& obj, const modellbasen::MessageBoard& defVal, AbstractExtractor* pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_message, defVal.m_message, pExt);
		TypeHandler<Poco::Int64>::extract(pos++, obj.m_posted_time, defVal.m_posted_time, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_user_id, defVal.m_user_id, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_MESSAGEBOARD_H_
