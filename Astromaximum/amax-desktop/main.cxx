#include "fltk/Window.h"
#include "fltk/Preferences.h"
#include "fltk/run.h"
#include "fltk/MenuBar.h"
#include "fltk/Browser.h"
#include "fltk/Item.h"
#include "fltk/Button.h"
#include "fltk/Font.h"
#include "fltk/SharedImage.h"
#include "fltk/FL_VERSION.h"
#include "fltk/ask.h"
#include "fltk/draw.h"
#include "LibUI.h"
#include <stdio.h>
#include "sqlite3.h"
//#include "MainWindow.h"
using namespace fltk;

SharedImage* sima=0;
class Painter: public Widget
{
public:
	Painter(int x, int y, int w, int h, const char* label=0);
	virtual void draw();
};

void Painter::draw()
{
	setcolor(BLUE);
	fillrect(0,0,w(),h());
	sima->draw(Rectangle(0,0,12,12),Rectangle(0,0,w(),h()));
}

Painter::Painter(int x, int y, int w, int h, const char* label):
	Widget(x,y,w,h,label)
{

}

int main(int argc, char* argv[]){
	//MainWindow mw;
	Font **arrayp;
	int count=list_fonts(arrayp);
	printf("fonts=%d\n",count);
	printf("%s\n",sqlite3_libversion());
	sqlite3 *sqdb;
	for(int i=0; i<count; i++)
		printf("%d - %s\n",i,arrayp[i]->name());
	printf("version %d.%d.%d",FL_MAJOR_VERSION, FL_MINOR_VERSION,FL_PATCH_VERSION);
	fflush(stdout);
	Window w(USEDEFAULT,USEDEFAULT,300,380,"Здравствуйте");
	w.begin();
	sima=SharedImage::get("planet12.gif");
	Symbol* si=(Symbol*)sima;
	si->name("planet");
	Button btn(10,10,100,50);
	btn.labelfont(font("WinStarTT",0));
	btn.labelsize(20);
	btn.labelcolor(WHITE);
	btn.label("\xa2\xa1\xa3");
	w.resizable(&btn);
	Painter pnt(10,80,100,100);
	Browser brw(10,80,200,200);
	brw.begin();
	if(sqlite3_open("pers-utf.sqb",&sqdb)==SQLITE_OK){
		printf("opened\n");
		sqlite3_stmt *pStmt;
		if(sqlite3_prepare(sqdb,"select name from natal limit 20",-1,&pStmt,0)==SQLITE_OK){
			printf("prepared\n");
			while(sqlite3_step(pStmt)==SQLITE_ROW){
				(new Item(0))->copy_label((const char*)sqlite3_column_text(pStmt,0));
				printf("\t%s\n",sqlite3_column_text(pStmt,0));
			}
		}
		
		
	}
	brw.end();
	LibUI ae;
	ae.window->show();
//	w.show(argc, argv);
	//mw.window->show(argc, argv);
	return run();
}
