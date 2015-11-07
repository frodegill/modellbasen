#ifndef _DBO_TAG_H_
#define _DBO_TAG_H_

#include <string>
#include <Wt/WLocalizedStrings>
#include "classes.h"
#include "../poco_glue.h"


namespace modellbasen
{
#define TAG_ADMINISTRATOR                  ("tag.administrator")
#define TAG_AGENCY                         ("tag.agency")
#define TAG_AGENCY_COMPANYADDRESS          ("tag.agency.companyaddress")
#define TAG_AGENCY_COMPANYNAME             ("tag.agency.companyname")
#define TAG_AGENCY_PHONE                   ("tag.agency.phone")
#define TAG_AGENCY_PUBLICEMAIL             ("tag.agency.publicemail")
#define TAG_AGENCY_WEBPAGE                 ("tag.agency.webpage")
#define TAG_BIRTHDAY                       ("tag.birthday")
#define TAG_CONFIRMEDBY                    ("tag.confirmedby")
#define TAG_HAIRDRESSER                    ("tag.hairdresser")
#define TAG_HAIRDRESSER_COMPANYADDRESS     ("tag.hairdresser.companyaddress")
#define TAG_HAIRDRESSER_COMPANYNAME        ("tag.hairdresser.companyname")
#define TAG_HAIRDRESSER_EXPERIENCE         ("tag.hairdresser.experience")
#define TAG_HAIRDRESSER_PHONE              ("tag.hairdresser.phone")
#define TAG_HAIRDRESSER_PUBLICEMAIL        ("tag.hairdresser.publicemail")
#define TAG_HAIRDRESSER_TFP                ("tag.hairdresser.tfp")
#define TAG_HAIRDRESSER_WEBPAGE            ("tag.hairdresser.webpage")
#define TAG_LASTVISITEDTIME                ("tag.lastvisitedtime")
#define TAG_MODEL                          ("tag.model")
#define TAG_MODEL_COMPANYADDRESS           ("tag.model.companyaddress")
#define TAG_MODEL_COMPANYNAME              ("tag.model.companyname")
#define TAG_MODEL_EXPERIENCE               ("tag.model.experience")
#define TAG_MODEL_HAIRCOLOUR               ("tag.model.haircolour")
#define TAG_MODEL_HAIRLENGTH               ("tag.model.hairlength")
#define TAG_MODEL_HEIGHT                   ("tag.model.height")
#define TAG_MODEL_PAID                     ("tag.model.paid")
#define TAG_MODEL_PHONE                    ("tag.model.phone")
#define TAG_MODEL_PIERCING                 ("tag.model.piercing")
#define TAG_MODEL_PUBLICEMAIL              ("tag.model.publicemail")
#define TAG_MODEL_TATTOO                   ("tag.model.tattoo")
#define TAG_MODEL_TFP                      ("tag.model.tfp")
#define TAG_MODEL_WEBPAGE                  ("tag.model.webpage")
#define TAG_MUA                            ("tag.mua")
#define TAG_MUA_COMPANYADDRESS             ("tag.mua.companyaddress")
#define TAG_MUA_COMPANYNAME                ("tag.mua.companyname")
#define TAG_MUA_EXPERIENCE                 ("tag.mua.experience")
#define TAG_MUA_PHONE                      ("tag.mua.phone")
#define TAG_MUA_PUBLICEMAIL                ("tag.mua.publicemail")
#define TAG_MUA_TFP                        ("tag.mua.tfp")
#define TAG_MUA_WEBPAGE                    ("tag.mua.webpage")
#define TAG_NAME                           ("tag.name")
#define TAG_PHOTOGRAPHER                   ("tag.photographer")
#define TAG_PHOTOGRAPHER_COMPANYADDRESS    ("tag.photographer.companyaddress")
#define TAG_PHOTOGRAPHER_COMPANYNAME       ("tag.photographer.companyname")
#define TAG_PHOTOGRAPHER_EXPERIENCE        ("tag.photographer.experience")
#define TAG_PHOTOGRAPHER_OWNSTUDIO         ("tag.photographer.ownstudio")
#define TAG_PHOTOGRAPHER_PHONE             ("tag.photographer.phone")
#define TAG_PHOTOGRAPHER_PREFERREDLOCATION ("tag.photographer.preferredlocation")
#define TAG_PHOTOGRAPHER_PUBLICEMAIL       ("tag.photographer.publicemail")
#define TAG_PHOTOGRAPHER_TFP               ("tag.photographer.tfp")
#define TAG_PHOTOGRAPHER_WEBPAGE           ("tag.photographer.webpage")
#define TAG_POSTCODE                       ("tag.postcode")
#define TAG_REGISTEREDTIME                 ("tag.registeredtime")
#define TAG_SEX                            ("tag.sex")
#define TAG_SIRNAME                        ("tag.sirname")

	

class Tag {
public: //For PoCo::Data
	friend class Poco::Data::TypeHandler<class modellbasen::Tag>;
	bool operator <(const Tag& tag) const {return m_id < tag.m_id;} //For set and multiset support
	Poco::UInt32 operator()() const {return m_id;} //Operator to return the key for the map and multimap

