#ifndef RELGUI_H

#define RELGUI_H

class Relgui: public FXMainWindow{
    FXDECLARE(Relgui);
private:
    void moveCity(FXList* dest, FXList* src, FXuint index);
    FXList *lbSelected, *lbAvailable;
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
