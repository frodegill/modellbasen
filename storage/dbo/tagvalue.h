#ifndef _DBO_TAGVALUE_H_
#define _DBO_TAGVALUE_H_

#include "classes.h"
#include "../poco_glue.h"
#include <string>


namespace modellbasen
{

class TagValue {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::TagValue>;
	bool operator <(const TagValue& tagvalue) const {return m_id < tagvalue.m_id;} //For set and multiset support
	Poco::UInt32 operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	TagValue();

public:
	Poco::UInt32       GetId() const {return m_id;}

private:
	Poco::UInt32 m_id;
	std::string  m_value;
	Poco::UInt32 m_pos;
	Poco::UInt32 m_tag_id;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::TagValue>
{
public:
	static std::size_t size() {return 4;}

	static void bind(std::size_t pos, const modellbasen::TagValue& obj, AbstractBinder* pBinder)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_id, pBinder);
		TypeHandler<std::string>::bind(pos++, obj.m_value, pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_pos, pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_tag_id, pBinder);
	}

	static void prepare(std::size_t pos, const modellbasen::TagValue& obj, AbstractPreparation* pPrepare)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_id, pPrepare);
		TypeHandler<std::string>::prepare(pos++, obj.m_value, pPrepare);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_pos, pPrepare);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_tag_id, pPrepare);
	}

	static void extract(std::size_t pos, modellbasen::TagValue& obj, const modellbasen::TagValue& defVal, AbstractExtractor* pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_value, defVal.m_value, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_pos, defVal.m_pos, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_tag_id, defVal.m_tag_id, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_TAGVALUE_H_
