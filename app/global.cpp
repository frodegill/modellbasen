#ifdef USE_PCH
# include "../pch.h"
#else
# include <boost/locale.hpp>
# include <Wt/WString>
#endif

#include "application.h"
#include "defines.h"
#include "global.h"


// Logging
Wt::WLogger g_logger;
Wt::WLogEntry Log(const std::string& type)
{
	return g_logger.entry(type);
}


// Database
modellbasen::PocoGlue DB;


// Application
Wt::WApplication* createApplication(const Wt::WEnvironment& env)
{
	modellbasen::WebApplication* app = new modellbasen::WebApplication(env);
	if (!app->Initialize())
	{
		delete app;
		app = NULL;
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
