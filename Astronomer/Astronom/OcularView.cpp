#include "OcularView.h"
#include "GlyphManager.h"
#include "PlanetLabel.h"
#include "ZodiacLabel.h"
#include "Chart.h"
#include "constants.h"

FXDEFMAP(OcularView) WheelViewMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_MOTION,            0, OcularView::onMouseMove),
//	FXMAPFUNC(SEL_QUERY_TIP,         0, OcularView::onQueryTip),
	FXMAPFUNC(SEL_CONFIGURE,         0, OcularView::onConfigure),
	FXMAPFUNC(SEL_RIGHTBUTTONPRESS,   0, OcularView::onRightBtnPress),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_SET_ZERO,     OcularView::onCmdSetZero),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_SET_OCULAR_DIM,     OcularView::onCmdSetDimensions),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_SET_OCULAR_COLOR,  OcularView::onCmdSetColors),
	FXMAPFUNC(SEL_COMMAND,           astro::ID_UPDATE_CHART,  OcularView::onCmdUpdateChart),
};

FXIMPLEMENT(OcularView, WheelView, WheelViewMessageMap, ARRAYNUMBER(WheelViewMessageMap))

const int TICK_10_SIZE = 8;
const int TICK_5_SIZE = 5;

OcularView::OcularView(FXComposite* p, FXint x, FXint y, FXint r)
: WheelView(p, x, y, r)
, zero_point_(ZERO_ARIES)
, zero_angle_(180)
, cur_label_(NULL)
{
	popup_ = new FXPopup(this);
    FXMenuCommand* cmd = new FXMenuCommand(popup_, "Press 10\"12'");
//	cmd->setFont(fntAstro);
	new FXMenuCommand(popup_, "Me");
	new FXMenuCommand(popup_, "Twice");
}

OcularView::~OcularView(void)
{
}

void OcularView::create()
{
	FXString label_text;
	for (int i = 0; i < ZODIAC_SIGN_COUNT; ++i) {
		label_text.format("%c", glyph_manager_->getSignLabel(i));
		ZodiacLabel* label = new ZodiacLabel(i, this, -100, -100, 20, 20);
		label->setText(label_text, glyph_manager_->getFont());
		labels_.push_back(label);
	}
    WheelView::create();
    reorderLabels();
}

