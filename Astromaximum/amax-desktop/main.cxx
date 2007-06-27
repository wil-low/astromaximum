#include "fltk/Window.h"
#include "fltk/Preferences.h"
#include "fltk/run.h"
#include "fltk/MenuBar.h"
#include "fltk/ask.h"
#include "MainWindow.h"
using namespace fltk;

int main(int argc, char* argv[]){
	MainWindow mw;
	mw.window->show(argc, argv);
	return run();
}
