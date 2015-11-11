#ifndef _SEARCH_TAB_H_
#define _SEARCH_TAB_H_

#include <Wt/WContainerWidget>
#include <Wt/WGroupBox>

#include "../../../storage/dbo/search.h"


namespace modellbasen
{

class WebApplication;

class SearchTab : public Wt::WContainerWidget
{
public:
	SearchTab(WebApplication* app);
	~SearchTab();

private:
	Wt::WContainerWidget* CreateTagsContainer();
	Wt::WContainerWidget* CreateResultsContainer() const;

	void PopulateTagsContainers();

	void OnSearchInstanceTagButtonClicked(Poco::UInt32 tag_id);
	void OnAvailableTagButtonClicked(Poco::UInt32 tag_id);

private:
	WebApplication* m_app;
	
	Search m_search;

	Wt::WContainerWidget* m_tags_container;
	Wt::WContainerWidget* m_results_container;

	Wt::WGroupBox* m_search_tags;
	Wt::WGroupBox* m_available_tags;
};

} // namespace modellbasen

#endif // _SEARCH_TAB_H_
