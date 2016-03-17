
#ifndef _MAIN_WIDGET_H_
#define _MAIN_WIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WTabWidget>
#include <Wt/WVBoxLayout>


namespace modellbasen
{

class AdministratorTab;
class MessageBoardTab;
class ProfileTab;
class SearchTab;
class WebApplication;

class MainWidget : public Wt::WContainerWidget
{
public:
	MainWidget(WebApplication* app);
	~MainWidget();

	void Initialize();

private:
	void AddHeader(Wt::WVBoxLayout* layout);
	void CreateProfileTab(Wt::WTabWidget* tab_widget);
	void CreateMessageBoardTab(Wt::WTabWidget* tab_widget);
	void CreateSearchTab(Wt::WTabWidget* tab_widget);
	void CreateAdministratorTab(Wt::WTabWidget* tab_widget);
	void UpdateTabVisibility();

public:
	void OnLoggedIn();
	void OnLoggedOut();

private:
	WebApplication* m_app;
	bool            m_initialized;

	Wt::WTabWidget*   m_tab_widget;
	ProfileTab*       m_profile_tab;
	MessageBoardTab*  m_messageboard_tab;
	SearchTab*        m_search_tab;
	AdministratorTab* m_administrator_tab;
};

} // namespace modellbasen

#endif // _MAIN_WIDGET_H_
