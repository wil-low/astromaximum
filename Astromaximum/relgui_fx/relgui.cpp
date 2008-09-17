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
                new FXRadioButton(fr04, "City", &sortByStateTarget, FXDataTarget::ID_OPTION+0,
                        RADIOBUTTON_NORMAL|LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
                new FXRadioButton(fr04, "State", &sortByStateTarget, FXDataTarget::ID_OPTION+1,
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
    
    sortByState=0;
    
    sortByStateTarget.connect(sortByState);
    sortByStateTarget.setTarget(this);
    sortByStateTarget.setSelector(ID_RESORT);
    
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
    onGetCityList(NULL,0,NULL);
}

void Relgui::clearLr(LocRec &lr){
    for(int i=0; i<lr.no(); i++){
        delete lr[i];
    }
    lr.clear();
}

Relgui::~Relgui(){
    clearLr(lrAvailable);
}

void Relgui::create(){
    FXMainWindow::create();
    show();
}

void Relgui::moveCity(FXList* dest, FXList* src, FXuint index){
/*
    CityItem* item=(CityItem*)(src->getItem(index));
    dest->appendItem(new CityItem(item->getCity(), item->getState(), item->getDatapath()));
    src->removeItem(index);
 */
}

long Relgui::onListSelected(FXObject* o,FXSelector,void* index){
    moveCity(lbAvailable, lbSelected, (FXuint)index);
}

long Relgui::onListAvailable(FXObject* o,FXSelector,void* index){
    moveCity(lbSelected, lbAvailable, (FXuint)index);
}

long Relgui::onGetCityList(FXObject* o,FXSelector sel,void* data){
    FXString *fileList;
    clearLr(lrAvailable);
    int count=FXDir::listFiles(fileList, ".", "*", FXDir::AllFiles|FXDir::NoParent);
    for(int i=0; i<count; i++){
        lrAvailable.append(new City(fileList[i], "", ""));
    }
    delete[] fileList;
}

long Relgui::onResortCityList(FXObject* o,FXSelector,void* data){
    refillList(lbAvailable, lrAvailable);
    
}

void Relgui::refillList(FXList *lst, const LocRec &lr) {
    lst->clearItems();
    for(int i=0; i<lr.no(); i++){
        if(sortByState){
            lst->appendItem(lr[i]->state + ", " + lr[i]->city);
        }
        else{
            lst->appendItem(lr[i]->city + ", " + lr[i]->state);
        }
    }
}
