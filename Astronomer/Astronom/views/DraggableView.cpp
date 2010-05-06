#include "DraggableView.h"

FXDEFMAP(DraggableView) DraggableViewMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             0, DraggableView::onPaint),
	FXMAPFUNC(SEL_MOTION,            0, DraggableView::onMouseMove),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,   0, DraggableView::onMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, DraggableView::onMouseUp),
	FXMAPFUNC(SEL_COMMAND,           DraggableView::ID_LOCK, DraggableView::onCmdLock),
};

FXIMPLEMENT(DraggableView, FXCanvas, DraggableViewMessageMap, ARRAYNUMBER(DraggableViewMessageMap))

const FXint DraggableView::MOUSE_SENSITIVITY = 8;

DraggableView::DraggableView(FXComposite* p, FXint x, FXint y, FXint w, FXint h)
: FXCanvas(p, NULL, 0, LAYOUT_EXPLICIT, x, y, w, h)
, mouse_flag_(HS_NONE)
, is_locked_(true)
, pivot_x_(0)
, pivot_y_(0)
{
	drawColor=FXRGB(255,0,0);
	setBackColor(FXRGBA(255,255,255, 0));
}

DraggableView::~DraggableView(void)
{
}

void DraggableView::setGlyphManager(GlyphManager* gm)
{
	glyph_manager_ = gm;
}

long DraggableView::onPaint(FXObject*, FXSelector, void*)
{
	return 1;
}

long DraggableView::onMouseDown(FXObject*,FXSelector,void* ptr)
{
	if (is_locked_)
        return 0;
	grab();
	raise();
	FXEvent *ev=(FXEvent*)ptr;
	FXDefaultCursor cursor;
	mouse_flag_ = hotSpot (ev->win_x, ev->win_y, true, cursor);
	return 1;
}

long DraggableView::onMouseMove(FXObject* o, FXSelector, void* ptr)
{
	if (is_locked_) {
        mouse_flag_ = HS_NONE;
        setDefaultCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
        setDragCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
        return 0;
	}
	FXEvent *ev=(FXEvent*)ptr;
	if (mouse_flag_ != HS_NONE) {
		// forbid resize and move if cursor is outside parent
		FXint tox, toy;
		translateCoordinatesTo(tox, toy, getParent(), ev->win_x, ev->win_y);
		if (getParent()->contains(tox, toy) == false)
			return 0;
	}
	switch (mouse_flag_) {
		case HS_MOVE:
			dragMove(ev->win_x, ev->win_y);
			break;
		case HS_RESIZE:
			dragResize (ev->win_x, ev->win_y);
			break;
		default:
			FXDefaultCursor cursor_id = DEF_ARROW_CURSOR;
			hotSpot (ev->win_x, ev->win_y, false, cursor_id);
			setDefaultCursor(getApp()->getDefaultCursor(cursor_id));
			setDragCursor(getApp()->getDefaultCursor(cursor_id));
			return 0;
	}
	return 1;
}

long DraggableView::onMouseUp(FXObject*, FXSelector, void*)
{
	if (is_locked_)
        return 0;
	ungrab();
	mouse_flag_ = HS_NONE;
	return 1;
}

DraggableView::hotspot_t DraggableView::hotSpot (FXint, FXint, FXbool, FXDefaultCursor& cursor)
{
	cursor = DEF_ARROW_CURSOR;
	return HS_NONE;
}

float DraggableView::distance(FXint x1, FXint y1, FXint x2, FXint y2)
{
	FXint dx = x1 - x2;
	FXint dy = y1 - y2;
	return sqrt (float(dx * dx + dy * dy));
}

long DraggableView::onCmdLock(FXObject*, FXSelector, void* ptr)
{
    is_locked_ = ptr != NULL;
    mouse_flag_ = HS_NONE;
    return 0;
}
