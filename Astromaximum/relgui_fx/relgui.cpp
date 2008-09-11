#include <fx.h>
#include "relgui.h"

FXIMPLEMENT(Relgui, FXMainWindow, RelguiMap, ARRAYNUMBER(RelguiMap));

Relgui::Relgui(FXApp *a): FXMainWindow(a,"Relgui",NULL,NULL,DECOR_ALL,0,0,800,600){
    FXHorizontalFrame *fr00=new FXHorizontalFrame(this,
        LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 

        // 1st column
        FXVerticalFrame *fr01=new FXVerticalFrame(fr00,
            LAYOUT_SIDE_TOP|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 

            FXMatrix *fr02=new FXMatrix(fr01, 2,
                PACK_UNIFORM_WIDTH|MATRIX_BY_COLUMNS|LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0); 

                // year
                FXTextField *txtYear=new FXTextField(fr02, 5, NULL, 0,
                    TEXTFIELD_NORMAL|TEXTFIELD_INTEGER|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
                // set year
                FXButton *bYear=new FXButton(fr02, "Set year", NULL, 0, 0,
                    BUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
                // year
                FXListBox *lbLang=new FXListBox(fr02, NULL, 0); 
                // set year
                FXButton *bDemo=new FXButton(fr02, "Demo", NULL, 0, 0,
                    BUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 

            FXHorizontalFrame *fr03=new FXHorizontalFrame(fr01,
                LAYOUT_SIDE_TOP,0,0,0,0,0,0,0); 
                new FXLabel(fr03, "IMEI:", NULL);
                FXTextField *txtIMEI=new FXTextField(fr03, 20, NULL, 0,
                    TEXTFIELD_NORMAL|TEXTFIELD_INTEGER|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 

            fr02=new FXMatrix(fr01, 2,
                PACK_UNIFORM_WIDTH|MATRIX_BY_COLUMNS|LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0); 
                FXButton *bIMEI=new FXButton(fr02, "IMEI", NULL, 0, 0,
                    BUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
                FXButton *bIMEIlogger=new FXButton(fr02, "IMEI logger", NULL, 0, 0,
                    BUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
                
                FXLabel *l0=new FXLabel(fr02, "Timeshift, hrs:", NULL, LAYOUT_CENTER_X);
                new FXFrame(fr02,0,0,0,0,0,0,0,0);
                FXTextField *txtTimeshift=new FXTextField(fr02, 8, NULL, 0,
                    TEXTFIELD_NORMAL|TEXTFIELD_INTEGER|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
                FXButton *bTimebomb=new FXButton(fr02, "Time bomb", NULL, 0, 0,
                    BUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
            
                new FXLabel(fr02, "Delta, min:", NULL, LAYOUT_CENTER_X);
                new FXFrame(fr02,0,0,0,0,0,0,0,0);
                FXTextField *txtDelta=new FXTextField(fr02, 8, NULL, 0,
                    TEXTFIELD_NORMAL|TEXTFIELD_INTEGER|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
                FXButton *bTBlogger=new FXButton(fr02, "TB logger", NULL, 0, 0,
                    BUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
                FXCheckButton *ckDebug=new FXCheckButton(fr02, "debug", NULL, 0,
                    CHECKBUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
                FXCheckButton *ckMessjar=new FXCheckButton(fr02, "messjar", NULL, 0,
                    CHECKBUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 

            FXTextField *txtPath=new FXTextField(fr01, 10, NULL, 0,
                TEXTFIELD_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
            new FXButton(fr01, "Load", NULL, 0, 0, BUTTON_NORMAL|LAYOUT_CENTER_X); 
            new FXButton(fr01, "Save", NULL, 0, 0, BUTTON_NORMAL|LAYOUT_CENTER_X); 
            new FXButton(fr01, "Clear", NULL, 0, 0, BUTTON_NORMAL|LAYOUT_CENTER_X); 

            FXTextField *txtFilesize=new FXTextField(fr01, 10, NULL, 0,
                TEXTFIELD_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
            
            new FXLabel(fr01, "Sort by:", NULL, LAYOUT_CENTER_X);

            FXHorizontalFrame *fr04=new FXHorizontalFrame(fr01,
                LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0,0,0,0); 
                new FXRadioButton(fr04, "City", NULL, 0,
                        RADIOBUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
                new FXRadioButton(fr04, "State", NULL, 0,
                        RADIOBUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
            new FXButton(fr01, "Generate", NULL, 0, 0, BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FILL_X); 

        // 2nd column
        FXVerticalFrame *fr10=new FXVerticalFrame(fr00,
            LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0); 
        new FXLabel(fr10, "Selected", NULL, LAYOUT_CENTER_X);
        FXVerticalFrame *fr11=new FXVerticalFrame(fr10,
            FRAME_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 
        
            lbSelected=new FXList(fr11, this, ID_LIST_SELECTED,
                LIST_BROWSESELECT|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0); 

        // 3rd column
        FXVerticalFrame *fr20=new FXVerticalFrame(fr00,
            LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0); 
        new FXLabel(fr20, "Available", NULL, LAYOUT_CENTER_X);
        FXVerticalFrame *fr21=new FXVerticalFrame(fr20,
            FRAME_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 
        
            lbAvailable=new FXList(fr21, this, ID_LIST_AVAILABLE,
                LIST_BROWSESELECT|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0); 
    
    lbAvailable->appendItem("1", NULL, NULL, FALSE);
    lbAvailable->appendItem("Привет ребята", NULL, NULL, FALSE);
    txtYear->setText("2008", true);
    lbLang->setNumVisible(3);
    lbLang->appendItem("ru");
    lbLang->appendItem("en");
    lbLang->appendItem("uk");
    l0->disable();
    txtTimeshift->setText("-24", true);
    txtTimeshift->disable();
    ckDebug->disable();
    txtFilesize->setEditable(false);
    
    FXString *fileList;
    int count=FXDir::listFiles(fileList, ".", "*", FXDir::AllFiles|FXDir::NoParent);
    for(int i=0; i<count; i++){
        lbAvailable->appendItem(fileList[i]);
    }
    FXDate dt(2008,9,11);
    printf("%d\n", dt.getJulian());
    
}

void Relgui::create(){
    FXMainWindow::create();
    show();
}

void Relgui::moveCity(FXList* dest, FXList* src, FXuint index){
    FXListItem* item=src->getItem(index);
    dest->appendItem(new FXListItem(item->getText(), item->getIcon(), item->getData()));
    src->removeItem(index);
}

long Relgui::onListSelected(FXObject* o,FXSelector,void* index){
    moveCity(lbAvailable, lbSelected, (FXuint)index);
}

long Relgui::onListAvailable(FXObject* o,FXSelector,void* index){
    moveCity(lbSelected, lbAvailable, (FXuint)index);
}

