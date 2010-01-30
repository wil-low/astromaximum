#include "MainForm.h"
#include "RectangleView.h"
#include "WheelView.h"
#include "Astronom.h"

FXDEFMAP(MainForm) MainFormMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             MainForm::ID_CANVAS,    MainForm::onPaint),
	FXMAPFUNC(SEL_COMMAND,           MainForm::ID_ADD,       MainForm::onAddView),
};

FXIMPLEMENT(MainForm, FXMainWindow, MainFormMessageMap, ARRAYNUMBER(MainFormMessageMap))

// Construct a MainForm
MainForm::MainForm(FXApp *a)
: FXMainWindow(a,"MainForm",NULL,NULL,DECOR_ALL,0,0,800,600)
{
	FXVerticalFrame* vframe=new FXVerticalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
	
	FXMenuBar* menubar = new FXMenuBar(vframe, LAYOUT_TOP|LAYOUT_FILL_X);
	// File menu
	filemenu=new FXMenuPane(menubar);
	new FXMenuTitle(menubar, tr("&File"), NULL, filemenu);
	new FXMenuCommand(filemenu, tr("&Glyph Manager...\tCtl-G\tGlyph Manager"), NULL, getApp(), Astronom::ID_GLYPH);
	
	contents=new FXHorizontalFrame(vframe,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

	// LEFT pane to contain the canvas
	canvasFrame=new FXVerticalFrame(contents, LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0, 0,0,0,0);
	canvasFrame->setBackColor(FXRGB(255,255,255));

	new RectangleView(canvasFrame, LAYOUT_EXPLICIT, 100, 350, 100, 200);

	// RIGHT pane for the buttons
	buttonFrame=new FXVerticalFrame(contents,FRAME_SUNKEN|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);

	// Label above the buttons
	new FXLabel(buttonFrame,"Button Frame",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);

	// Horizontal divider line
	new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);

	// Button to clear
	FXButton* btn = new FXButton(buttonFrame,"&Add",NULL,this,ID_ADD,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

	// Exit button
	new FXButton(buttonFrame,"&Exit",NULL,getApp(),FXApp::ID_QUIT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
}

MainForm::~MainForm()
{
	delete filemenu;
}

// Create and initialize
void MainForm::create(){

	// Create the windows
	FXMainWindow::create();
	filemenu->create();
	// Make the main window appear
	show(PLACEMENT_SCREEN);
}

long MainForm::onAddView(FXObject*, FXSelector, void*)
{
	static int counter = 0;
	DraggableView* dv = NULL;
	switch (counter % 2) {
		case 1:
			dv = new RectangleView(canvasFrame, LAYOUT_EXPLICIT, 100, 35, 100, 100);
			break;
		case 0:
			dv = new WheelView(canvasFrame, LAYOUT_EXPLICIT, 100, 35, 100, 100);
			break;
	}
	dv->create();
	dv->raise();
	++counter;
	return 1;
}
