#ifndef _DBO_EVENT_H_
#define _DBO_EVENT_H_

#include "classes.h"
#include "../poco_glue.h"
#include <string>


namespace modellbasen
{

class Event {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::Event>;
	bool operator <(const Event& event) const {return m_id < event.m_id;} //For set and multiset support
	Poco::UInt32 operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	Event();

public:
	Poco::UInt32       GetId() const {return m_id;}

private:
	Poco::UInt32 m_id;
	Poco::Int64  m_starttime;
	Poco::Int64  m_endtime;
	std::string  m_description;
	Poco::UInt32 m_location_id;
	Poco::UInt32 m_owner_id;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::Event>
{
public:
	static std::size_t size() {return 6;}

	static void bind(std::size_t pos, const modellbasen::Event& obj, AbstractBinder* pBinder)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_id, pBinder);
		TypeHandler<Poco::Int64>::bind(pos++, obj.m_starttime, pBinder);
		TypeHandler<Poco::Int64>::bind(pos++, obj.m_endtime, pBinder);
		TypeHandler<std::string>::bind(pos++, obj.m_description, pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_location_id, pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_owner_id, pBinder);
	}

	static void prepare(std::size_t pos, const modellbasen::Event& obj, AbstractPreparation* pPrepare)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_id, pPrepare);
		TypeHandler<Poco::Int64>::prepare(pos++, obj.m_starttime, pPrepare);
		TypeHandler<Poco::Int64>::prepare(pos++, obj.m_endtime, pPrepare);
		TypeHandler<std::string>::prepare(pos++, obj.m_description, pPrepare);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_location_id, pPrepare);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_owner_id, pPrepare);
	}

	static void extract(std::size_t pos, modellbasen::Event& obj, const modellbasen::Event& defVal, AbstractExtractor* pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<Poco::Int64>::extract(pos++, obj.m_starttime, defVal.m_starttime, pExt);
		TypeHandler<Poco::Int64>::extract(pos++, obj.m_endtime, defVal.m_endtime, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_description, defVal.m_description, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_location_id, defVal.m_location_id, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_owner_id, defVal.m_owner_id, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_EVENT_H_
