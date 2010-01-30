#include "RectangleView.h"
#include "GlyphManager.h"

/*
FXDEFMAP(RectangleView) RectangleViewMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_CONFIGURE,         DraggableView::ID_VIEW, WheelView::onConfigure),
//	FXMAPFUNC(SEL_PAINT,             DraggableView::ID_VIEW, WheelView::onPaint),
};
*/
FXIMPLEMENT(RectangleView, DraggableView, 0, 0);//RectangleViewMessageMap, ARRAYNUMBER(RectangleViewMessageMap))

RectangleView::RectangleView(FXComposite* p, FXuint opts, FXint x, FXint y, FXint w, FXint h)
: DraggableView(p, opts, x, y, w, h)
{
}

RectangleView::~RectangleView(void)
{
}

long RectangleView::onPaint(FXObject* o, FXSelector, void* ptr)
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

DraggableView::hotspot_t RectangleView::hotSpot (FXint x, FXint y, FXbool down)
{
	if (down) {
		pivot_x_ = x;
		pivot_y_ = y;
	}
	if (y < MOUSE_SENSITIVITY) // move by anywhere in upper edge
		return HS_MOVE;
	if (getHeight() - y < MOUSE_SENSITIVITY) {
		if (x < MOUSE_SENSITIVITY) {
			is_right_resize_ = false;
			return HS_RESIZE;
		}
		if (getWidth() - x < MOUSE_SENSITIVITY) {
			is_right_resize_ = true;
			return HS_RESIZE;
		}
	}
	return HS_NONE;
}

void RectangleView::dragResize (FXint x, FXint y)
{
	FXint dx = x - pivot_x_, dy = y - pivot_y_;
	FXint xx = getX(), yy = getY(), ww = x, hh = y;
	if (!is_right_resize_) {
		xx += dx; ww = getWidth() - dx;
	}
	if (ww > MOUSE_SENSITIVITY && hh > MOUSE_SENSITIVITY)
		position (xx, yy, ww, hh);
}

void RectangleView::dragMove (FXint x, FXint y)
{
	setX (getX() + x - pivot_x_);
	setY (getY() + y - pivot_y_);
}
