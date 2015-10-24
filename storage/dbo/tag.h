#ifndef _DBO_TAG_H_
#define _DBO_TAG_H_

#include "classes.h"
#include "../poco_glue.h"
#include <string>


namespace modellbasen
{

#define TAG_ADMINISTRATOR                  ("tag.administrator")
#define TAG_AGENCY                         ("tag.agency")
#define TAG_BIRTHDAY                       ("tag.birthday")
#define TAG_CONFIRMEDBY                    ("tag.confirmedby")
#define TAG_HAIR                           ("tag.hair")
#define TAG_HAIR_EXPERIENCE                ("tag.hair.experience")
#define TAG_HAIR_TFP                       ("tag.hair.tfp")
#define TAG_LASTVISITEDTIME                ("tag.lastvisitedtime")
#define TAG_MODEL                          ("tag.model")
#define TAG_MODEL_EXPERIENCE               ("tag.model.experience")
#define TAG_MODEL_HAIRCOLOUR               ("tag.model.haircolour")
#define TAG_MODEL_HAIRLENGTH               ("tag.model.hairlength")
#define TAG_MODEL_HEIGHT                   ("tag.model.height")
#define TAG_MODEL_PAID                     ("tag.model.paid")
#define TAG_MODEL_PIERCING                 ("tag.model.piercing")
#define TAG_MODEL_TATTOO                   ("tag.model.tattoo")
#define TAG_MODEL_TFP                      ("tag.model.tfp")
#define TAG_MUA                            ("tag.mua")
#define TAG_MUA_EXPERIENCE                 ("tag.mua.experience")
#define TAG_MUA_TFP                        ("tag.mua.tfp")
#define TAG_NAME                           ("tag.name")
#define TAG_PHOTOGRAPHER                   ("tag.photographer")
#define TAG_PHOTOGRAPHER_EXPERIENCE        ("tag.photographer.experience")
#define TAG_PHOTOGRAPHER_OWNSTUDIO         ("tag.photographer.ownstudio")
#define TAG_PHOTOGRAPHER_PREFERREDLOCATION ("tag.photographer.preferredlocation")
#define TAG_PHOTOGRAPHER_TFP               ("tag.photographer.tfp")
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

public:
	Poco::UInt32       GetId() const {return m_id;}
	const std::string& GetName() const {return m_name;}
	TagDataType        GetInsertDataType() const {return static_cast<TagDataType>(m_insert_datatype);}
	TagDataType        GetQueryDataType() const {return static_cast<TagDataType>(m_query_datatype);}
	Poco::UInt32       GetParentId() const {return m_parent_id;}
	bool							 IsQueryOnly() const {return m_query_only;}

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
