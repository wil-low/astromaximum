#include "fltk/Window.h"
#include "fltk/Preferences.h"
#include "fltk/run.h"
#include "fltk/MenuBar.h"
#include "fltk/ask.h"
using namespace fltk;

int main(int argc, char* argv[]){
	Preferences prefs(Preferences::USER,"S&W Axis","amax-desktop");
	char *text=0;
	prefs.get("main",text,"no value");
	message("Read '%s'", text);
	prefs.set("main","user data");
	Window w(100,100,200,200,"Привет миру!");
	delete[] text;
	w.begin();
	Widget bb(10,10,80,80, "Я - снежинка");
	MenuBar menu(10,10,10,10,"Menu");
	menu.add("File",0,0);
	w.end();
	w.resizable(&bb);
	w.show(argc, argv);
	const char *defstr="Хорошо вроде"; 
	const char *answer=input("Как у вас делишки?",defstr);
	message("Вы ответили '%s'", answer);
	return run();
}
