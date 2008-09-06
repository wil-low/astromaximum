#ifndef RELGUI_H

#define RELGUI_H

class Relgui: public FXMainWindow
{
    FXDECLARE(FXMainWindow);
public:    
    Relgui(FXApp *a);
    virtual void create();
protected:
    Relgui(){};    
};

FXDEFMAP(Relgui) RelguiMap[]={
    
};

#endif
