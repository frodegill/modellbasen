#ifndef _DBO_SEARCHINSTANCE_H_
#define _DBO_SEARCHINSTANCE_H_

/* This is not a persisted object */

#include "../../app/application.h"
#include "tag.h"


namespace modellbasen
{

class SearchInstance {

public:
	SearchInstance(WebApplication* app, Poco::UInt32 tag_id, Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype);
	~SearchInstance();

public:
	Poco::UInt32 GetTagId() const {return m_tag_id;}
	bool ToString(std::string& str) const;

public:
	void SetIntValue(Poco::UInt32 intvalue);
	void SetIntValues(Poco::UInt32 intvalue, Poco::UInt32 intvalue2);
	void SetStringValue(const std::string& stringvalue);
	void SetDatetimeValue(Poco::Int64 datetimevalue);
	void SetSelectionValues(const std::list<Poco::UInt32>& selectionvalues);

private:
	void DateAsString(std::string& str) const;
	void SingleselectAsString(std::string& str) const;
	void MultiselectAsString(std::string& str) const;

private:
	WebApplication* m_app;
	Poco::UInt32 m_tag_id;
	Tag::TagDataType m_insert_datatype;
	Tag::TagDataType m_query_datatype;

	Poco::UInt32 m_intvalue;
	Poco::UInt32 m_intvalue2;
	std::string  m_stringvalue;
	Poco::Int64  m_datetimevalue;
	std::list<Poco::UInt32> m_selectionvalues;
};

} // namespace modellbasen

#endif // _DBO_SEARCHINSTANCE_H_
