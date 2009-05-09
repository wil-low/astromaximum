#include <fx.h>
#include "relgui.h"

int main(int argc, char *argv[]){
    FXApp* application=new FXApp("Scribble","Test");

    // Start app
    application->init(argc,argv);
    // Scribble window
    new Relgui(application);

    // Create the application's windows
    application->create();

    // Run the application
    application->run();
    return 0;
}
