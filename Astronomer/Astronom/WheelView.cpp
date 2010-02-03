#include "WheelView.h"
#include "GlyphManager.h"

FXDEFMAP(WheelView) WheelViewMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_CONFIGURE,         DraggableView::ID_VIEW, WheelView::onConfigure),
//	FXMAPFUNC(SEL_PAINT,             DraggableView::ID_VIEW, WheelView::onPaint),
};

FXIMPLEMENT(WheelView, DraggableView, WheelViewMessageMap, ARRAYNUMBER(WheelViewMessageMap))

WheelView::WheelView(FXComposite* p, FXint x, FXint y, FXint w, FXint h)
: DraggableView(p, x, y, w, h)
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
	FXCanvas* canvas = (FXCanvas*)o;
	FXDCWindow dc(canvas,ev);
	dc.setForeground(canvas->getBackColor());
	dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
	dc.setForeground(drawColor);
	dc.drawEllipse(0, 0, canvas->getWidth() - 1, canvas->getHeight() - 1);
	dc.setFont(GlyphManager::fntAstro);
	dc.drawEllipse(center_x_ - 5, center_y_ - 5, 10, 10);
	dc.drawText(30, 30, FXString().format("%c%c%c%c", 115, 117, 85, 80));
	return 1;
}

DraggableView::hotspot_t WheelView::hotSpot (FXint x, FXint y, FXbool down, FXDefaultCursor& cursor)
{
	if (down) {
		pivot_x_ = x;
		pivot_y_ = y;
	}
	FXint dist = distance(x, y, center_x_, center_y_);
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
    FXint dist = distance(x, y, center_x_, center_y_);
	if (dist > 2 * MOUSE_SENSITIVITY)
		position (getX() + center_x_ - dist, getY() + center_y_ - dist, dist * 2, dist * 2);
}

void WheelView::dragMove (FXint x, FXint y)
{
	setX (getX() + x - pivot_x_);
	setY (getY() + y - pivot_y_);
}

long WheelView::onConfigure(FXObject*, FXSelector, void*)
{
	center_x_ = getWidth() / 2;
	center_y_ = getHeight() / 2;
	radius_ = center_x_ < center_y_ ? center_x_ : center_y_;
	return 0;
}
