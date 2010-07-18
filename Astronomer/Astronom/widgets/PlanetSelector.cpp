#include "PlanetSelector.h"
#include "../forms/GlyphManager.h"
#include "../widgets/PlanetListItem.h"
#include "../labels/AstroLabel.h"
#include "../utils/constants.h"
#include <vector>
#include <boost/foreach.hpp>

FXDEFMAP(PlanetSelector) PlanetSelectorMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           astro::ID_FILL_PLANET_LIST,		 PlanetSelector::onCmdFillPlanetList),
	FXMAPFUNC(SEL_SELECTED,           PlanetSelector::ID_PLANETS,	 PlanetSelector::onListSelChanged),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_GET_DEG_MODE, PlanetSelector::onCmdGetDegMode),
	FXMAPFUNC(SEL_COMMAND,          PlanetSelector::ID_DEGMODE, PlanetSelector::onCmdSetDegMode),
//    FXMAPFUNC(SEL_RIGHTBUTTONPRESS,PlanetSelector::ID_PLANETS,PlanetSelector::onRBtnPress),
    FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,PlanetSelector::ID_PLANETS,PlanetSelector::onRBtnRelease),
};

FXIMPLEMENT(PlanetSelector, FXVerticalFrame, PlanetSelectorMessageMap, ARRAYNUMBER(PlanetSelectorMessageMap))

PlanetSelector::PlanetSelector (FXComposite* p, GlyphManager* gm)
: FXVerticalFrame(p, FRAME_SUNKEN|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0,10,10,10,10)
, gm_(gm)
, deg_mode_(dm_Absolute)
{
    setBackColor(FXRGB(0, 255, 0));
    tabbar = new FXTabBar(this, this, ID_DEGMODE, TABBOOK_NORMAL);
	tabbar->setBackColor(getBackColor());
    FXTabItem* item = new FXTabItem(tabbar, FXString("Abs.\t") + tr("Absolute"));
	item->setBackColor(getBackColor());
    item = new FXTabItem(tabbar, FXString("Ecl.\t") + tr("Longitude"));
	item->setBackColor(getBackColor());
    item = new FXTabItem(tabbar, FXString("R.A.\t") + tr("Rectascension"));
	item->setBackColor(getBackColor());
    item = new FXTabItem(tabbar, FXString("Ob.A.\t") + tr("Oblique ascension"));
	item->setBackColor(getBackColor());
    item = new FXTabItem(tabbar, FXString("L/D\t") + tr("Latitude/Declination"));
	item->setBackColor(getBackColor());

	lstPlanets = new FXList (this, this, ID_PLANETS, LIST_BROWSESELECT|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0);
	lstPlanets->setNumVisible(10);
	lstPlanets->setBackColor(getBackColor());
}

PlanetSelector::~PlanetSelector(void)
{
}

void PlanetSelector::create()
{
	FXVerticalFrame::create();
	lstPlanets->setFont(gm_->getFont(12));
}

long PlanetSelector::onCmdFillPlanetList(FXObject* sender, FXSelector sel, void* ptr)
{
	std::vector<AstroLabel*> *planets = (std::vector<AstroLabel*>*)ptr;
	lstPlanets->clearItems();
	BOOST_FOREACH(AstroLabel* al, *planets) {
		PlanetListItem *pli = new PlanetListItem(al);
		pli->setDegMode(lstPlanets, deg_mode_);
		lstPlanets->appendItem(pli);
	}
	return 1;
}

long PlanetSelector::onListSelChanged(FXObject*, FXSelector, void* ptr)
{
	int idx = int(ptr);
	if (idx != -1) {
		AstroLabel* al = (AstroLabel*)lstPlanets->getItemData(idx);
		selectAstroLabel(al);
	}
	return 1;
}

void PlanetSelector::selectAstroLabel(AstroLabel* al)
{
	FXEvent evt;
	evt.win_x = al->getRect().x;
	evt.win_y = al->getRect().y;
    al->getParent()->handle(this, FXSEL(SEL_MOTION, 0), (void*)&evt);
}

long PlanetSelector::onCmdGetDegMode(FXObject*, FXSelector, void* ptr)
{
	int idx = tabbar->getCurrent();
	ptr = (void*)idx;
	return 1;
}

long PlanetSelector::onCmdSetDegMode(FXObject*, FXSelector, void* ptr)
{
	deg_mode_ = (deg_mode)int(ptr);
	for (int i = 0; i < lstPlanets->getNumItems(); ++i) {
	    dynamic_cast<PlanetListItem*>(lstPlanets->getItem(i))->setDegMode(lstPlanets, deg_mode_);
	}
	lstPlanets->update();
	return 1;
}

// Right button released
long PlanetSelector::onRBtnRelease(FXObject* o, FXSelector sel, void* ptr)
{
    FXEvent *event=(FXEvent*)ptr;
    ungrab();
    int idx = lstPlanets->getItemAt (event->win_x, event->win_y);
    if (idx == -1)
        return 0;
    lstPlanets->setCurrentItem (idx, true);
//    obj->handle(this, FXSEL(SEL_LEFTBUTTONRELEASE, 0), ptr);
//    flags&=~FLAG_PRESSED;
//    if(event->moved) return 1;
    FXMenuPane filemenu(this);
    new FXMenuCaption(&filemenu,"ShutterBug");
    new FXMenuSeparator(&filemenu);
    FXMenuCommand* mcmd = new FXMenuCommand(&filemenu,tr("Snap..."),NULL,this,0);
	mcmd->setFont(gm_->getFont(12));
    new FXMenuCommand(&filemenu,tr("Snap delayed..."),NULL,this,0);
    new FXMenuCommand(&filemenu,tr("Snap to clipboard..."),NULL,this,0);
    new FXMenuCommand(&filemenu,tr("Record movie..."),NULL,this,0);
    new FXMenuCheck(&filemenu,tr("Show lasso"),this,0);
    new FXMenuCheck(&filemenu,tr("Lines inside"),this,0);
    new FXMenuCommand(&filemenu,tr("Color..."),NULL,this,0);
    filemenu.create();
    filemenu.popup(NULL,event->root_x,event->root_y);
    getApp()->runModalWhileShown(&filemenu);
    return 1;
}
