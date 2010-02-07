#include "OcularView.h"
#include "GlyphManager.h"
#include "AstroLabel.h"

FXDEFMAP(OcularView) WheelViewMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
//	FXMAPFUNC(SEL_CONFIGURE,         0, WheelView::onConfigure),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_SET_ZERO,     OcularView::onCmdSetZero),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_SET_OCULAR_DIM,     OcularView::onCmdSetDimensions),
};

FXIMPLEMENT(OcularView, WheelView, WheelViewMessageMap, ARRAYNUMBER(WheelViewMessageMap))

const int DEG_PER_SIGN = 30;

const int TICK_10_SIZE = 8;
const int TICK_5_SIZE = 3;

OcularView::OcularView(FXComposite* p, FXint x, FXint y, FXint w, FXint h)
: WheelView(p, x, y, w, h)
, zero_point_(ZERO_ARIES)
, zero_angle_(180)
{
	zodiac_label_ = new AstroLabel*[ZODIAC_SIGN_COUNT];
	FXString zod_label_text;
	for (int i = 0; i < ZODIAC_SIGN_COUNT; ++i) {
		zod_label_text.format("%c", i + 'a');
		zodiac_label_[i] = new AstroLabel(this, zod_label_text, 0, LABEL_NORMAL|LAYOUT_EXPLICIT);
	}
}

OcularView::~OcularView(void)
{
	delete[] zodiac_label_;
}

long OcularView::onPaint(FXObject* o, FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	FXDCWindow dc(this,ev);
//	dc.setFunction(BLT_SRC_XOR_DST);
	dc.setForeground(getBackColor());
	dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
	dc.setForeground(drawColor);
	dc.drawEllipse(0, 0, getWidth() - 1, getHeight() - 1);
	dc.setFont(GlyphManager::fntAstro);
	drawCircle(dc, 5);
	dc.drawText(30, 30, FXString().format("%c%c%c%c", 115, 117, 85, 80));
	FXPoint pt[2];
	pt[0] = getCenter();
	pt[1] = getXYdeg(zero_angle_, radius_);
	dc.drawLines(pt, 2);

	double r = radius_ / 100.0;
	if (dimensions_.ascArrowLen != 0) {
		drawCircle(dc, dimensions_.ascArrowLen * r);
	}
	if (dimensions_.cuspidLen != 0) {
		drawCircle(dc, dimensions_.cuspidLen * r);
	}
	if (dimensions_.homeLen != 0) {
		drawCircle(dc, dimensions_.homeLen * r);
	}
	if (dimensions_.zodiacOuterLen != 0) {
		drawCircle(dc, dimensions_.zodiacOuterLen * r);
	}
	if (dimensions_.zodiacInnerLen != 0) {
		drawCircle(dc, dimensions_.zodiacInnerLen * r);
	}
	// 
	double ang = zero_angle_ * DTOR;
	double delta_ang = 5 * DTOR;
	double zinner = dimensions_.zodiacInnerLen * r;
	double zouter = dimensions_.zodiacOuterLen * r;
	for (int tick = 0; tick < 360 / 5; ++tick) {
		if (tick % 6 == 0) { // solid line - sign
			pt[0] = getXYrad(ang, zinner);
			pt[1] = getXYrad(ang, zouter);
			dc.drawLines(pt, 2);
		}
		else if (tick % 2 == 0) { // 10 degrees
			pt[0] = getXYrad(ang, zinner);
			pt[1] = getXYrad(ang, zinner + TICK_10_SIZE);
			dc.drawLines(pt, 2);
			pt[0] = getXYrad(ang, zouter);
			pt[1] = getXYrad(ang, zouter - TICK_10_SIZE);
			dc.drawLines(pt, 2);
		}
		else { // 10 degrees
			pt[0] = getXYrad(ang, zinner);
			pt[1] = getXYrad(ang, zinner + TICK_5_SIZE);
			dc.drawLines(pt, 2);
			pt[0] = getXYrad(ang, zouter);
			pt[1] = getXYrad(ang, zouter - TICK_5_SIZE);
			dc.drawLines(pt, 2);
		}

		ang += delta_ang;
	}
	return 1;
}

long OcularView::onConfigure(FXObject*, FXSelector, void*)
{
	center_x_ = getWidth() / 2;
	center_y_ = getHeight() / 2;
	radius_ = center_x_ < center_y_ ? center_x_ : center_y_;
	reorderLabels();
	return 0;
}

void OcularView::reorderLabels()
{
	double radius = (dimensions_.zodiacInnerLen + dimensions_.zodiacOuterLen) * radius_ / 100.0;
	for (int i = 0; i < ZODIAC_SIGN_COUNT; ++i) {
		FXPoint pt = getXYdeg(zero_angle_ + DEG_PER_SIGN / 2 + DEG_PER_SIGN * i, radius);
		zodiac_label_[i]->position(pt.x, pt.y, 30, 30);
	}
}

void OcularView::drawCircle (FXDC& dc, int radius)
{
	dc.drawEllipse(center_x_ - radius, center_x_ - radius, radius * 2, radius * 2);
}

void OcularView::drawCircle (FXDC& dc, int radius, int x, int y)
{
	dc.drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
}

long OcularView::onCmdSetZero(FXObject*, FXSelector, void* data)
{
	zero_point_ = (int)data;
	return 1;
}

FXPoint OcularView::getXYrad(double radian, double len)
{
	return FXPoint(center_x_ + len * cos(radian) + 0.5, center_y_ + len * sin(radian) + 0.5);
};

FXPoint OcularView::getXYdeg(double degree, double len)
{
	return FXPoint(center_x_ + len * cos(degree * DTOR) + 0.5, center_y_ + len * sin(degree * DTOR) + 0.5);
};

FXPoint OcularView::getCenter()
{
	return FXPoint(center_x_, center_y_);
};

long OcularView::onCmdSetDimensions(FXObject*, FXSelector, void* data)
{
	dimensions_ = *((OcularDimensions*)data);
	return 1;
}
