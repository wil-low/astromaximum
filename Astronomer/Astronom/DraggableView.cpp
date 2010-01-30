#include "DraggableView.h"

FXDEFMAP(DraggableView) DraggableViewMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             DraggableView::ID_VIEW, DraggableView::onPaint),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,   DraggableView::ID_VIEW, DraggableView::onMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, DraggableView::ID_VIEW, DraggableView::onMouseUp),
	FXMAPFUNC(SEL_MOTION,            DraggableView::ID_VIEW, DraggableView::onMouseMove),
};

FXIMPLEMENT(DraggableView, FXCanvas, DraggableViewMessageMap, ARRAYNUMBER(DraggableViewMessageMap))

const FXint DraggableView::MOUSE_SENSITIVITY = 8;

DraggableView::DraggableView(FXComposite* p, FXuint opts, FXint x, FXint y, FXint w, FXint h)
: FXCanvas(p, this, ID_VIEW, opts, x, y, w, h)
, mouse_flag_(HS_NONE)
, pivot_x_(0)
, pivot_y_(0)
{
	drawColor=FXRGB(255,0,0);
}

DraggableView::~DraggableView(void)
{
}

long DraggableView::onPaint(FXObject* o, FXSelector, void* ptr)
{
	return 1;
}

long DraggableView::onMouseDown(FXObject* o,FXSelector,void* ptr)
{
	DraggableView* win =(DraggableView*) o;
	win->grab();
	win->raise();
	FXEvent *ev=(FXEvent*)ptr;
	mouse_flag_ = win->hotSpot (ev->win_x, ev->win_y, true);
	return 1;
}

long DraggableView::onMouseMove(FXObject* o, FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	DraggableView* win =(DraggableView*) o;
	if (mouse_flag_ != HS_NONE) {
		// forbid resize and move if cursor is outside parent
		FXint tox, toy;
		win->translateCoordinatesTo(tox, toy, win->getParent(), ev->win_x, ev->win_y);
		if (win->getParent()->contains(tox, toy) == false)
			return 0;
	}
	switch (mouse_flag_) {
		case HS_MOVE:
			win->dragMove(ev->win_x, ev->win_y);
			break;
		case HS_RESIZE:
			win->dragResize (ev->win_x, ev->win_y);
			break;
		default:
			hotspot_t hs = win->hotSpot (ev->win_x, ev->win_y, false);
			FXDefaultCursor cursor_id = DEF_ARROW_CURSOR;
			switch (hs) {
				case HS_MOVE:
					cursor_id = DEF_SWATCH_CURSOR;
					break;
				case HS_RESIZE:
					cursor_id = DEF_CROSSHAIR_CURSOR;
					break;
			}
			win->setDefaultCursor(getApp()->getDefaultCursor(cursor_id));
			win->setDragCursor(getApp()->getDefaultCursor(cursor_id));
			return 0;
	}
	return 1;
}

long DraggableView::onMouseUp(FXObject*,FXSelector,void* ptr)
{
	FXEvent *ev=(FXEvent*) ptr;
	ungrab();
	mouse_flag_ = HS_NONE;
	return 1;
}
