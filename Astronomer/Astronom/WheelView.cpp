#include "WheelView.h"
#include "GlyphManager.h"

WheelView::WheelView(FXComposite* p, FXuint opts, FXint x, FXint y, FXint w, FXint h)
: DraggableView(p, opts, x, y, w, h)
{
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
	dc.drawText(30, 30, FXString().format("%c%c%c%c", 115, 117, 85, 80));
	return 1;
}
