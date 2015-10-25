
#ifndef _MAIN_WIDGET_H_
#define _MAIN_WIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WTabWidget>
#include <Wt/WVBoxLayout>


namespace modellbasen
{

class WebApplication;

class MainWidget : public Wt::WContainerWidget
{
public:
	MainWidget(WebApplication* app);
	~MainWidget();

private:
	void Initialize();
	void AddHeader(Wt::WVBoxLayout* layout);
	void CreateProfileTab(Wt::WTabWidget* tab_widget);
	void CreateMessageBoardTab(Wt::WTabWidget* tab_widget);
	void CreateSearchTab(Wt::WTabWidget* tab_widget);
	void CreateAdministratorTab(Wt::WTabWidget* tab_widget);

public:
	void ActivateMainWidget();

	void LogOut();

private:

private:
	WebApplication* m_app;
	bool            m_initialized;

	Wt::WTabWidget*           m_tab_widget;
	Wt::WContainerWidget*     m_profile_tab;
	Wt::WContainerWidget*     m_messageboard_tab;
	Wt::WContainerWidget*     m_search_tab;
	Wt::WContainerWidget*     m_administrator_tab;
};

} // namespace modellbasen

#endif // _MAIN_WIDGET_H_
