/********************************************************************************
*                                                                               *
*                         Scribble  Application coding sample                   *
*                                                                               *
********************************************************************************/
#include "fx.h"

// Main Window
class AstrologerApp : public FXMainWindow {

	// Macro for class hierarchy declarations
	FXDECLARE(AstrologerApp)

private:

	FXHorizontalFrame *contents;                // Content frame
	FXVerticalFrame   *canvasFrame;             // Canvas frame
	FXVerticalFrame   *buttonFrame;             // Button frame
	FXCanvas          *canvas;                  // Canvas to draw into
	FXCanvas          *canvas1;                  // Canvas to draw into
	int                mdflag;                  // Mouse button down?
	int                moveflag;                  // Mouse button down?
	int                resizeflag;                  // Mouse button down?
	int                dirty;                   // Canvas has been painted?
	FXColor            drawColor;               // Color for the line
protected:
	AstrologerApp(){}

public:

	// Message handlers
	long onPaint(FXObject*,FXSelector,void*);
	long onMouseDown(FXObject*,FXSelector,void*);
	long onMouseUp(FXObject*,FXSelector,void*);
	long onMouseMove(FXObject*,FXSelector,void*);
	long onCmdClear(FXObject*,FXSelector,void*);
	long onUpdClear(FXObject*,FXSelector,void*);

public:

	// Messages for our class
	enum{
		ID_CANVAS=FXMainWindow::ID_LAST,
		ID_CLEAR,
		ID_LAST
	};

public:

	// AstrologerApp's constructor
	AstrologerApp(FXApp* a);

	// Initialize
	virtual void create();

	virtual ~AstrologerApp();
};



// Message Map for the Scribble Window class
FXDEFMAP(AstrologerApp) ScribbleWindowMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             AstrologerApp::ID_CANVAS, AstrologerApp::onPaint),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,   AstrologerApp::ID_CANVAS, AstrologerApp::onMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, AstrologerApp::ID_CANVAS, AstrologerApp::onMouseUp),
	FXMAPFUNC(SEL_MOTION,            AstrologerApp::ID_CANVAS, AstrologerApp::onMouseMove),
	FXMAPFUNC(SEL_COMMAND,           AstrologerApp::ID_CLEAR,  AstrologerApp::onCmdClear),
	FXMAPFUNC(SEL_UPDATE,            AstrologerApp::ID_CLEAR,  AstrologerApp::onUpdClear),
};



// Macro for the ScribbleApp class hierarchy implementation
FXIMPLEMENT(AstrologerApp,FXMainWindow,ScribbleWindowMap,ARRAYNUMBER(ScribbleWindowMap))



