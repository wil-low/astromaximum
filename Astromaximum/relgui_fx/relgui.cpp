#include <fx.h>
#include "relgui.h"
FXIMPLEMENT(Relgui, FXMainWindow, RelguiMap, ARRAYNUMBER(RelguiMap));

Relgui::Relgui(FXApp *a): FXMainWindow(a,"Relgui",NULL,NULL,DECOR_ALL,0,0,800,600){
    FXHorizontalFrame *fr0=new FXHorizontalFrame(this,
        LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 
    FXList *lb0=new FXList(fr0, NULL, 0,
        LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0); 
    lb0->appendItem("1", NULL, NULL, FALSE);
    FXList *lb1=new FXList(fr0, NULL, 0,
        LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0); 
    FXList *lb2=new FXList(fr0, NULL, 0,
        LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0); 
}

void Relgui::create(){
    FXMainWindow::create();
    show();
}

