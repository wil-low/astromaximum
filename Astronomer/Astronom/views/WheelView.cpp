#include "WheelView.h"
#include "../utils/GlyphManager.h"

FXDEFMAP(WheelView) WheelViewMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_CONFIGURE,         0, WheelView::onConfigure),
//	FXMAPFUNC(SEL_PAINT,             DraggableView::ID_VIEW, WheelView::onPaint),
};

FXIMPLEMENT(WheelView, DraggableView, WheelViewMessageMap, ARRAYNUMBER(WheelViewMessageMap))

WheelView::WheelView(FXComposite* p, FXint x, FXint y, FXint r)
: DraggableView(p, x, y, r * 2, r * 2)
{
	onConfigure(this, 0, NULL);
}

void WheelView::create()
{
	DraggableView::create();
	show();
}

WheelView::~WheelView(void)
{
}

long WheelView::onPaint(FXObject* o, FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	FXDCWindow dc(this,ev);
	dc.setFunction(BLT_SRC_XOR_DST);
	dc.setForeground(getBackColor());
	dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
	dc.setForeground(drawColor);
	dc.drawEllipse(0, 0, getWidth() - 1, getHeight() - 1);
//	dc.setFont(GlyphManager::get_const_instance().getFont());
	dc.drawEllipse(radius_ - 5, radius_ - 5, 10, 10);
	FXString s;
	s.format("%c%c%c%c", 115, 117, 85, 80);
	dc.drawText(30, 30, s);
	return 1;
}

DraggableView::hotspot_t WheelView::hotSpot (FXint x, FXint y, FXbool down, FXDefaultCursor& cursor)
{
	if (down) {
		pivot_x_ = x;
		pivot_y_ = y;
	}
	FXint dist = distance(x, y, radius_, radius_);
	if (dist < MOUSE_SENSITIVITY) {
		cursor = DEF_SWATCH_CURSOR;
		return HS_MOVE;
	}
	else if (abs(dist - radius_)  < MOUSE_SENSITIVITY) {
		cursor = DEF_CROSSHAIR_CURSOR;
		return HS_RESIZE;
	}
	cursor = DEF_ARROW_CURSOR;
	return HS_NONE;
}

void WheelView::dragResize (FXint x, FXint y)
{
    FXint dist = distance(x, y, radius_, radius_);
	if (dist > 2 * MOUSE_SENSITIVITY)
		position (getX() + radius_ - dist, getY() + radius_ - dist, dist * 2, dist * 2);
}

void WheelView::dragMove (FXint x, FXint y)
{
	setX (getX() + x - pivot_x_);
	setY (getY() + y - pivot_y_);
}

long WheelView::onConfigure(FXObject*, FXSelector, void*)
{
	radius_ = getWidth() / 2;
	return 1;
}
