#include <sqlite3.h>
#include "Astronom.h"

int main(int argc,char *argv[])
{
	sqlite3 *db;
	int rc = sqlite3_open("test.sqb", &db);
	sqlite3_close(db);
	// Make application
	Astronom application("Astronom","S&W Axis");

	// Start app
	application.reg().setAsciiMode(true);
	application.init(argc,argv);
	application.create();

	// Run the application
	return application.run();
}
