
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
class RegisterProfileTab;
class SearchTab;
class WebApplication;

class MainWidget : public Wt::WContainerWidget
{
public:
	enum TabType {PROFILE, REGISTER_PROFILE, MESSAGEBOARD, SEARCH, ADMINISTRATOR};

	MainWidget(WebApplication* app);
	~MainWidget();

	void Initialize();

private:
	void AddHeader(Wt::WVBoxLayout* layout);
	void CreateProfileTab(Wt::WTabWidget* tab_widget);
	void CreateRegisterProfileTab(Wt::WTabWidget* tab_widget);
	void CreateMessageBoardTab(Wt::WTabWidget* tab_widget);
	void CreateSearchTab(Wt::WTabWidget* tab_widget);
	void CreateAdministratorTab(Wt::WTabWidget* tab_widget);
	void UpdateTabVisibility();

public:
	void ActivateTab(const TabType& tab_type);
	void OnLoggedIn();
	void OnLoggedOut();
	void OnRegisterNewProfile();

private:
	WebApplication* m_app;
	bool            m_initialized;

	Wt::WTabWidget*     m_tab_widget;
	ProfileTab*         m_profile_tab;
	RegisterProfileTab* m_register_profile_tab;
	MessageBoardTab*    m_messageboard_tab;
	SearchTab*          m_search_tab;
	AdministratorTab*   m_administrator_tab;
};

} // namespace modellbasen

#endif // _MAIN_WIDGET_H_
