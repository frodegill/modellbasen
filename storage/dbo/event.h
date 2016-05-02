#ifndef _DBO_EVENT_H_
#define _DBO_EVENT_H_

#include "classes.h"
#include "../poco_glue.h"
#include "../../app/defines.h"
#include <string>


namespace modellbasen
{

class Event {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::Event>;
	bool operator <(const Event& event) const {return m_id < event.m_id;} //For set and multiset support
	IdType operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	Event();

public:
	IdType       GetId() const {return m_id;}

private:
	IdType m_id;
	TimeType  m_starttime;
	TimeType  m_endtime;
	std::string  m_description;
	IdType m_location_id;
	IdType m_owner_id;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::Event>
{
public:
	static std::size_t size() {return 6;}

	static void bind(std::size_t pos, const modellbasen::Event& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::bind(pos++, obj.m_id, pBinder, dir);
		TypeHandler<TimeType>::bind(pos++, obj.m_starttime, pBinder, dir);
		TypeHandler<TimeType>::bind(pos++, obj.m_endtime, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_description, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_location_id, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_owner_id, pBinder, dir);
	}

	static void prepare(std::size_t pos, const modellbasen::Event& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::prepare(pos++, obj.m_id, pPreparator);
		TypeHandler<TimeType>::prepare(pos++, obj.m_starttime, pPreparator);
		TypeHandler<TimeType>::prepare(pos++, obj.m_endtime, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_description, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_location_id, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_owner_id, pPreparator);
	}

	static void extract(std::size_t pos, modellbasen::Event& obj, const modellbasen::Event& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<TimeType>::extract(pos++, obj.m_starttime, defVal.m_starttime, pExt);
		TypeHandler<TimeType>::extract(pos++, obj.m_endtime, defVal.m_endtime, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_description, defVal.m_description, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_location_id, defVal.m_location_id, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_owner_id, defVal.m_owner_id, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_EVENT_H_
