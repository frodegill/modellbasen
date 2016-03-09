#include "main.h"
#include "logging.h"
#include "../app/application.h"
#include <boost/locale.hpp>


Wt::WApplication* createApplication(const Wt::WEnvironment& env)
{
	modellbasen::WebApplication* app = new modellbasen::WebApplication(env);
	if (!app->Initialize())
	{
		delete app;
		app = nullptr;
	}
	return app;
}

int main(int argc, char** argv)
{
	g_logger.addField("message", true);
	Wt::WString::setDefaultEncoding(Wt::UTF8);

	// Create Norwegian locale
	boost::locale::generator gen;
	std::locale loc = gen("nb_NO.UTF-8"); 
	std::locale::global(loc);

	return Wt::WRun(argc, argv, &createApplication);
}
