#ifndef RELGUI_H

#define RELGUI_H

class City: public FXObject 
{
public:    
    FXString city, state, datapath;
    FXbool selected;
    City(const FXString& ci, const FXString& st, const FXString& da){
        city=ci; state=st; datapath=da; selected=false;
    }
};

typedef FXObjectListOf<City> LocRec;

class Relgui: public FXMainWindow{
    FXDECLARE(Relgui);
private:
    void moveCity(FXList* lst, FXuint index);
    void addCityItem(FXList *lst, FXuint i);
    FXList *lbSelected, *lbAvailable;
    LocRec lrCities;
    void clearLr(LocRec &lr);
    FXchar sortByState;
    FXDataTarget sortByStateTarget;
    FXuint year;
    FXDataTarget yearTarget;
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
        ID_YEAR,
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
    FXMAPFUNC(SEL_COMMAND, Relgui::ID_YEAR, Relgui::onGetCityList),
};

#endif
