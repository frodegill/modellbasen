#ifndef _SEARCH_TAB_H_
#define _SEARCH_TAB_H_

#include <Wt/WContainerWidget>
#include <Wt/WGroupBox>
#include <Wt/WHBoxLayout>

#include "../dialog/query_dialogs.h"
#include "../../../storage/dbo/search.h"


namespace modellbasen
{

class WebApplication;

class SearchTab : public Wt::WContainerWidget
{
public:
	SearchTab(WebApplication* app);
	~SearchTab();

public:
	Wt::WContainerWidget* GetPageContainer() const {return m_page_container;}
	Wt::WContainerWidget* GetDialogContainer() const {return m_dialog_container;}
	Wt::WVBoxLayout*      GetDialogLayout() const {return m_dialog_container_vbox;}

public:
	void OnLoggedIn();
	void OnLoggedOut();
	void OnQueryConfirmed();
	void OnQueryCancelled();

private:
	Wt::WContainerWidget* CreateTagsContainer();
	Wt::WContainerWidget* CreateResultsContainer() const;

	void PopulateTagsContainers();

	void OnSearchInstanceTagButtonClicked(Poco::UInt32 tag_id);
	void OnAvailableTagButtonClicked(Poco::UInt32 tag_id);

private:
	WebApplication* m_app;
	
	Search* m_search;

	std::shared_ptr<QueryDialogs> m_current_query_dialog;

	Wt::WContainerWidget* m_page_container;
	Wt::WContainerWidget* m_dialog_container;
	Wt::WVBoxLayout*      m_dialog_container_vbox;

	Wt::WContainerWidget* m_tags_container;
	Wt::WContainerWidget* m_results_container;

	Wt::WGroupBox* m_search_tags;
	Wt::WGroupBox* m_available_tags;
};

} // namespace modellbasen

#endif // _SEARCH_TAB_H_
