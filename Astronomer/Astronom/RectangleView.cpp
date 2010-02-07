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

const FXint SIDEFLAG_VERTICAL = 0x1;
const FXint SIDEFLAG_HORIZONTAL = 0x2;

RectangleView::RectangleView(FXComposite* p, FXint x, FXint y, FXint w, FXint h)
: DraggableView(p, x, y, w, h)
, side_flag_(0)
{
//	new FXFrame(this, FRAME_NORMAL, 100, 100, 20, 30);
}

RectangleView::~RectangleView(void)
{
}

long RectangleView::onPaint(FXObject* o, FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	FXDCWindow dc(this,ev);
	dc.setForeground(getBackColor());
	dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
	dc.setForeground(drawColor);
	dc.drawRoundRectangle(0, 0, getWidth() - 1, getHeight() - 1, 15, 15);
	dc.setFont(GlyphManager::fntAstro);
	dc.drawText(10, 100, "sjafjamMIi,ozqtr");
	return 1;
}

DraggableView::hotspot_t RectangleView::hotSpot (FXint x, FXint y, FXbool down, FXDefaultCursor& cursor)
{
	if (down) {
		pivot_x_ = x;
		pivot_y_ = y;
	}
	if (y < MOUSE_SENSITIVITY) { // move by anywhere in upper edge
		cursor = DEF_SWATCH_CURSOR;
		return HS_MOVE;
	}

	cursor = DEF_ARROW_CURSOR;
	side_flag_ = 0;
	if (x < MOUSE_SENSITIVITY) {
		is_right_resize_ = false;
		side_flag_ |= SIDEFLAG_VERTICAL;
	}
	if (getWidth() - x < MOUSE_SENSITIVITY) {
		is_right_resize_ = true;
		side_flag_ |= SIDEFLAG_VERTICAL;
	}
	if (getHeight() - y < MOUSE_SENSITIVITY) {
		side_flag_ |= SIDEFLAG_HORIZONTAL;
	}
	switch (side_flag_) {
		case SIDEFLAG_VERTICAL:
			cursor = DEF_DRAGV_CURSOR;
			break;
		case SIDEFLAG_HORIZONTAL:
			cursor = DEF_DRAGH_CURSOR;
			break;
		case SIDEFLAG_VERTICAL|SIDEFLAG_HORIZONTAL:
			cursor = is_right_resize_ ? DEF_DRAGBR_CURSOR : DEF_DRAGBL_CURSOR;
			break;
	}
	return side_flag_ ? HS_RESIZE : HS_NONE;
}

void RectangleView::dragResize (FXint x, FXint y)
{
	FXint dx = x - pivot_x_, dy = y - pivot_y_;
	FXint xx = getX(), yy = getY();
	FXint ww = (side_flag_ & SIDEFLAG_VERTICAL) ? x : getWidth();
	FXint hh = (side_flag_ & SIDEFLAG_HORIZONTAL) ? y : getHeight();
	if (!is_right_resize_) {
		xx += dx;
		if (side_flag_ & SIDEFLAG_VERTICAL)
			ww = getWidth() - dx;
	}
	if (ww > MOUSE_SENSITIVITY && hh > MOUSE_SENSITIVITY)
		position (xx, yy, ww, hh);
}

void RectangleView::dragMove (FXint x, FXint y)
{
	setX (getX() + x - pivot_x_);
	setY (getY() + y - pivot_y_);
}
