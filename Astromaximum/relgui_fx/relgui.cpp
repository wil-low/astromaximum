#include <fx.h>
#include "relgui.h"
FXIMPLEMENT(Relgui, FXMainWindow, RelguiMap, ARRAYNUMBER(RelguiMap));

Relgui::Relgui(FXApp *a): FXMainWindow(a,"Relgui",NULL,NULL,DECOR_ALL,0,0,800,600){
    FXHorizontalFrame *fr00=new FXHorizontalFrame(this,
        LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 

        // 1st column
        FXVerticalFrame *fr01=new FXVerticalFrame(fr00,
            LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 

        FXMatrix *fr02=new FXMatrix(fr00, 2,
            MATRIX_BY_COLUMNS|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 

            // year
            FXTextField *txtYear=new FXTextField(fr02, 5, NULL, 0,
                TEXTFIELD_NORMAL|TEXTFIELD_INTEGER|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y); 
            // set year
            FXButton *bYear=new FXButton(fr02, "Set year", NULL, 0,
                BUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y); 
            // year
            FXListBox *lbLang=new FXListBox(fr02, NULL, 0); 
            // set year
            FXButton *bDemo=new FXButton(fr02, "Demo", NULL, 0,
                BUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y); 

        // 2nd column
        FXVerticalFrame *fr10=new FXVerticalFrame(fr00,
            LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 
        new FXLabel(fr10, "Selected", NULL, LAYOUT_CENTER_X);
        FXList *lb1=new FXList(fr10, NULL, 0,
            LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0); 

        // 3rd column
        FXVerticalFrame *fr20=new FXVerticalFrame(fr00,
            FRAME_SUNKEN|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0); 
        new FXLabel(fr20, "Available", NULL, LAYOUT_CENTER_X);
        FXList *lb2=new FXList(fr20, NULL, 0,
            LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y); 
    
    lb2->appendItem("1", NULL, NULL, FALSE);
    lb2->appendItem("Привет ребята", NULL, NULL, FALSE);
    lb2->getItem(1)->setText("fdgsdrgser");
    txtYear->setText("2008", true);
    lbLang->appendItem("ru");
    lbLang->appendItem("en");
    lbLang->appendItem("uk");
}

void Relgui::create(){
    FXMainWindow::create();
    show();
}

