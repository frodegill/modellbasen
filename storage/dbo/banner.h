#ifndef _DBO_BANNER_H_
#define _DBO_BANNER_H_

#include "classes.h"
#include "../poco_glue.h"
#include "../../singleton/db.h"

#include <string>


namespace modellbasen
{

class Banner {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::Banner>;
	bool operator <(const Banner& banner) const {return m_id < banner.m_id;} //For set and multiset support
	Poco::UInt32 operator()() const {return m_id;} //Operator to return the key for the map and multimap

public:
	Banner();

	void Reset() {m_id = 0;}
	bool IsValid() const {return 0!=m_id;}

	static bool GetDisplayBanner(Poco::Data::Session* session_in_transaction, Banner& banner);

public:
	Poco::UInt32       GetId() const {return m_id;}
	std::string        GetText() const {return m_text;}
	std::string        GetURL() const {return m_url;}

private:
	Poco::UInt32 m_id;
	Poco::Int64  m_valid_from;
	Poco::Int64  m_valid_to;
	std::string  m_responsible_email;
	std::string  m_text;
	std::string  m_url;
	Poco::UInt32 m_hits;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::Banner>
{
public:
	static std::size_t size() {return 7;}

	static void bind(std::size_t pos, const modellbasen::Banner& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_id, pBinder, dir);
		TypeHandler<Poco::Int64>::bind(pos++, obj.m_valid_from, pBinder, dir);
		TypeHandler<Poco::Int64>::bind(pos++, obj.m_valid_to, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_responsible_email, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_text, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_url, pBinder, dir);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_hits, pBinder, dir);
	}

	static void prepare(std::size_t pos, const modellbasen::Banner& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_id, pPreparator);
		TypeHandler<Poco::Int64>::prepare(pos++, obj.m_valid_from, pPreparator);
		TypeHandler<Poco::Int64>::prepare(pos++, obj.m_valid_to, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_responsible_email, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_text, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_url, pPreparator);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_hits, pPreparator);
	}

	static void extract(std::size_t pos, modellbasen::Banner& obj, const modellbasen::Banner& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<Poco::Int64>::extract(pos++, obj.m_valid_from, defVal.m_valid_from, pExt);
		TypeHandler<Poco::Int64>::extract(pos++, obj.m_valid_to, defVal.m_valid_to, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_responsible_email, defVal.m_responsible_email, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_text, defVal.m_text, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_url, defVal.m_url, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_hits, defVal.m_hits, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_BANNER_H_
