#include "TriangleView.h"
#include "GlyphManager.h"


FXDEFMAP(TriangleView) TriangleViewMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_CONFIGURE,         0, TriangleView::onConfigure),
//	FXMAPFUNC(SEL_PAINT,             DraggableView::ID_VIEW, WheelView::onPaint),
};

FXIMPLEMENT(TriangleView, DraggableView, TriangleViewMessageMap, ARRAYNUMBER(TriangleViewMessageMap))

const FXint SIDEFLAG_VERTICAL = 0x1;
const FXint SIDEFLAG_HORIZONTAL = 0x2;

TriangleView::TriangleView(FXComposite* p, FXint x, FXint y, FXint w, FXint h, right_angle_t right_angle)
: DraggableView(p, x, y, w, h)
, side_flag_(0)
, right_angle_(right_angle)
{
    onConfigure(this, 0, NULL);
}

TriangleView::~TriangleView(void)
{
}

long TriangleView::onPaint(FXObject* o, FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	FXDCWindow dc(this,ev);
	dc.setForeground(getBackColor());
	dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
	dc.setForeground(drawColor);
	dc.drawLines(vertex_, 4);
	dc.setFont(glyph_manager_->getFont());
	dc.drawText(10, 100, "s");
	return 1;
}

DraggableView::hotspot_t TriangleView::hotSpot (FXint x, FXint y, FXbool down, FXDefaultCursor& cursor)
{
	if (down) {
		pivot_x_ = x;
		pivot_y_ = y;
	}
	FXint dist = distance(x, y, vertex_[0].x, vertex_[0].y);
	if (dist < MOUSE_SENSITIVITY) { // move by anywhere in upper edge
		cursor = DEF_SWATCH_CURSOR;
		return HS_MOVE;
	}

	cursor = DEF_ARROW_CURSOR;
	side_flag_ = 0;
	dist = distance(x, y, vertex_[1].x, vertex_[1].y);
	if (dist < MOUSE_SENSITIVITY) {
		side_flag_ = SIDEFLAG_HORIZONTAL;
	}
	dist = distance(x, y, vertex_[2].x, vertex_[2].y);
	if (dist < MOUSE_SENSITIVITY) {
		side_flag_ = SIDEFLAG_VERTICAL;
	}
	if (side_flag_)
        cursor = DEF_CROSSHAIR_CURSOR;
	return side_flag_ ? HS_RESIZE : HS_NONE;
}

void TriangleView::dragResize (FXint x, FXint y)
{
	FXint dx = x - pivot_x_, dy = y - pivot_y_;
	if (distance(x, y, vertex_[0].x, vertex_[0].y) < MOUSE_SENSITIVITY * 2)
        return;
    switch (right_angle_) {
        case RIGHT_ANGLE_NW:
            if (side_flag_ == SIDEFLAG_HORIZONTAL)
                setWidth(x);
            else
                setHeight(y);
            break;
        case RIGHT_ANGLE_NE:
            if (side_flag_ == SIDEFLAG_HORIZONTAL) {
                setX(getX() + x);
                setWidth(getWidth() - x);
            }
            else
                setHeight(y);
            break;
        case RIGHT_ANGLE_SE:
            break;
        case RIGHT_ANGLE_SW:
            break;
    }
/*	FXint xx = getX(), yy = getY();
	FXint ww = (side_flag_ & SIDEFLAG_VERTICAL) ? x : getWidth();
	FXint hh = (side_flag_ & SIDEFLAG_HORIZONTAL) ? y : getHeight();
	if (!is_right_resize_) {
		xx += dx;
		if (side_flag_ & SIDEFLAG_VERTICAL)
			ww = getWidth() - dx;
	}
	if (ww > MOUSE_SENSITIVITY && hh > MOUSE_SENSITIVITY)
		position (xx, yy, ww, hh);*/
}

void TriangleView::dragMove (FXint x, FXint y)
{
	setX (getX() + x - pivot_x_);
	setY (getY() + y - pivot_y_);
}

long TriangleView::onConfigure(FXObject*, FXSelector, void*)
{
    FXint w = getWidth() - 1, h = getHeight() - 1;
    // [0] - right angle, [1] - horizontal, [2] - vertical
    switch (right_angle_) {
        case RIGHT_ANGLE_NW:
            vertex_[0].set(0, 0);
            vertex_[1].set(w, 0);
            vertex_[2].set(0, h);
            break;
        case RIGHT_ANGLE_NE:
            vertex_[0].set(w, 0);
            vertex_[1].set(0, 0);
            vertex_[2].set(w, h);
            break;
        case RIGHT_ANGLE_SE:
            vertex_[0].set(w, h);
            vertex_[1].set(0, h);
            vertex_[2].set(w, 0);
            break;
        case RIGHT_ANGLE_SW:
            vertex_[0].set(0, h);
            vertex_[1].set(w, h);
            vertex_[2].set(0, 0);
            break;
    }
    vertex_[3] = vertex_[0];
	return 0;
}
