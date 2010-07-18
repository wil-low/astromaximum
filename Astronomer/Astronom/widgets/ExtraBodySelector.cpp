#include "ExtraBodySelector.h"
#include "../widgets/PlanetListItem.h"
#include "../labels/AstroLabel.h"
#include "../utils/constants.h"
#include "../utils/GlyphManager.h"
#include <vector>
#include <boost/foreach.hpp>

FXDEFMAP(ExtraBodySelector) ExtraBodySelectorMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           astro::ID_FILL_PLANET_LIST,		 ExtraBodySelector::onCmdFillPlanetList),
	FXMAPFUNC(SEL_SELECTED,           ExtraBodySelector::ID_PLANETS,	 ExtraBodySelector::onListSelChanged),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_GET_DEG_MODE, ExtraBodySelector::onCmdGetDegMode),
	FXMAPFUNC(SEL_COMMAND,          ExtraBodySelector::ID_DEGMODE, ExtraBodySelector::onCmdSetDegMode),
//    FXMAPFUNC(SEL_RIGHTBUTTONPRESS,ExtraBodySelector::ID_PLANETS,ExtraBodySelector::onRBtnPress),
    FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,ExtraBodySelector::ID_PLANETS,ExtraBodySelector::onRBtnRelease),
};

FXIMPLEMENT(ExtraBodySelector, FXVerticalFrame, ExtraBodySelectorMessageMap, ARRAYNUMBER(ExtraBodySelectorMessageMap))

const unsigned int PLANET_SELECTOR_COLOR = FXRGB(131, 160, 165);

ExtraBodySelector::ExtraBodySelector (FXComposite* p)
: FXVerticalFrame(p, FRAME_SUNKEN|LAYOUT_FILL|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0,10,10,10,10)
, deg_mode_(dm_Absolute)
{
    setBackColor(PLANET_SELECTOR_COLOR);
/*
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
*/
	lstPlanets = new FXList (this, this, ID_PLANETS, LIST_BROWSESELECT|LAYOUT_FILL|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0);
	lstPlanets->setNumVisible(12);
	for (int i = 0; i < 12; ++i)
		lstPlanets->appendItem("123");
	lstPlanets->setBackColor(getBackColor());
}

ExtraBodySelector::~ExtraBodySelector(void)
{
}

void ExtraBodySelector::create()
{
	FXVerticalFrame::create();
	lstPlanets->setFont(GlyphManager::get_const_instance().getFont(12, FF_ASTRO));
}

long ExtraBodySelector::onCmdFillPlanetList(FXObject* sender, FXSelector sel, void* ptr)
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

long ExtraBodySelector::onListSelChanged(FXObject*, FXSelector, void* ptr)
{
	int idx = int(ptr);
	if (idx != -1) {
		AstroLabel* al = (AstroLabel*)lstPlanets->getItemData(idx);
		if (al)
			selectAstroLabel(al);
	}
	return 1;
}

void ExtraBodySelector::selectAstroLabel(AstroLabel* al)
{
	FXEvent evt;
	evt.win_x = al->getRect().x;
	evt.win_y = al->getRect().y;
    al->getParent()->handle(this, FXSEL(SEL_MOTION, 0), (void*)&evt);
}

long ExtraBodySelector::onCmdGetDegMode(FXObject*, FXSelector, void* ptr)
{
	int idx = tabbar->getCurrent();
	ptr = (void*)idx;
	return 1;
}

long ExtraBodySelector::onCmdSetDegMode(FXObject*, FXSelector, void* ptr)
{
	deg_mode_ = (deg_mode)int(ptr);
	for (int i = 0; i < lstPlanets->getNumItems(); ++i) {
	    dynamic_cast<PlanetListItem*>(lstPlanets->getItem(i))->setDegMode(lstPlanets, deg_mode_);
	}
	lstPlanets->update();
	return 1;
}

// Right button released
long ExtraBodySelector::onRBtnRelease(FXObject* o, FXSelector sel, void* ptr)
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
	mcmd->setFont(GlyphManager::get_const_instance().getFont(12, FF_ASTRO));
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
