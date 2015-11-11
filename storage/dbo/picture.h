#ifndef _DBO_PICTURE_H_
#define _DBO_PICTURE_H_

#include "classes.h"
#include "../poco_glue.h"
#include <string>


namespace modellbasen
{

class Picture {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::Picture>;
	bool operator <(const Picture& picture) const {return m_id < picture.m_id;} //For set and multiset support
	Poco::UInt32 operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	Picture();

public:
	Poco::UInt32       GetId() const {return m_id;}

private:
	Poco::UInt32 m_id;
	Poco::UInt32 m_owner_id;
	Poco::UInt32 m_event_id;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::Picture>
{
public:
	static std::size_t size() {return 3;}

	static void bind(std::size_t pos, const modellbasen::Picture& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_id, pBinder, dir);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_owner_id, pBinder, dir);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_event_id, pBinder, dir);
	}

	static void prepare(std::size_t pos, const modellbasen::Picture& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_id, pPreparator);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_owner_id, pPreparator);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_event_id, pPreparator);
	}

	static void extract(std::size_t pos, modellbasen::Picture& obj, const modellbasen::Picture& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_owner_id, defVal.m_owner_id, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_event_id, defVal.m_event_id, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_PICTURE_H_
