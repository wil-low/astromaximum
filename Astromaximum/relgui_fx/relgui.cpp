#include <fx.h>
#include "relgui.h"

FXIMPLEMENT(Relgui, FXMainWindow, RelguiMap, ARRAYNUMBER(RelguiMap));

FXint sortCities(const FXListItem* li,const FXListItem* ri) {
    return compare(li->getText(), ri->getText());
}

Relgui::Relgui(FXApp *a): FXMainWindow(a,"",NULL,NULL,DECOR_ALL,0,0,1000,600){
    FXHorizontalFrame *fr00=new FXHorizontalFrame(this,
        LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 

        // 1st column
        FXVerticalFrame *fr01=new FXVerticalFrame(fr00,
            LAYOUT_SIDE_TOP|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0); 

            FXMatrix *fr02=new FXMatrix(fr01, 2,
                PACK_UNIFORM_WIDTH|MATRIX_BY_COLUMNS|LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0); 

                // year
                FXTextField *txtYear=new FXTextField(fr02, 5, &yearTarget, FXDataTarget::ID_VALUE,
                    TEXTFIELD_NORMAL|TEXTFIELD_ENTER_ONLY|TEXTFIELD_INTEGER|LAYOUT_SIDE_TOP|LAYOUT_FILL_X); 
                // set year
                FXButton *bYear=new FXButton(fr02, "Set year", NULL, this, ID_YEAR,
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
    
    year=FXDate::localDate().year();
    yearTarget.connect(year);
    yearTarget.setTarget(this);
    yearTarget.setSelector(ID_YEAR);
    
    lbLang->setNumVisible(3);
    lbLang->appendItem("ru");
    lbLang->appendItem("en");
    lbLang->appendItem("uk");
    lbAvailable->setSortFunc(sortCities);
    l0->disable();
    txtTimeshift->setText("-24", true);
    txtTimeshift->disable();
    ckDebug->disable();
    txtFilesize->setEditable(false);
}

void Relgui::clearLr(LocRec &lr){
    for(int i=0; i<lr.no(); i++){
        delete lr[i];
    }
    lr.clear();
}

Relgui::~Relgui(){
    clearLr(lrCities);
}

void Relgui::create(){
    FXMainWindow::create();
    onGetCityList(NULL,0,NULL);
    show();
}

void Relgui::moveCity(FXList* lst, FXuint index){
    FXuint idx2=(FXuint)(lst->getItem(index)->getData());
    lrCities[idx2]->selected=!lrCities[idx2]->selected;
    onResortCityList(NULL, 0, NULL);
}

long Relgui::onListSelected(FXObject* o,FXSelector,void* index){
    moveCity((FXList*)o, (FXuint)index);
}

long Relgui::onListAvailable(FXObject* o,FXSelector,void* index){
    moveCity((FXList*)o, (FXuint)index);
}

long Relgui::onGetCityList(FXObject* o,FXSelector sel,void* data){
    FXString *fileList;
    getApp()->beginWaitCursor();
    setTitle("Relgui - "+FXStringVal(year));
    clearLr(lrCities);
    char buf[1000];
    const FXString dir="../data/archive/";
    int count=FXDir::listFiles(fileList, dir, "*", FXDir::AllFiles|FXDir::NoParent);
    FXRex parser("([^\\|]+?)\\|[\\-\\d\\.]+\\|[\\-\\d\\.]+\\|([^\\|]+?)\\|[A-Z]+", REX_CAPTURE);
    int beg[3], end[3];
    FXString city, state, dpath, ini;
    for(int i=0; i<count; i++){
        if(FXPath::match("*.txt", fileList[i])){
            char *cc=(char*)((dir+fileList[i]).text());
			FILE *intxt=fopen(cc, "r");
			if(!intxt){
				perror("txt open");
				continue;
			}
            ini=fileList[i].trunc(fileList[i].length()-4);
			int ii=0, ind=0;
			while(fgets(buf, 1000, intxt)){
                FXString line(buf);
                line=line.trim();
                ii=line.find('#');
                if(ii>=0) 
                    line.trunc(ii);
                if(parser.match(line, beg, end, REX_FORWARD, 3)){
                    city=line.mid(beg[1], end[1]-beg[1]);
                    ii=city.find('!');
                    if(ii>=0) 
                        city.erase(0, ii+1);
                    state=line.mid(beg[2], end[2]-beg[2]);
                    ii=state.find('$');
                    if(ii>=0) 
                        state.erase(0, ii+1);
                }
                char dat[5];
                sprintf(dat, "%04d", ind++);
                dpath=dir+FXStringVal(year)+"/"+ini+"/Data"+dat+".dat";
                if(FXStat::exists(dpath)){
                    lrCities.append(new City(city, state, dpath));
                }
            }
            fclose(intxt);
        }
    }
    delete[] fileList;
    onResortCityList(NULL,0,NULL);
    getApp()->endWaitCursor();
}

long Relgui::onResortCityList(FXObject* o,FXSelector,void* data){
    lbSelected->clearItems();
    lbAvailable->clearItems();
    for(int i=0; i<lrCities.no(); i++){
        addCityItem(lrCities[i]->selected ? lbSelected: lbAvailable, i);
    }
    lbAvailable->sortItems();
}

void Relgui::addCityItem(FXList *lst, FXuint i) {
    if(sortByState){
        lst->appendItem(lrCities[i]->state + ", " + lrCities[i]->city, NULL, (void*)i);
    }
    else{
        lst->appendItem(lrCities[i]->city + ", " + lrCities[i]->state, NULL, (void*)i);
    }
}