long OcularView::onPaint(FXObject* o, FXSelector, void* ptr)
{
	FXEvent *ev=(FXEvent*)ptr;
	FXDCWindow dc(this,ev);
    dc.setBackground(getBackColor());
	dc.setForeground(getBackColor());
	dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
	dc.setForeground(drawColor);
//	dc.drawEllipse(0, 0, getWidth() - 1, getHeight() - 1);
	dc.setFont(glyph_manager_->getFont());
	drawCircle(dc, 5);

	dc.setForeground(colors_.arrowColor);
	FXPoint pt[2];
	pt[0] = getCenter();
	pt[1] = getXYdeg(zero_angle_, radius_);
	dc.drawLines(pt, 2);

	double r = radius_ / DENOMINATOR;
	double ang = zero_angle_;
	double delta_ang = DEG_PER_SIGN;
	if (dimensions_.ascArrowR != 0) {
		dc.setForeground(colors_.ocularColor);
		drawCircle(dc, dimensions_.ascArrowR * r);
	}
	if (dimensions_.zodiacOuterR != 0) {
		dc.setForeground(colors_.mainLineColor);
		ang = zero_angle_;
		delta_ang = DEG_PER_SIGN;
		dc.setForeground(colors_.fillColor);
		for (int sign = 0; sign < 6; ++sign) {
			fillArc(dc, dimensions_.zodiacOuterR * r, ang + delta_ang, delta_ang);
			ang += delta_ang * 2;
		}
		dc.setForeground(colors_.contourColor);
		drawCircle(dc, dimensions_.zodiacOuterR * r);
	}
	if (dimensions_.zodiac10dgrR != 0) {
		dc.setForeground(getBackColor());
		fillCircle(dc, dimensions_.zodiac10dgrR * r);
		dc.setForeground(colors_.mainLineColor);
		drawCircle(dc, dimensions_.zodiac10dgrR * r);
	}
	if (dimensions_.zodiac5dgrR != 0) {
		dc.setForeground(colors_.mainLineColor);
		drawCircle(dc, dimensions_.zodiac5dgrR * r);
	}
	if (dimensions_.zodiac30dgrR != 0) {
		dc.setForeground(colors_.mainLineColor);
		drawCircle(dc, dimensions_.zodiac30dgrR * r);
	}
	if (dimensions_.zodiacInnerR != 0) {
		dc.setForeground(colors_.mainLineColor);
		drawCircle(dc, dimensions_.zodiacInnerR * r);
	}
/*
	if (dimensions_.aspectR != 0) {
		dc.setForeground(colors_.mainLineColor);
		drawCircle(dc, dimensions_.aspectR * r);
	}
	*/
	ang = zero_angle_ * DTOR;
	delta_ang = 5 * DTOR;
	double zinner = dimensions_.zodiac5dgrR * r - 1;
	double zouter;
	for (int tick = 0; tick < 360 / 5; ++tick) {
		pt[0] = getXYrad(ang, zinner);
		if (tick % 6 == 0) { // solid line - sign
			dc.setLineWidth(2);
			pt[1] = getXYrad(ang, zinner + TICK_10_SIZE);
		}
		else {
			dc.setLineWidth(1);
			pt[1] = getXYrad(ang, zinner + TICK_5_SIZE);
		}
		dc.drawLines(pt, 2);
		ang += delta_ang;
	}
	dc.setForeground(colors_.tick10Color);
	ang = zero_angle_ * DTOR;
	delta_ang = 10 * DTOR;
	zinner = dimensions_.zodiac10dgrR * r - 1;
	zouter = dimensions_.zodiacOuterR * r - 1;
	for (int tick = 0; tick < 360 / 10; ++tick) {
		pt[0] = getXYrad(ang, zinner);
		pt[1] = getXYrad(ang, zouter);
		dc.drawLines(pt, 2);
		ang += delta_ang;
	}
/*
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
*/
/*
	zouter = dimensions_.zodiacInnerR * r;
	zinner = dimensions_.aspectR * r;
	dc.setForeground(colors_.aspectTickColor);
	dc.setLineWidth(2);
	for (int i = 0; i < planet_list_.size(); ++i) {
		ang = planet_list_[i]->getAngle() + zero_angle_;
		pt[0] = getXYdeg(ang, zouter);
		pt[1] = getXYdeg(ang, zinner);
		dc.drawLines(pt, 2);
	}
*/
	drawLabels (dc, labels_);
	return 1;
}

long OcularView::onConfigure(FXObject* o, FXSelector sel, void* ptr)
{
    WheelView::onConfigure(o, sel, ptr);
    dimensions_.radius = radius_;
	reorderLabels();
	return 0;
}

long OcularView::onRightBtnPress(FXObject* o, FXSelector sel, void* ptr)
{

}

void OcularView::reorderLabels()
{
    double rad[AstroLabel::TYPE_LAST];
	rad[AstroLabel::TYPE_ZODIAC] = (dimensions_.zodiac10dgrR + dimensions_.zodiac5dgrR) / 2 * radius_ / DENOMINATOR;
	rad[AstroLabel::TYPE_PLANET] = dimensions_.innerPlanetLabelR * radius_ / DENOMINATOR;
    FXPoint pt;
	for (int i = 0; i < labels_.size(); ++i) {
	    pt = getXYdeg(zero_angle_ + labels_[i]->getAngle(), rad[labels_[i]->getType()]);
		labels_[i]->position(pt.x, pt.y);
	}
}

void OcularView::fillArc (FXDC& dc, int radius, int ang1, int ang2)
{
	dc.fillArc(radius_ - radius, radius_ - radius, radius * 2, radius * 2, ang1 * 64, ang2 * 64);
}

