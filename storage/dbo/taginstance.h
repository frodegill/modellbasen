#ifndef _DBO_TAGINSTANCE_H_
#define _DBO_TAGINSTANCE_H_

#include "classes.h"
#include "../poco_glue.h"
#include "../../app/defines.h"
#include <string>
#include <Poco/Timestamp.h>


namespace modellbasen
{

class TagInstance {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::TagInstance>;
	bool operator <(const TagInstance& taginstance) const {return m_id < taginstance.m_id;} //For set and multiset support
	IdType operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	TagInstance();

public:
	IdType       GetId() const {return m_id;}

private:
	IdType m_id;
	Poco::Nullable<std::string>  m_stringvalue;
	Poco::Nullable<Poco::UInt32> m_intvalue;
	Poco::Nullable<TimeType>  m_datetimevalue;
	IdType m_tag_id;
	IdType m_owner_id;
	IdType m_eventparticipant_id;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::TagInstance>
{
public:
	static std::size_t size() {return 7;}

	static void bind(std::size_t pos, const modellbasen::TagInstance& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::bind(pos++, obj.m_id, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_stringvalue, pBinder, dir);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_intvalue, pBinder, dir);
		TypeHandler<TimeType>::bind(pos++, obj.m_datetimevalue, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_tag_id, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_owner_id, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_eventparticipant_id, pBinder, dir);
	}

	static void prepare(std::size_t pos, const modellbasen::TagInstance& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::prepare(pos++, obj.m_id, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_stringvalue, pPreparator);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_intvalue, pPreparator);
		TypeHandler<TimeType>::prepare(pos++, obj.m_datetimevalue, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_tag_id, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_owner_id, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_eventparticipant_id, pPreparator);
	}

	static void extract(std::size_t pos, modellbasen::TagInstance& obj, const modellbasen::TagInstance& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_stringvalue, defVal.m_stringvalue, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_intvalue, defVal.m_intvalue, pExt);
		TypeHandler<TimeType>::extract(pos++, obj.m_datetimevalue, defVal.m_datetimevalue, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_tag_id, defVal.m_tag_id, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_owner_id, defVal.m_owner_id, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_eventparticipant_id, defVal.m_eventparticipant_id, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_TAGINSTANCE_H_
