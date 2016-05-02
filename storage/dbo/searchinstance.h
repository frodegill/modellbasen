#ifndef _DBO_SEARCHINSTANCE_H_
#define _DBO_SEARCHINSTANCE_H_

/* This is not a persisted object */

#include "../../app/application.h"
#include "tag.h"


namespace modellbasen
{

class SearchInstance {

public:
	SearchInstance(WebApplication* app, IdType tag_id, Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype);
	~SearchInstance();

public:
	IdType GetTagId() const {return m_tag_id;}
	bool ToString(std::string& str) const;

	bool IsInverted() const {return m_inverted;}
	void Invert() {m_inverted = !m_inverted;}

public:
	void SetIntValue(Poco::UInt32 intvalue);
	void SetIntValues(Poco::UInt32 intvalue, Poco::UInt32 intvalue2);
	void SetStringValue(const std::string& stringvalue);
	void SetDatetimeValue(TimeType datetimevalue);
	void SetSelectionValues(const std::list<IdType>& selectionvalues);

private:
	void DateAsString(std::string& str) const;
	void SingleselectAsString(std::string& str) const;
	void MultiselectAsString(std::string& str) const;

private:
	WebApplication* m_app;
	IdType m_tag_id;
	Tag::TagDataType m_insert_datatype;
	Tag::TagDataType m_query_datatype;

	bool m_inverted;

	Poco::UInt32 m_intvalue;
	Poco::UInt32 m_intvalue2;
	std::string  m_stringvalue;
	TimeType  m_datetimevalue;
	std::list<IdType> m_selectionvalues;
};

} // namespace modellbasen

#endif // _DBO_SEARCHINSTANCE_H_
