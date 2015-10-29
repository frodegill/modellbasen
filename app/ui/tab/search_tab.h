#ifndef _SEARCH_TAB_H_
#define _SEARCH_TAB_H_

#include <Wt/WContainerWidget>

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
	Wt::WContainerWidget* CreateTagsContainer() const;
	Wt::WContainerWidget* CreateResultsContainer() const;

private:
	WebApplication* m_app;
	
	Search m_search;

	Wt::WContainerWidget* m_tags_container;
	Wt::WContainerWidget* m_results_container;
};

} // namespace modellbasen

#endif // _SEARCH_TAB_H_
