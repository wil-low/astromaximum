#ifndef RELGUI_H

#define RELGUI_H

class City: public FXObject 
{
public:    
    FXString city, state, datapath;
    City(const FXString& ci, const FXString& st, const FXString& da){
        city=ci; state=st; datapath=da;
    }
};

typedef FXObjectListOf<City> LocRec;

class Relgui: public FXMainWindow{
    FXDECLARE(Relgui);
private:
    void moveCity(FXList* dest, FXList* src, FXuint index);
    FXList *lbSelected, *lbAvailable;
    LocRec lrSelected, lrAvailable;
    void refillList(FXList *lst, const LocRec &lr);
    void clearLr(LocRec &lr);
    FXchar sortByState;
    FXDataTarget sortByStateTarget;
    FXuint year;
protected:
    Relgui(){};    
    ~Relgui();
public:    
    Relgui(FXApp* a);
    virtual void create();
    
    enum {
        ID_LIST_SELECTED=FXMainWindow::ID_LAST,
        ID_LIST_AVAILABLE,
        ID_RESORT,
    };

    // Message handlers
    long onListSelected(FXObject*,FXSelector,void*);
    long onListAvailable(FXObject*,FXSelector,void*);
    long onGetCityList(FXObject*,FXSelector,void*);
    long onResortCityList(FXObject* o,FXSelector sel,void* data);
};

FXDEFMAP(Relgui) RelguiMap[]={
    FXMAPFUNC(SEL_DOUBLECLICKED, Relgui::ID_LIST_SELECTED, Relgui::onListSelected),
    FXMAPFUNC(SEL_DOUBLECLICKED, Relgui::ID_LIST_AVAILABLE, Relgui::onListAvailable),
    FXMAPFUNC(SEL_COMMAND, Relgui::ID_RESORT, Relgui::onResortCityList),
};

#endif
