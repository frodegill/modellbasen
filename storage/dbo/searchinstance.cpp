#include <Wt/WString>
#include <Wt/WDateTime>

#include "searchinstance.h"
#include "tagvalue.h"


using namespace modellbasen;


SearchInstance::SearchInstance(WebApplication* app, Poco::UInt32 tag_id, Tag::TagDataType insert_datatype, Tag::TagDataType query_datatype)
: m_app(app),
  m_tag_id(tag_id),
  m_insert_datatype(insert_datatype),
  m_query_datatype(query_datatype),
  m_intvalue(0),
  m_intvalue2(0),
  m_datetimevalue(0)
{
}

SearchInstance::~SearchInstance()
{
}

bool SearchInstance::ToString(std::string& str) const
{
	Tag tag;
	tag.Initialize(m_tag_id);
	if (!tag.IsValid())
		return false;

	std::string tag_name;
	if (!tag.GetTagText(m_app->localizedStrings(), true, tag_name))
		return false;

	switch(m_query_datatype)
	{
		case Tag::INTEGER: str = Wt::WString::tr("ArgInteger").arg(tag_name).arg(m_intvalue).toUTF8(); break;
		case Tag::STRING: str = Wt::WString::tr("ArgString").arg(tag_name).arg(m_stringvalue).toUTF8(); break;
		case Tag::DATETIME:
		{
			std::string date_str;
			DateAsString(date_str);
			str = Wt::WString::tr("ArgDate").arg(tag_name).arg(date_str).toUTF8();
			break;
		}
		case Tag::BOOLEAN: str = Wt::WString::tr("ArgBoolean").arg(tag_name).toUTF8(); break;
		case Tag::LOCATION: str = Wt::WString::tr("ArgLocation").arg(tag_name).arg(m_stringvalue).toUTF8(); break;
		case Tag::SINGLESELECT:
		{
			std::string selection_str;
			SelectionAsString(selection_str);
			str = Wt::WString::tr("ArgSingleselect").arg(tag_name).arg(selection_str).toUTF8();
			break;
		}
		case Tag::MULTISELECT:
		{
			std::string selection_str;
			SelectionAsString(selection_str);
			str = Wt::WString::tr("ArgMultiselect").arg(tag_name).arg(selection_str).toUTF8();
			break;
		}
		case Tag::HEIGHT_RANGE: str = Wt::WString::tr("ArgHeightRange").arg(tag_name).arg(m_intvalue).arg(m_intvalue2).toUTF8(); break;
		case Tag::DAY_RANGE: str = Wt::WString::tr("ArgDayRange").arg(tag_name).arg(m_intvalue).arg(m_intvalue2).toUTF8(); break;
		case Tag::AGE_RANGE: str = Wt::WString::tr("ArgAgeRange").arg(tag_name).arg(m_intvalue).arg(m_intvalue2).toUTF8(); break;
		case Tag::DISTANCE: str = Wt::WString::tr("ArgDistance").arg(tag_name).arg(m_intvalue).arg(m_stringvalue).toUTF8(); break;
		default: str=tag_name; break;
	}
	return true;
}

void SearchInstance::SetIntValue(Poco::UInt32 intvalue)
{
	if (Tag::INTEGER==m_query_datatype || Tag::LOCATION==m_query_datatype || Tag::DISTANCE==m_query_datatype)
	{
		m_intvalue = intvalue;
	}
}

void SearchInstance::SetIntValues(Poco::UInt32 intvalue, Poco::UInt32 intvalue2)
{
	if (Tag::HEIGHT_RANGE==m_query_datatype || Tag::DAY_RANGE==m_query_datatype ||
	    Tag::AGE_RANGE==m_query_datatype)
	{
		m_intvalue = intvalue;
		m_intvalue2 = intvalue2;
	}
}

void SearchInstance::SetStringValue(const std::string& stringvalue)
{
	if (Tag::STRING==m_query_datatype || Tag::LOCATION==m_query_datatype || Tag::DISTANCE==m_query_datatype)
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
		m_selectionvalues = selectionvalues;
	}
}

void SearchInstance::DateAsString(std::string& str) const
{
	Wt::WDateTime date;
	date.setTime_t(m_datetimevalue);
  str = date.toString(Wt::WString::tr("DateFormat")).toUTF8();
}

void SearchInstance::SelectionAsString(std::string& str) const
{
	str.clear();

	std::string tag_value_str;
	for(std::list<Poco::UInt32>::const_iterator it = m_selectionvalues.begin(); it!=m_selectionvalues.end(); ++it)
	{
		TagValue tag_value;
		tag_value.Initialize(*it);
		if (!tag_value.IsValid())
			continue;

		if (tag_value.GetTagValueText(m_app->localizedStrings(), tag_value_str))
		{
			if (!str.empty())
			{
				str.append(", ");
			}
			
			str.append(tag_value_str);
		}
	}
}
