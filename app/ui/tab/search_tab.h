#ifndef _SEARCH_TAB_H_
#define _SEARCH_TAB_H_

#include <Wt/WContainerWidget>


namespace modellbasen
{

class WebApplication;

class SearchTab : public Wt::WContainerWidget
{
public:
	SearchTab(WebApplication* app);
	~SearchTab();

private:
	WebApplication* m_app;
};

} // namespace modellbasen

#endif // _SEARCH_TAB_H_
