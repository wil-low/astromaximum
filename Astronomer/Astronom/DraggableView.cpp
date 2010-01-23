#include "DraggableView.h"
#include "GlyphManager.h"

// Message Map for the Scribble Window class
FXDEFMAP(DraggableView) DraggableViewMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             DraggableView::ID_VIEW, DraggableView::onPaint),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,   DraggableView::ID_VIEW, DraggableView::onMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, DraggableView::ID_VIEW, DraggableView::onMouseUp),
	FXMAPFUNC(SEL_MOTION,            DraggableView::ID_VIEW, DraggableView::onMouseMove),
};

FXIMPLEMENT(DraggableView, FXCanvas, DraggableViewMessageMap, ARRAYNUMBER(DraggableViewMessageMap))

DraggableView::DraggableView(FXComposite* p, FXuint opts, FXint x, FXint y, FXint w, FXint h)
: FXCanvas(p, this, ID_VIEW, opts, x, y, w, h)
, mouse_flag_(MF_NONE)
{
	drawColor=FXRGB(255,0,0);
}

DraggableView::~DraggableView(void)
{
}

long DraggableView::onPaint(FXObject* o, FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	FXCanvas* canvas = (FXCanvas*)o;
	FXDCWindow dc(canvas,ev);
	dc.setForeground(canvas->getBackColor());
	dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
	dc.setForeground(drawColor);
	dc.drawRoundRectangle(0, 0, canvas->getWidth() - 1, canvas->getHeight() - 1, 15, 15);
	dc.setFont(GlyphManager::fntAstro);
	dc.drawText(10, 100, "sjafjamMIi,ozqtr");
	return 1;
}

// Mouse button was pressed somewhere
long DraggableView::onMouseDown(FXObject* o,FXSelector,void* ptr){
	FXWindow* win =(FXWindow*) o;
	win->grab();
	win->raise();

	FXEvent *ev=(FXEvent*)ptr;
	mouse_flag_ = MF_DOWN;
	if (ev->win_x < 8 && ev->win_y < 8)
		mouse_flag_ = MF_MOVE;
	else if (abs(win->getWidth() - ev->win_x < 8) && abs(win->getHeight() - ev->win_y < 8))
		mouse_flag_ = MF_RESIZE;
	return 1;
}

// The mouse has moved, draw a line
long DraggableView::onMouseMove(FXObject* o, FXSelector, void* ptr){
	FXEvent *ev=(FXEvent*)ptr;
	FXWindow* win =(FXWindow*) o;
	if (ev->win_x < 8 && ev->win_y < 8)
		win->setDefaultCursor(getApp()->getDefaultCursor(DEF_MOVE_CURSOR));
	else if (abs(win->getWidth() - ev->win_x < 8) && abs(win->getHeight() - ev->win_y < 8))
		win->setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGBR_CURSOR));
	else
		win->setDefaultCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));

	if (mouse_flag_ == MF_MOVE) {
		win->setX (win->getX() + ev->win_x);
		win->setY (win->getY() + ev->win_y);
	}

	if (mouse_flag_ == MF_RESIZE) {
		win->resize (ev->win_x, ev->win_y);
	}
	return 1;
}


// The mouse button was released again
long DraggableView::onMouseUp(FXObject*,FXSelector,void* ptr){
	FXEvent *ev=(FXEvent*) ptr;
	ungrab();
	mouse_flag_ = MF_NONE;
	return 1;
}