	enum TagDataType {
		UNKNOWN=0,
		INTEGER=1,
		STRING=2,
		DATETIME=3,
		BOOLEAN=4,
		LOCATION=5,
		SINGLESELECT=6,
		MULTISELECT=7,
		HEIGHT_RANGE=8,
		DAY_RANGE=9,
		AGE_RANGE=10,
		DISTANCE=11
	};

public:
	Tag();
	bool Initialize(Poco::UInt32 id);

public:
	Poco::UInt32       GetId() const {return m_id;}
	const std::string& GetName() const {return m_name;}
	TagDataType        GetInsertDataType() const {return static_cast<TagDataType>(m_insert_datatype);}
	TagDataType        GetQueryDataType() const {return static_cast<TagDataType>(m_query_datatype);}
	Poco::UInt32       GetParentId() const {return m_parent_id;}
	bool               IsQueryOnly() const {return m_query_only;}

	bool               GetTagValues(std::list<TagValue>& tag_values) const;

	bool               GetTagText(Wt::WLocalizedStrings* localized_strings, bool is_query, std::string& text) const;

private:
	Poco::UInt32 m_id;
	std::string  m_name;
	Poco::UInt32 m_insert_datatype;
	Poco::UInt32 m_query_datatype;
	Poco::UInt32 m_parent_id;
	bool         m_query_only;
};

} // namespace modellbasen


namespace Poco {
namespace Data {
template <> class TypeHandler<class modellbasen::Tag>
{
public:
	static std::size_t size() {return 6;}

	static void bind(std::size_t pos, const modellbasen::Tag& obj, AbstractBinder* pBinder)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_id, pBinder);
		TypeHandler<std::string>::bind(pos++, obj.m_name, pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_insert_datatype, pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_query_datatype, pBinder);
		TypeHandler<Poco::UInt32>::bind(pos++, obj.m_parent_id, pBinder);
		TypeHandler<bool>::bind(pos++, obj.m_query_only, pBinder);
	}

	static void prepare(std::size_t pos, const modellbasen::Tag& obj, AbstractPreparation* pPrepare)
	{
		poco_assert_dbg(pBinder);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_id, pPrepare);
		TypeHandler<std::string>::prepare(pos++, obj.m_name, pPrepare);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_insert_datatype, pPrepare);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_query_datatype, pPrepare);
		TypeHandler<Poco::UInt32>::prepare(pos++, obj.m_parent_id, pPrepare);
		TypeHandler<bool>::prepare(pos++, obj.m_query_only, pPrepare);
	}

	static void extract(std::size_t pos, modellbasen::Tag& obj, const modellbasen::Tag& defVal, AbstractExtractor* pExt)
	{
		poco_assert_dbg(pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_id, defVal.m_id, pExt);
		TypeHandler<std::string>::extract(pos++, obj.m_name, defVal.m_name, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_insert_datatype, defVal.m_insert_datatype, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_query_datatype, defVal.m_query_datatype, pExt);
		TypeHandler<Poco::UInt32>::extract(pos++, obj.m_parent_id, defVal.m_parent_id, pExt);
		TypeHandler<bool>::extract(pos++, obj.m_query_only, defVal.m_query_only, pExt);
	}
};

} } // namespace Poco::Data

#endif // _DBO_TAG_H_
