#ifndef _QUERY_DIALOGS_H_
#define _QUERY_DIALOGS_H_

#include <Wt/WComboBox>
#include <Wt/WDatePicker>
#include <Wt/WGoogleMap>
#include <Wt/WLineEdit>
#include <Wt/WObject>
#include <Wt/WPushButton>
#include <Wt/WSelectionBox>
#include <Wt/WStringListModel>
#include <Wt/WVBoxLayout>
#include "../../application.h"
#include "../../../storage/dbo/tag.h"
#include "../../../storage/dbo/tagvalue.h"


namespace modellbasen
{

class SearchTab;

class QueryDialogs : public Wt::WObject
{
public:
	QueryDialogs(WebApplication* app, SearchTab* search_tab);
	~QueryDialogs();

	bool Initialize(Poco::UInt32 tag_id);

	bool ExecuteAsync();

	Tag* GetQueryTag() const {return m_query_tag.get();}

public:
	bool GetInt(Poco::UInt32& value) const;
	bool GetString(std::string& value) const;
	bool GetDatetime(Poco::UInt64& value) const;
	bool GetSingleSelect(Poco::UInt32& selected_value) const;
	bool GetMultiSelect(std::list<Poco::UInt32>& selected_values) const;
	bool GetInts(Poco::UInt32& value1, Poco::UInt32& value2) const;
	bool GetStringInt(std::string& value1, Poco::UInt32& value2) const;
	bool GetDistance(std::string& postcode, Poco::UInt32& distance) const;

private:
	bool AddIntControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label);
	bool AddStringControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label);
	bool AddDatetimeControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label);
	bool AddSingleSelectControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label);
	bool AddMultiSelectControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label);
	bool AddIntIntControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& label1, const Wt::WString& label2);
	bool AddDistanceControls(Wt::WVBoxLayout* layout, const Wt::WString& title, const Wt::WString& location_label, const Wt::WString& distance_label);

	bool AppendCommonControls(Wt::WVBoxLayout* layout);

	void FirstControlEnterPressed();
	void LastControlEnterPressed();

	void OnOK();
	void OnCancel();

	void PopulateSelectModel(const std::list<TagValue>& tag_values);

private:
	WebApplication* m_app;
	SearchTab* m_search_tab;
	std::shared_ptr<Tag>  m_query_tag;

	Wt::WLineEdit* m_int_edit;
	Wt::WLineEdit* m_int2_edit;
	Wt::WLineEdit* m_string_edit;
	Wt::WDatePicker* m_datetime_edit;

	Wt::WComboBox* m_singleselect_box;
	Wt::WSelectionBox* m_multiselect_box;
	Wt::WStringListModel* m_select_box_model;

	Wt::WGoogleMap* m_location_gmap;

	Wt::WPushButton* m_ok_button;
	Wt::WPushButton* m_cancel_button;
};

} // namespace modellbasen

#endif // _QUERY_DIALOGS_H_
