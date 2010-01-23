#include <fx.h>
#include <sqlite3.h>
#include "AstronomApp.h"
#include "GlyphManager.h"

int main(int argc,char *argv[])
{
	sqlite3 *db;
	int rc = sqlite3_open("test.sqb", &db);
	// Make application
	FXApp application("Astronom","S&W Axis");

	// Start app
	application.init(argc,argv);

	// Create the application's windows
	Astronom* astro = new Astronom(&application);
	GlyphManager* gm = new GlyphManager(&application);
	astro->fntAstro = gm->fntAstro;
	application.create();
	// Run the application
	return application.run();
}
