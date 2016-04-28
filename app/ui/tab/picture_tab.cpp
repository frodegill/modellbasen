#include <Wt/WVBoxLayout>

#include "picture_tab.h"
#include "../../application.h"


using namespace modellbasen;

PictureTab::PictureTab(WebApplication* app)
: Wt::WContainerWidget(),
  m_app(app)
{
	Wt::WVBoxLayout* tab_container_vbox = new Wt::WVBoxLayout();
	tab_container_vbox->setContentsMargins(0, 9, 0, 0);
	setLayout(tab_container_vbox);
}

PictureTab::~PictureTab()
{
}

void PictureTab::OnLoggedIn()
{
}

void PictureTab::OnLoggedOut()
{
}
