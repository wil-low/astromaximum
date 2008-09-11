#ifndef RELGUI_H

#define RELGUI_H

class LocRec: public FXObject{
public:
    FXString city, state, datapath;
    LocRec(const FXString& ci, const FXString& st, const FXString& da): city(ci), state(st), datapath(da){};
};


class Relgui: public FXMainWindow{
    FXDECLARE(Relgui);
private:
    void moveCity(FXList* dest, FXList* src, FXuint index);
    FXList *lbSelected, *lbAvailable;
    FXObjectListOf<LocRec> cities;
protected:
    Relgui(){};    
public:    
    Relgui(FXApp* a);
    virtual void create();
    
    enum {
        ID_LIST_SELECTED=FXMainWindow::ID_LAST,
        ID_LIST_AVAILABLE,
    };

    // Message handlers
    long onListSelected(FXObject*,FXSelector,void*);
    long onListAvailable(FXObject*,FXSelector,void*);
};

FXDEFMAP(Relgui) RelguiMap[]={
    FXMAPFUNC(SEL_DOUBLECLICKED, Relgui::ID_LIST_SELECTED, Relgui::onListSelected),
    FXMAPFUNC(SEL_DOUBLECLICKED, Relgui::ID_LIST_AVAILABLE, Relgui::onListAvailable),
};

#endif
