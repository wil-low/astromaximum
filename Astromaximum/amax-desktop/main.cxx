#include "fltk/Window.h"
#include "fltk/run.h"
#include "fltk/Browser.h"
#include "fltk/Button.h"
#include <stdio.h>
using namespace fltk;

int main(int argc, char* argv[]){
  Window w(USEDEFAULT, USEDEFAULT, 300, 380, "Здравствуйте");
  w.begin();
  Button btn(10, 10, 100, 50);
//	btn.labelfont(font("WinStarTT",0));
//	btn.labelsize(20);
  btn.labelcolor(WHITE);
  w.resizable(&btn);
  Browser* browser=new Browser(10,60,100,100);

  // CLEAR BROWSER
  browser->clear();

  // ADD LINES TO BROWSER
  browser->add("AAA");		// fltk does strdup() internally       
  browser->add("BBB");
  browser->add("CCC");

  // FORMAT CHARACTERS: CHANGING TEXT COLORS IN LINES
  //    Warning: format chars are returned to you via ::text()
  //    @C# - text color             @b  - bold
  //    @B# - background color       @i  - italic
  //    @F# - set font number        @f  - fixed pitch font
  //    @S# - set point size         @u  - underline
  //    @.  - terminate '@' parser   @-  - strikeout
  //
Widget* elem = browser->find( "BBB" );
printf("elem: %s\n",elem->label());
browser->item(elem);
browser->select_only_this();
 
//  ae.window->show();
//	w.show(argc, argv);
  w.end();
  w.show(argc, argv);
  return run();
}
