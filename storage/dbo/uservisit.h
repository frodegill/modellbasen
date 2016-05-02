#ifndef _DBO_USERVISIT_H_
#define _DBO_USERVISIT_H_

#include "classes.h"
#include "../poco_glue.h"
#include "../../app/defines.h"
#include <string>


namespace modellbasen
{

class UserVisit {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::UserVisit>;
	bool operator <(const UserVisit& uservisit) const {return m_id < uservisit.m_id;} //For set and multiset support
	IdType operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	UserVisit();

public:
	IdType       GetId() const {return m_id;}

private:
	IdType m_id;
	IdType m_user_id;
	IdType m_visitor_id;
	TimeType m_last_visit;
	Poco::UInt32 m_visits;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::UserVisit>
{
public:
	static std::size_t size() {return 5;}

	static void bind(std::size_t pos, const modellbasen::UserVisit& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::bind(pos++, obj.m_id, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_user_id, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_visitor_id, pBinder, dir);
		TypeHandler<TimeType>::bind(pos++, obj.m_last_visit, pBinder, dir);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_visits, pBinder, dir);
	}

	static void prepare(std::size_t pos, const modellbasen::UserVisit& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::prepare(pos++, obj.m_id, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_user_id, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_visitor_id, pPreparator);
		TypeHandler<TimeType>::prepare(pos++, obj.m_last_visit, pPreparator);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_visits, pPreparator);
	}

	static void extract(std::size_t pos, modellbasen::UserVisit& obj, const modellbasen::UserVisit& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_user_id, defVal.m_user_id, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_visitor_id, defVal.m_visitor_id, pExt);
		TypeHandler<TimeType>::extract(pos++, obj.m_last_visit, defVal.m_last_visit, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_visits, defVal.m_visits, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_USERVISIT_H_
