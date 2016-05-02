#ifndef _DBO_POSTCODE_H_
#define _DBO_POSTCODE_H_

#include "classes.h"
#include "../poco_glue.h"
#include "../../app/defines.h"
#include <string>


namespace modellbasen
{

class PostCode {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::PostCode>;
	bool operator <(const PostCode& postcode) const {return m_postcode.compare(postcode.m_postcode);} //For set and multiset support
	IdType operator()() const {return atoi(m_postcode.c_str());} //Operator to return the key for the map and multimap

public:
	PostCode();

public:
	const std::string& GetPostCode() const {return m_postcode;}
	const std::string& GetPlace() const {return m_place;}
	double             GetLatitude() const {return m_latitude;}
	double             GetLongitude() const {return m_longitude;}

public:
	static bool Exists(const std::string& postcode, bool& exists);
	static bool Exists(Poco::Data::Session* session, const std::string& postcode, bool& exists);

	static bool GetPlace(const std::string& postcode, std::string& place);
	static bool GetPlace(Poco::Data::Session* session, const std::string& postcode, std::string& place);

private:
	std::string  m_postcode;
	std::string  m_place;
	double       m_latitude;
	double       m_longitude;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::PostCode>
{
public:
	static std::size_t size() {return 4;}

	static void bind(std::size_t pos, const modellbasen::PostCode& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<std::string>::bind(pos++, obj.m_postcode, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, obj.m_place, pBinder, dir);
		TypeHandler<double>::bind(pos++, obj.m_latitude, pBinder, dir);
		TypeHandler<double>::bind(pos++, obj.m_longitude, pBinder, dir);
	}

	static void prepare(std::size_t pos, const modellbasen::PostCode& obj, AbstractPreparator::Ptr pPreparator)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<std::string>::prepare(pos++, obj.m_postcode, pPreparator);
		TypeHandler<std::string>::prepare(pos++, obj.m_place, pPreparator);
		TypeHandler<double>::prepare(pos++, obj.m_latitude, pPreparator);
		TypeHandler<double>::prepare(pos++, obj.m_longitude, pPreparator);
	}

	static void extract(std::size_t pos, modellbasen::PostCode& obj, const modellbasen::PostCode& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_postcode, defVal.m_postcode, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_place, defVal.m_place, pExt);
		TypeHandler<double>::extract(pos++, obj.m_latitude, defVal.m_latitude, pExt);
		TypeHandler<double>::extract(pos++, obj.m_longitude, defVal.m_longitude, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_POSTCODE_H_
