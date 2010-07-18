#include "MainForm.h"
#include "../Astronom.h"
#include "../views/RectangleView.h"
#include "../views/TriangleView.h"
#include "../views/WheelView.h"
#include "../views/OcularView.h"
#include "../widgets/PlanetSelector.h"
#include "../utils/constants.h"
#include "../utils/GlyphManager.h"

#include <boost/foreach.hpp>
#include <fxkeys.h>

FXDEFMAP(MainForm) MainFormMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
//	FXMAPFUNC(SEL_COMMAND,           MainForm::ID_ADD,       MainForm::onAddView),
	FXMAPFUNC(SEL_PAINT,             MainForm::ID_CANVAS,    MainForm::onPaint),
	FXMAPFUNC(SEL_COMMAND,           MainForm::ID_LOCK,		 MainForm::onCmdLock),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_FILL_PLANET_LIST,		 MainForm::onCmdFillPlanetList),
};

FXIMPLEMENT(MainForm, FXMainWindow, MainFormMessageMap, ARRAYNUMBER(MainFormMessageMap))

// Construct a MainForm
MainForm::MainForm(FXApp *a)
: FXMainWindow(a,"Astronom",NULL,NULL,DECOR_ALL,0,0,800,600)
{
    setTarget(a);
	FXMenuBar* menubar = new FXMenuBar(this, LAYOUT_TOP|LAYOUT_FILL_X);
	// File menu
	filemenu=new FXMenuPane(menubar);
	new FXMenuTitle(menubar, tr("&File"), NULL, filemenu);
	new FXMenuCommand(filemenu, tr("&Input data..."), NULL, getApp(), Astronom::ID_INPUTDATA);
	new FXMenuCommand(filemenu, tr("&Glyph Manager...\tCtrl-G\tGlyph Manager"), NULL, getApp(), Astronom::ID_GLYPH);
	new FXMenuCommand(filemenu, tr("Toggle C&hrono...\tF3\tToggle Chrono"), NULL, getApp(), Astronom::ID_CHRONO);

	new FXMenuCommand(menubar, tr("&Persons..."), NULL, getApp(), Astronom::ID_PERSONS);

	housemenu=new FXMenuPane(menubar);
	new FXMenuTitle(menubar, tr("&Houses"), NULL, housemenu);
	new FXMenuRadio(housemenu, tr("&Koch\tK"), getApp(), Astronom::ID_HOUSE);
	new FXMenuRadio(housemenu, tr("&Gaqueline\tG"), getApp(), Astronom::ID_HOUSE);

	if (getAccelTable()) {
		getAccelTable()->addAccel (MKUINT(KEY_G,CONTROLMASK), getApp(), FXSEL(SEL_COMMAND, Astronom::ID_GLYPH));
		getAccelTable()->addAccel (MKUINT(KEY_F3, 0), getApp(), FXSEL(SEL_COMMAND, Astronom::ID_CHRONO));
	}

	new FXStatusBar(this, LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
	splitter = new FXSplitter(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y|SPLITTER_REVERSED|SPLITTER_TRACKING); 

	contents=new FXHorizontalFrame(splitter,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

	// LEFT pane to contain the canvas
	canvasFrame=new FXVerticalFrame(contents, LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0, 0,0,0,0);
	canvasFrame->setBackColor(FXRGB(255,255,255));

	// RIGHT pane for the buttons
	planetSelector = new PlanetSelector(splitter);

//    btnLock = new FXCheckButton(buttonFrame,"&Lock", this, ID_LOCK,CHECKBUTTON_NORMAL,0,0,0,0,10,10,5,5);

//    btnLock->setCheck();
}

MainForm::~MainForm()
{
	delete filemenu;
	delete housemenu;
}

// Create and initialize
void MainForm::create()
{
	// Create the windows
	FXMainWindow::create();
	filemenu->create();
	housemenu->create();
}

void MainForm::init()
{
	onAddView(0, 0, 0);
}

long MainForm::onCmdLock(FXObject*, FXSelector, void* ptr)
{
    FXWindow* child = canvasFrame->getFirst();
    while (child) {
        FXTRACE((10, "onCmdLock-child %X\n", (int)child));
        child->handle(child, FXSEL(SEL_COMMAND, DraggableView::ID_LOCK), ptr);
        child = child->getNext();
    }
    return 1;
}

long MainForm::onAddView(FXObject*, FXSelector, void*)
{
	dv = new OcularView(canvasFrame, 10, 10, 377);
	((Astronom*)getApp())->setOcular(dv);
	dv->create();
	dv->handle(dv, FXSEL(SEL_COMMAND, DraggableView::ID_LOCK), (void*)0);//btnLock->getCheck());
	dv->raise();
/*
	dv = new RectangleView(canvasFrame, 100, 35, 100, 100);
	dv->setGlyphManager(((Astronom*)getApp())->fGlyphManager);
	dv->create();
	dv->handle(dv, FXSEL(SEL_COMMAND, DraggableView::ID_LOCK), (void*)btnLock->getCheck());
	dv->raise();*/
	return 1;
}

long MainForm::onCmdFillPlanetList(FXObject* sender, FXSelector sel, void* ptr)
{
	return planetSelector->handle(sender, sel, ptr);
}
