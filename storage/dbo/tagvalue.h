#ifndef _DBO_TAGVALUE_H_
#define _DBO_TAGVALUE_H_

#include "classes.h"
#include "../poco_glue.h"
#include "../../app/defines.h"
#include <string>
#include <Wt/WLocalizedStrings>


namespace modellbasen
{

class TagValue {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::TagValue>;
	bool operator <(const TagValue& tagvalue) const {return m_id < tagvalue.m_id;} //For set and multiset support
	IdType operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	TagValue();

	bool Initialize(IdType id);
	void Reset() {m_id=INVALID_ID;}
	bool IsValid() const {return INVALID_ID!=m_id;}

public:
	IdType       GetId() const {return m_id;}
	Poco::UInt32       GetPos() const {return m_pos;}
	IdType       GetTagId() const {return m_tag_id;}

	bool               GetTagValueText(Wt::WLocalizedStrings* localized_strings, std::string& text) const;

private:
	const std::string& GetValue() const {return m_value;}

private:
	IdType m_id;
	std::string  m_value;
	Poco::UInt32 m_pos;
	IdType m_tag_id;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::TagValue>
{
public:
	static std::size_t size() {return 4;}

	static void bind(std::size_t pos, const modellbasen::TagValue& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::bind(pos++, obj.m_id, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_value, pBinder, dir);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_pos, pBinder, dir);
		TypeHandler<IdType>::bind(pos++, obj.m_tag_id, pBinder, dir);
	}

	static void prepare(std::size_t pos, const modellbasen::TagValue& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<IdType>::prepare(pos++, obj.m_id, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_value, pPreparator);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_pos, pPreparator);
		TypeHandler<IdType>::prepare(pos++, obj.m_tag_id, pPreparator);
	}

	static void extract(std::size_t pos, modellbasen::TagValue& obj, const modellbasen::TagValue& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_value, defVal.m_value, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_pos, defVal.m_pos, pExt);
		TypeHandler<IdType>::extract(pos++, obj.m_tag_id, defVal.m_tag_id, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_TAGVALUE_H_
