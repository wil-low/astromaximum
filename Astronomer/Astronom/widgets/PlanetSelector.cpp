#include "PlanetSelector.h"
#include "../widgets/PlanetListItem.h"
#include "../labels/AstroLabel.h"
#include "../utils/constants.h"
#include "../utils/GlyphManager.h"
#include <vector>
#include <boost/foreach.hpp>

FXDEFMAP(PlanetSelector) PlanetSelectorMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           astro::ID_FILL_PLANET_LIST,		 PlanetSelector::onCmdFillPlanetList),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_GET_DEG_MODE, PlanetSelector::onCmdGetDegMode),
	FXMAPFUNC(SEL_COMMAND,          PlanetSelector::ID_DEGMODE, PlanetSelector::onCmdSetDegMode),
//    FXMAPFUNC(SEL_RIGHTBUTTONPRESS,PlanetSelector::ID_PLANETS,PlanetSelector::onRBtnPress),
    FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,PlanetSelector::ID_PLANETS,PlanetSelector::onListRBtnRelease),
    FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,PlanetSelector::ID_EXTRA,PlanetSelector::onListRBtnRelease),
	FXMAPFUNC(SEL_SELECTED,           PlanetSelector::ID_PLANETS,	 PlanetSelector::onListSelChanged),
	FXMAPFUNC(SEL_SELECTED,           PlanetSelector::ID_EXTRA,	 PlanetSelector::onListSelChanged),
};

FXIMPLEMENT(PlanetSelector, FXVerticalFrame, PlanetSelectorMessageMap, ARRAYNUMBER(PlanetSelectorMessageMap))

const unsigned int PLANET_SELECTOR_COLOR = FXRGB(131, 160, 165);

PlanetSelector::PlanetSelector (FXComposite* p)
: FXVerticalFrame(p, FRAME_SUNKEN|LAYOUT_FILL|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0,10,10,10,10)
, deg_mode_(dm_Absolute)
{
    setBackColor(PLANET_SELECTOR_COLOR);
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

    FXVerticalFrame* frame = new FXVerticalFrame(this, FRAME_LINE|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0,2,2,2,2);
    frame->setBackColor(getBackColor());
	lstPlanets = new FXList (frame, this, ID_PLANETS, LIST_BROWSESELECT|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0);
	lstPlanets->setNumVisible(12);
	lstPlanets->setBackColor(getBackColor());
	lstPlanets->horizontalScrollBar()->setBackColor(getBackColor());
	lstPlanets->verticalScrollBar()->setBackColor(getBackColor());

    frame = new FXVerticalFrame(this, FRAME_LINE|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0,2,2,2,2);
    frame->setBackColor(getBackColor());
	lstExtra = new FXList (frame, this, ID_EXTRA, LIST_BROWSESELECT|LAYOUT_FILL|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0);
	lstExtra->setNumVisible(12);
	lstExtra->setBackColor(getBackColor());
	lstExtra->horizontalScrollBar()->setBackColor(getBackColor());
	lstExtra->verticalScrollBar()->setBackColor(getBackColor());
}

PlanetSelector::~PlanetSelector(void)
{
}

void PlanetSelector::create()
{
	FXVerticalFrame::create();

	int deg_mode = getApp()->reg().readIntEntry("PlanetSelector", "degree_mode", 0);
	tabbar->setCurrent(deg_mode, true);

	lstPlanets->setFont(GlyphManager::get_const_instance().getFont(12, FF_ASTRO));
	lstExtra->setFont(GlyphManager::get_const_instance().getFont(12, FF_ASTRO));
}

long PlanetSelector::onCmdFillPlanetList(FXObject* sender, FXSelector sel, void* ptr)
{
	std::vector<AstroLabel*> *planets = (std::vector<AstroLabel*>*)ptr;
	lstPlanets->clearItems();
	lstExtra->clearItems();
	BOOST_FOREACH(AstroLabel* al, *planets) {
	    if (al->getType() == TYPE_ZODIAC)
            continue;
	    bool is_extra = (al->getType() == TYPE_PLANET && al->getId() > LAST_PLANET_ID);
		PlanetListItem *pli = new PlanetListItem(al, is_extra);
		pli->setDegMode(lstPlanets, deg_mode_);
		al->setVisible(!is_extra);
		(is_extra ? lstExtra : lstPlanets)->appendItem(pli);
	}
	return 1;
}

long PlanetSelector::onListSelChanged(FXObject* o, FXSelector, void* ptr)
{
    FXList* list = dynamic_cast<FXList*>(o);
    int idx = int(ptr);
	if (idx != -1) {
		AstroLabel* al = (AstroLabel*)list->getItemData(idx);
		selectAstroLabel(al);
	}
	return 1;
}

void PlanetSelector::selectAstroLabel(AstroLabel* al)
{
    al->getParent()->handle(this, FXSEL(SEL_COMMAND, astro::ID_SELECT_LABEL), (void*)al);
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
	getApp()->reg().writeIntEntry("PlanetSelector", "degree_mode", deg_mode_);
	getApp()->reg().write();
	setDegMode(lstPlanets);
	setDegMode(lstExtra);
	return 1;
}

void PlanetSelector::setDegMode(FXList* list)
{
	for (int i = 0; i < list->getNumItems(); ++i) {
	    dynamic_cast<PlanetListItem*>(list->getItem(i))->setDegMode(list, deg_mode_);
	}
	list->update();
}
// Right button released
long PlanetSelector::onListRBtnRelease(FXObject* o, FXSelector sel, void* ptr)
{
    FXEvent *event=(FXEvent*)ptr;
    FXList* list = dynamic_cast<FXList*>(o);
    ungrab();
    int idx = list->getItemAt (event->win_x, event->win_y);
    if (idx == -1)
        return 0;

    list->setCurrentItem (idx, true);
    AstroLabel* al = (AstroLabel*)(list->getItem(idx)->getData());
    al->setVisible(!al->isVisible());
    list->update();
    al->getParent()->handle(this, FXSEL(SEL_COMMAND, astro::ID_REORDER_LABELS), NULL);
    return 1;
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
