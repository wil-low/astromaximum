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
	FXMAPFUNC(SEL_CLICKED,           PlanetSelector::ID_PLANETS,	 PlanetSelector::onClickedPlanetList),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_GET_DEG_MODE, PlanetSelector::onCmdGetDegMode),
	FXMAPFUNC(SEL_COMMAND,          PlanetSelector::ID_DEGMODE, PlanetSelector::onCmdSetDegMode),
};

FXIMPLEMENT(PlanetSelector, FXVerticalFrame, PlanetSelectorMessageMap, ARRAYNUMBER(PlanetSelectorMessageMap))

PlanetSelector::PlanetSelector (FXComposite* p, GlyphManager* gm)
: FXVerticalFrame(p, FRAME_SUNKEN|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0,10,10,10,10)
, gm_(gm)
{
    tabbar = new FXTabBar(this, this, ID_DEGMODE, TABBOOK_NORMAL);
    FXTabItem* item = new FXTabItem(tabbar, tr("Abs.\tAbsolute"));
    item = new FXTabItem(tabbar, tr("Ecl.\tLongitude"));
    item = new FXTabItem(tabbar, tr("R.A.\tRectascension"));
    item = new FXTabItem(tabbar, tr("Ob.A.\tOblique ascension"));
    item = new FXTabItem(tabbar, tr("L/D\tLatitude/Declination"));

	lstPlanets = new FXList (this, this, ID_PLANETS, LIST_BROWSESELECT|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0);
	lstPlanets->setNumVisible(10);
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
		pli->setDegMode(deg_mode_);
		lstPlanets->appendItem(pli);
	}
	return 1;
}

long PlanetSelector::onClickedPlanetList(FXObject*, FXSelector, void*)
{
	int idx = lstPlanets->getCurrentItem();
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
	return 1;
}
