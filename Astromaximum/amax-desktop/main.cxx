#include "fltk/Window.h"
#include "fltk/Preferences.h"
#include "fltk/run.h"
#include "fltk/MenuBar.h"
#include "fltk/Browser.h"
#include "fltk/Item.h"
#include "fltk/Button.h"
#include "fltk/Font.h"
#include "fltk/SharedImage.h"
#include "fltk/file_chooser.h"
#include "fltk/FL_VERSION.h"
#include "fltk/ask.h"
#include "fltk/draw.h"
#include "LibUI.h"
#include <stdio.h>
#include "sqlite3.h"
//#include "MainWindow.h"
using namespace fltk;

SharedImage* sima=0;
class Painter: public Widget {
public:
  Painter(int x, int y, int w, int h, const char* label=0);
  virtual void draw();
};

void Painter::draw() {
  setcolor(BLUE);
  fillrect(0, 0, w(), h());
  sima->draw(Rectangle(0, 0, 12, 12), Rectangle(0, 0, w(), h()));
}

Painter::Painter(int x, int y, int w, int h, const char* label):
  Widget(x, y, w, h, label) {
    
}
  
int main(int argc, char* argv[]){
  //MainWindow mw;
  Font **arrayp;
  int count=list_fonts(arrayp);
  printf("fonts=%d\n", count);
  printf("%s\n", sqlite3_libversion());
  sqlite3 *sqdb;
  for(int i=0; i<count; i++)
    printf("%d - %s\n", i, arrayp[i]->name());
  printf("version %d.%d.%d", FL_MAJOR_VERSION, FL_MINOR_VERSION, FL_PATCH_VERSION);
  fflush(stdout);
  Window w(USEDEFAULT, USEDEFAULT, 300, 380, "Здравствуйте");
  w.begin();
  sima=SharedImage::get("planet12.gif");
  Symbol* si=(Symbol*)sima;
  si->name("planet");
  Button btn(10, 10, 100, 50);
//	btn.labelfont(font("WinStarTT",0));
//	btn.labelsize(20);
  btn.labelcolor(WHITE);
  btn.label("\xa2\xa1\xa3@planet");
  w.resizable(&btn);
  file_chooser("", "*", "init");
  Painter pnt(10, 80, 100, 100);
  LibUI ae;
  const int widths[]   = { 100, 100, 100, 0 };
  ae.lwLib->column_widths(widths);
  const char* widths1[]   = { "100", "100", "100",0 };
  ae.lwLib->column_labels(widths1);
  if(sqlite3_open("/home/willow/pers-utf.sqb", &sqdb)==SQLITE_OK){
    printf("opened\n");
    sqlite3_stmt *pStmt;
    if(sqlite3_prepare(sqdb, "select name from natal limit 20", -1, &pStmt, 0)==SQLITE_OK){
      printf("prepared\n");
      while(sqlite3_step(pStmt)==SQLITE_ROW){
	ae.lwLib->add((const char*)sqlite3_column_text(pStmt, 0), 0);
//		(new Item(0))->copy_label((const char*)sqlite3_column_text(pStmt,0));
//		printf("\t%s\n",sqlite3_column_text(pStmt,0));
      }
    }


  }
  
  Browser* browser=ae.lbTopic;

  // CLEAR BROWSER
  browser->clear();

  // ADD LINES TO BROWSER
  browser->add("One");		// fltk does strdup() internally       
  browser->add("Two");
  browser->add("Three");

  // FORMAT CHARACTERS: CHANGING TEXT COLORS IN LINES
  //    Warning: format chars are returned to you via ::text()
  //    @C# - text color             @b  - bold
  //    @B# - background color       @i  - italic
  //    @F# - set font number        @f  - fixed pitch font
  //    @S# - set point size         @u  - underline
  //    @.  - terminate '@' parser   @-  - strikeout
  //
  ae.lwLib->add("Blackt@C1Red\t@C2Green\t@C3Yellow");


 
  ae.window->show();
//	w.show(argc, argv);
  //mw.window->show(argc, argv);
  return run();
}
