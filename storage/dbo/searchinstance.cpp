#include "searchinstance.h"


using namespace modellbasen;


SearchInstance::SearchInstance(Poco::UInt32 tag_id, Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype)
: m_tag_id(tag_id),
  m_insert_datatype(insert_datatype),
  m_query_datatype(query_datatype),
  m_intvalue(0),
  m_intvalue2(0),
  m_datetimevalue(0)
{
}

SearchInstance::~SearchInstance()
{
	int dummy = 0;
	dummy++;
}

void SearchInstance::SetIntValue(Poco::UInt32 intvalue)
{
	if (Tag::INTEGER==m_query_datatype || Tag::LOCATION==m_query_datatype)
	{
		m_intvalue = intvalue;
	}
}

void SearchInstance::SetIntValues(Poco::UInt32 intvalue, Poco::UInt32 intvalue2)
{
	if (Tag::HEIGHT_RANGE==m_query_datatype || Tag::DAY_RANGE==m_query_datatype ||
	    Tag::AGE_RANGE==m_query_datatype || Tag::DISTANCE==m_query_datatype)
	{
		m_intvalue = intvalue;
		m_intvalue2 = intvalue2;
	}
}

void SearchInstance::SetStringValue(const std::string& stringvalue)
{
	if (Tag::STRING==m_query_datatype)
	{
		m_stringvalue = stringvalue;
	}
}

void SearchInstance::SetDatetimeValue(Poco::Int64 datetimevalue)
{
	if (Tag::DATETIME==m_query_datatype)
	{
		m_datetimevalue = datetimevalue;
	}
}

void SearchInstance::SetSelectionValues(const std::list<Poco::UInt32>& selectionvalues)
{
	if ((Tag::SINGLESELECT==m_query_datatype && 1==selectionvalues.size()) ||
	    Tag::MULTISELECT==m_query_datatype)
	{
	}
}