// Construct a AstrologerApp
AstrologerApp::AstrologerApp(FXApp *a):FXMainWindow(a,"Astronomer",NULL,NULL,DECOR_ALL,0,0,800,600){

	contents=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

	// LEFT pane to contain the canvas
	canvasFrame=new FXVerticalFrame(contents,FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);

	// Label above the canvas
	new FXLabel(canvasFrame,"Canvas Frame",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X); 

	// Horizontal divider line
	new FXHorizontalSeparator(canvasFrame,SEPARATOR_GROOVE|LAYOUT_FILL_X);


	// Drawing canvas
	canvas=new FXCanvas(canvasFrame,this,ID_CANVAS,LAYOUT_EXPLICIT, 100, 350, 100, 200);
	// Drawing canvas
	//    canvas1=new FXCanvas(canvasFrame,this,ID_CANVAS,LAYOUT_EXPLICIT, 10, 35, 200, 100);

	// RIGHT pane for the buttons
	buttonFrame=new FXVerticalFrame(contents,FRAME_SUNKEN|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);

	// Label above the buttons
	new FXLabel(buttonFrame,"Button Frame",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);

	// Horizontal divider line
	new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);

	// Button to clear
	FXButton* btn = new FXButton(buttonFrame,"&Clear",NULL,this,ID_CLEAR,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

	// Exit button
	new FXButton(buttonFrame,"&Exit",NULL,getApp(),FXApp::ID_QUIT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

	// Initialize private variables
	drawColor=FXRGB(255,0,0);
	mdflag=0;
	dirty=0;
	moveflag = 0;
	resizeflag = 0;

}


AstrologerApp::~AstrologerApp(){
}


// Create and initialize
void AstrologerApp::create(){

	// Create the windows
	FXMainWindow::create();

	// Make the main window appear
	show(PLACEMENT_SCREEN);
}



// Mouse button was pressed somewhere
long AstrologerApp::onMouseDown(FXObject* o,FXSelector,void* ptr){
	FXWindow* win =(FXWindow*) o;
	win->grab();

	// While the mouse is down, we'll draw lines
	mdflag=1;
	FXEvent *ev=(FXEvent*)ptr;
	if (ev->win_x < 8 && ev->win_y < 8)
		moveflag=1;
	if (abs(win->getWidth() - ev->win_x < 8) && abs(win->getHeight() - ev->win_y < 8))
		resizeflag = 1;
	return 1;
}



// The mouse has moved, draw a line
long AstrologerApp::onMouseMove(FXObject* o, FXSelector, void* ptr){
	FXEvent *ev=(FXEvent*)ptr;
	FXWindow* win =(FXWindow*) o;
	if (ev->win_x < 8 && ev->win_y < 8)
		win->setDefaultCursor(getApp()->getDefaultCursor(DEF_MOVE_CURSOR));
	else if (abs(win->getWidth() - ev->win_x < 8) && abs(win->getHeight() - ev->win_y < 8))
		win->setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGBR_CURSOR));
	else
		win->setDefaultCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
	if (moveflag) {
		win->setX (win->getX() + ev->win_x);
		win->setY (win->getY() + ev->win_y);
		return 1;
	}

	if (resizeflag) {
		win->resize (ev->win_x, ev->win_y);
		return 1;
	}
	// Draw
	if(mdflag){

		// Get DC for the canvas
		FXDCWindow dc(canvas);

		// Set foreground color
		dc.setForeground(drawColor);

		// Draw line
		dc.drawLine(ev->last_x, ev->last_y, ev->win_x, ev->win_y);

		// We have drawn something, so now the canvas is dirty
		dirty=1;
	}
	return 1;
}



// The mouse button was released again
long AstrologerApp::onMouseUp(FXObject*,FXSelector,void* ptr){
	FXEvent *ev=(FXEvent*) ptr;
	canvas->ungrab();
	if(mdflag){
		FXDCWindow dc(canvas);

		dc.setForeground(drawColor);
		dc.drawLine(ev->last_x, ev->last_y, ev->win_x, ev->win_y);

		// We have drawn something, so now the canvas is dirty
		dirty=1;

		// Mouse no longer down
		mdflag=0;
		moveflag = 0;
		resizeflag = 0;
	}
	return 1;
}


// Paint the canvas
long AstrologerApp::onPaint(FXObject* o,FXSelector,void* ptr){
	FXEvent *ev=(FXEvent*)ptr;
	FXCanvas* canvas = (FXCanvas*)o;
	FXDCWindow dc(canvas,ev);
	dc.setForeground(canvas->getBackColor());
	dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
	dc.setForeground(drawColor);
	dc.drawEllipse(0, 0, canvas->getWidth() - 1, canvas->getHeight() - 1);
	return 1;
}


// Handle the clear message
long AstrologerApp::onCmdClear(FXObject*,FXSelector,void*){
	FXDCWindow dc(canvas);
	dc.setForeground(canvas->getBackColor());
	dc.fillRectangle(0,0,canvas->getWidth(),canvas->getHeight());
	dirty=0;
	return 1;
}



// Update the clear button:- each gui element (widget) in FOX
// receives a message during idle processing asking it to be updated.
// For example, buttons can be sensitized or desensitized when the
// state of the application changes.
// In this case, we desensitize the sender (the clear button) when
// the canvas has already been cleared, and sensitize it when it has
// been painted (as indicated by the dirty flag).
long AstrologerApp::onUpdClear(FXObject* sender,FXSelector,void*){

	if(dirty)
		sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_ENABLE),NULL);
	else
		sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_DISABLE),NULL);

	return 1;
}


// Here we begin
int main(int argc,char *argv[]){

	// Make application
	FXApp application("Scribble","FoxTest");

	// Start app
	application.init(argc,argv);
	// Scribble window
	new AstrologerApp(&application);

	// Create the application's windows
	application.create();

	// Run the application
	return application.run();
}
