#include "AstronomApp.h"
#include "DraggableView.h"
#include "WheelView.h"

// Message Map for the Scribble Window class
FXDEFMAP(Astronom) AstronomMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             Astronom::ID_CANVAS, Astronom::onPaint),
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_ADD, Astronom::onAddView),
};

FXIMPLEMENT(Astronom, FXMainWindow, AstronomMessageMap, ARRAYNUMBER(AstronomMessageMap))

// Construct a Astronom
Astronom::Astronom(FXApp *a)
: FXMainWindow(a,"Astronom",NULL,NULL,DECOR_ALL,0,0,800,600){


	contents=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);


	// LEFT pane to contain the canvas
	canvasFrame=new FXVerticalFrame(contents,FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);

// Label above the canvas
	new FXLabel(canvasFrame,"Canvas Frame",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X); 

	// Horizontal divider line
	new FXHorizontalSeparator(canvasFrame,SEPARATOR_GROOVE|LAYOUT_FILL_X);


	new DraggableView(canvasFrame,LAYOUT_EXPLICIT, 100, 350, 100, 200);

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


Astronom::~Astronom(){
}


// Create and initialize
void Astronom::create(){

	// Create the windows
	FXMainWindow::create();

	// Make the main window appear
	show(PLACEMENT_SCREEN);
}

long Astronom::onAddView(FXObject*, FXSelector, void*)
{
	static int counter = 0;
	DraggableView* dv = NULL;
	switch (counter % 2) {
		case 0:
			dv = new DraggableView(canvasFrame, LAYOUT_EXPLICIT, 100, 35, 100, 100);
			break;
		case 1:
			dv = new WheelView(canvasFrame, LAYOUT_EXPLICIT, 100, 35, 100, 100);
			break;
	}
	dv->create();
	dv->raise();
	++counter;
	return 1;
}