void OcularView::fillCircle (FXDC& dc, int radius)
{
	dc.fillEllipse(radius_ - radius, radius_ - radius, radius * 2, radius * 2);
}

void OcularView::drawCircle (FXDC& dc, int radius)
{
	dc.drawEllipse(radius_ - radius, radius_ - radius, radius * 2, radius * 2);
}

void OcularView::drawCircle (FXDC& dc, int radius, int x, int y)
{
	dc.drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
}

long OcularView::onCmdSetZero(FXObject*, FXSelector, void* ptr)
{
	zero_point_ = (int)ptr;
	return 1;
}

FXPoint OcularView::getXYrad(double radian, double len)
{
	return FXPoint(radius_ + len * cos(radian) + 0.5, radius_ - len * sin(radian) + 0.5);
};

FXPoint OcularView::getXYdeg(double degree, double len)
{
	return FXPoint(radius_ + len * cos(degree * DTOR) + 0.5, radius_ - len * sin(degree * DTOR) + 0.5);
};

FXPoint OcularView::getCenter()
{
	return FXPoint(radius_, radius_);
};

long OcularView::onCmdSetDimensions(FXObject*, FXSelector, void* ptr)
{
	dimensions_ = *((OcularDimensions*)ptr);
	return 1;
}

long OcularView::onCmdSetColors(FXObject*, FXSelector, void* ptr)
{
	colors_ = *((OcularColors*)ptr);
	return 1;
}

long OcularView::onCmdUpdateChart(FXObject*, FXSelector, void* ptr)
{
//	ChartList* cl = (ChartList*)ptr;
	Chart* chart = (Chart*)ptr;
	labels_.clear();
	FXString label_text;
	double radius = dimensions_.innerPlanetLabelR * radius_ / DENOMINATOR;
	for (BodyPropsMap::const_iterator it = chart->bodies_.begin(); it != chart->bodies_.end(); ++it) {
		label_text.format("%c", glyph_manager_->getPlanetLabel((*it).first));
		PlanetLabel* label = new PlanetLabel((*it).second.prop[BodyProps::bp_Lon], this, -100, -100, 20, 20);
		label->setText(label_text, glyph_manager_->getFont());
		labels_.push_back(label);
	}
	reorderLabels();
	return 1;
}

void OcularView::drawLabels (FXDC& dc, const AstroLabelVector& ar)
{
    for (int i = 0; i < ar.size(); ++i) {
        ar[i]->handle(this, FXSEL(SEL_PAINT, 0), &dc);
    }
}

long OcularView::onMouseMove(FXObject* o, FXSelector sel, void* ptr)
{
    if (DraggableView::onMouseMove(o, sel, ptr))
        return 1;
    FXEvent *ev=(FXEvent*)ptr;
    AstroLabel* old_cur = cur_label_;
    cur_label_ = NULL;
    for (int i = 0; i < labels_.size(); ++i) {
        if (labels_[i]->contains(ev->win_x, ev->win_y)) {
            cur_label_ = labels_[i];
            break;
        }
    }
    if (cur_label_ != old_cur) {
       	FXDCWindow dc(this);
       	dc.setBackground(getBackColor());
        if (old_cur) {
            old_cur->handle(this, FXSEL(SEL_COMMAND, AstroLabel::ID_SELECT), (void*)0);
            old_cur->handle(this, FXSEL(SEL_PAINT, AstroLabel::ID_FOCUS), &dc);
        }
        if (cur_label_) {
            cur_label_->handle(this, FXSEL(SEL_COMMAND, AstroLabel::ID_SELECT), (void*)1);
            cur_label_->handle(this, FXSEL(SEL_PAINT, AstroLabel::ID_FOCUS), &dc);
        }
//        getApp()->handle (this, FXSEL(SEL_QUERY_TIP, 0), 0);
    }
    return 1;
}

long OcularView::onQueryTip(FXObject* sender, FXSelector, void*)
{
    if (cur_label_){}
    FXString tip("Hello");
    sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&tip);
}
