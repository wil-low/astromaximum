#include "OcularView.h"
#include "../labels/PlanetLabel.h"
#include "../labels/ZodiacLabel.h"
#include "../labels/HouseLabel.h"
#include "../Chart.h"
#include "../utils/constants.h"
#include "../utils/GlyphManager.h"
#include "../CircleSpread/CircleSpread.h"
#include <boost/foreach.hpp>
//#include <algorithm>

FXDEFMAP(OcularView) WheelViewMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_MOTION,            0, OcularView::onMouseMove),
	FXMAPFUNC(SEL_QUERY_HELP,        0, OcularView::onQueryHelp),
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
, is_resizing_(false)
{
	popup_ = new FXPopup(this);
    FXMenuCommand* cmd = new FXMenuCommand(popup_, "Press 10\"12'");
//	cmd->setFont(fntAstro);
	new FXMenuCommand(popup_, "Me");
	new FXMenuCommand(popup_, "Twice");
}

OcularView::~OcularView(void)
{
    BOOST_FOREACH (AstroLabel* al, labels_) {
        delete al;
    }
    labels_.clear();
}

void OcularView::create()
{
	FXString label_text;
	for (int i = 0; i < ZODIAC_SIGN_COUNT; ++i) {
		label_text.format("%c", GlyphManager::get_const_instance().getSignLabel(i));
		ZodiacLabel* label = new ZodiacLabel(i, this, -100, -100, 20, 20);
		label->setId(i, label_text);
//		label->setFont(GlyphManager::get_const_instance().getFont(dimensions_.fontSize));
		labels_.insert(label);
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
//	dc.setFont(GlyphManager::get_const_instance().getFont());

	double r = radius_ / DENOMINATOR;
	double ang = zero_angle_;
	double delta_ang = DEG_PER_SIGN;
	if (dimensions_.ascArrowR != 0) {
		dc.setForeground(colors_.ocularColor);
		drawCircle(dc, dimensions_.ascArrowR * r);
	}
	if (dimensions_.zodiacOuterR != 0) {
		if (!is_resizing_) {
			ang = zero_angle_;
			delta_ang = DEG_PER_SIGN;
			dc.setForeground(colors_.fillColor);
			for (int sign = 0; sign < 6; ++sign) {
				fillArc(dc, dimensions_.zodiacOuterR * r, ang + delta_ang, delta_ang);
				ang += delta_ang * 2;
			}
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

	dc.setForeground(colors_.ocularColor);
	drawCircle(dc, dimensions_.ascArrowR * 0.06 * r);

	// Aries line
	dc.setForeground(colors_.arrowColor);
	FXPoint pt[2];
	double r_aries = dimensions_.zodiacOuterR * r + 1;
	pt[1] = getXYdeg(zero_angle_, r_aries);
	pt[0] = getXYdeg(ang - 1, r_aries * 1.02);
	dc.drawLines(pt, 2);
	pt[0] = getXYdeg(ang + 1, r_aries * 1.02);
	dc.drawLines(pt, 2);

	dc.setForeground(colors_.mainLineColor);
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
	drawAspects(dc);
	drawPlanetLines (dc);
	drawHouseLines (dc);
	drawLabels (dc);
	return 1;
}

long OcularView::onConfigure(FXObject* o, FXSelector sel, void* ptr)
{
    WheelView::onConfigure(o, sel, ptr);
	if (dimensions_.radius != radius_) {
		dimensions_.radius = radius_;
		reorderLabels();
	}
	is_resizing_ = false;
	return 0;
}

long OcularView::onRightBtnPress(FXObject* o, FXSelector sel, void* ptr)
{
	return 1;
}

void OcularView::reorderLabels()
{
	int planet_font_size = dimensions_.planetFontSize * radius_ / DENOMINATOR;
	int zodiac_font_size = dimensions_.zodiacFontSize * radius_ / DENOMINATOR;

	BOOST_FOREACH (AstroLabel* al, labels_) {
		int font_size;
		switch (al->getType()) {
			case AstroLabel::TYPE_PLANET:
				font_size = planet_font_size;
				break;
			default:
				font_size = zodiac_font_size;
		}
		al->setFont(GlyphManager::get_const_instance().getFont(font_size, FF_ASTRO));
	}
    double rad[AstroLabel::TYPE_LAST];
	rad[AstroLabel::TYPE_ZODIAC] = (dimensions_.zodiac10dgrR + dimensions_.zodiac5dgrR) / 2 * radius_ / DENOMINATOR;
	rad[AstroLabel::TYPE_PLANET] = dimensions_.innerPlanetLabelR * radius_ / DENOMINATOR;
	rad[AstroLabel::TYPE_HOUSE] = 10000000;
    FXPoint pt;

	spreadLabels(0, AstroLabel::TYPE_PLANET, rad[AstroLabel::TYPE_PLANET]);
	spreadLabels(0, AstroLabel::TYPE_HOUSE, rad[AstroLabel::TYPE_HOUSE]);
	BOOST_FOREACH (AstroLabel* al, labels_) {
		pt = getXYdeg(zero_angle_ + al->getVisibleAngle(), rad[al->getType()]);
		al->position(pt.x, pt.y);
	}
}

void OcularView::fillArc (FXDC& dc, double radius, double ang1, double ang2)
{
	dc.fillArc(radius_ - radius, radius_ - radius, radius * 2, radius * 2, ang1 * 64, ang2 * 64);
}

void OcularView::fillCircle (FXDC& dc, double radius)
{
	dc.fillEllipse(radius_ - radius, radius_ - radius, radius * 2, radius * 2);
}

void OcularView::drawCircle (FXDC& dc, double radius)
{
	dc.drawEllipse(radius_ - radius, radius_ - radius, radius * 2, radius * 2);
}

void OcularView::drawCircle (FXDC& dc, double radius, double x, double y)
{
	dc.drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
}

long OcularView::onCmdSetZero(FXObject*, FXSelector, void* ptr)
{
	zero_point_ = (ZeroPoint)int(ptr);
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

struct less_planet_list {
	bool operator() (const AstroLabel* al1, const AstroLabel* al2) const
	{
		return al1->getIdentity() < al2->getIdentity();
	}
};

long OcularView::onCmdUpdateChart(FXObject*, FXSelector, void* ptr)
{
//	ChartList* cl = (ChartList*)ptr;
	Chart* chart = (Chart*)ptr;
	FXString label_text;
//	double radius = dimensions_.innerPlanetLabelR * radius_ / DENOMINATOR;
	for (Chart::BodyPropsMap::const_iterator it = chart->bodies_.begin(); it != chart->bodies_.end(); ++it) {
		AstroLabel* label = labels_.find_by_chart_id(chart->id_, (*it).first);
		bool need_insert = !label;
		if (!label)
			label = new PlanetLabel(this, -100, -100, 20, 20);
		label->setProps((*it).second);
		label_text.format("%c", GlyphManager::get_const_instance().getPlanetLabel((*it).first));
		label->setId((*it).first, label_text);
		label->setChartId(chart->id_);
		if (need_insert) {
			std::pair<AlcIter, bool> result = labels_.insert(label);
			assert (result.second);
		}
	}

	int cusp_count = chart->houses_.getCuspCount();
	BodyProps hprops;
	for (int i = 0; i < 2; ++i) {
		HouseLabel::HouseFlag hf = (i == 0) ? HouseLabel::hf_Asc : HouseLabel::hf_MC;
		AstroLabel* label = labels_.find_by_chart_id(chart->id_, HOUSE_ID_ASC + i);
		bool need_insert = !label;
		if (!label)
			label = new HouseLabel(this, hf, -100, -100, 20, 20);
		else
			label->setFlags((int)hf);
		hprops.prop[BodyProps::bp_Lon] = chart->houses_.ascmc[i];
		label->setProps(hprops);
		label_text.format("%d", i);
		label->setId(HOUSE_ID_ASC + i, label_text);
		label->setChartId(chart->id_);
		if (need_insert) {
			std::pair<AlcIter, bool> result = labels_.insert(label);
			assert (result.second);
		}
	}
    for (int i = 1; i <= cusp_count; ++i) {
		HouseLabel::HouseFlag hf = HouseLabel::flagOfHouse(i, cusp_count);
		AstroLabel* label = labels_.find_by_chart_id(chart->id_, HOUSE_ID_FIRST + i);
		bool need_insert = !label;
		if (!label)
			label = new HouseLabel(this, hf, -100, -100, 20, 20);
		else
			label->setFlags((int)hf);
		hprops.prop[BodyProps::bp_Lon] = chart->houses_.cusps[i];
		label->setProps(hprops);
		label_text.format("%d", i);
		label->setId(HOUSE_ID_FIRST + i, label_text);
		label->setChartId(chart->id_);
		if (need_insert) {
			std::pair<AlcIter, bool> result = labels_.insert(label);
			assert (result.second);
		}
    }
	switch (zero_point_) {
		case ZERO_ASC:
			zero_angle_ = 180 - chart->houses_.cusps[1];
			break;
		case ZERO_ARIES:
			zero_angle_ = 180;
			break;
	}
	reorderLabels();
	update();

	std::vector<AstroLabel*> planets;
	planets.resize(labels_.size());
	std::copy(labels_.begin(), labels_.end(), planets.begin());
	std::sort(planets.begin(), planets.end(), less_planet_list());
	getShell()->handle (0, FXSEL(SEL_COMMAND, astro::ID_FILL_PLANET_LIST), (void*)&planets);

	return 1;
}

struct less_deg {
	bool operator() (const AstroLabel* al1, const AstroLabel* al2) const
	{
		return al1->getVisibleAngle() < al2->getVisibleAngle();
	}
};

void OcularView::spreadLabels (int chart, AstroLabel::label_type_t type, double r)
{
	FXTRACE((99, "%s\n", __FUNCTION__));
	std::vector<SpreadValue> input;
	double delta_width = 0;

	alc_by_chart_type& idx = labels_.get<chart_type_tag>();
	std::pair<alc_by_chart_type::iterator, alc_by_chart_type::iterator> range =
        idx.equal_range(boost::make_tuple(chart, type));
	alc_by_chart_type::iterator it = range.first;
	while (it != range.second) {
		(*it)->setVisibleAngle((*it)->getAngle());
		input.push_back(SpreadValue((*it)->getAngle(), *it));
		delta_width = (*it)->getRect().w / 2;
        FXTRACE((90, "%s\n", (*it)->toString().text()));
		++it;
    }
	delta_width *= 1.2;
	double delta_ang = atan (delta_width / r) / DTOR * 2;
	CircleSpread cspread(input);

	std::vector<SpreadValue> output;
	FXTRACE((90, "Delta_ang %.02f, input vector size %d\n", delta_ang, input.size()));
	cspread.spread(output, delta_ang, 360);

    BOOST_FOREACH (SpreadValue& sv, output) {
		FXTRACE((99, "output %d->%.02f\n", (int)sv.ptr_, sv.val_));
		AstroLabel* label = static_cast<AstroLabel*>(sv.ptr_);
		label->setVisibleAngle(sv.val_);
	}
}

void OcularView::drawLabels (FXDC& dc)
{
	BOOST_FOREACH (AstroLabel* al, labels_) {
        al->handle(this, FXSEL(SEL_PAINT, 0), &dc);
    }
}

long OcularView::onMouseMove(FXObject* o, FXSelector sel, void* ptr)
{
    if (DraggableView::onMouseMove(o, sel, ptr))
        return 1;
    FXEvent *ev=(FXEvent*)ptr;
    AstroLabel* old_cur = cur_label_;
    cur_label_ = NULL;
    BOOST_FOREACH (AstroLabel* al, labels_) {
        if (al->contains(ev->win_x, ev->win_y)) {
            cur_label_ = al;
            break;
        }
    }
    if (cur_label_ != old_cur) {
       	FXDCWindow dc(this);
       	dc.setBackground(getBackColor());
        if (old_cur) {
			FXEvent ev1;
			ev1.rect = old_cur->getRect();
            old_cur->handle(this, FXSEL(SEL_COMMAND, AstroLabel::ID_SELECT), (void*)0); // unset selection
			handle(this, FXSEL(SEL_PAINT, 0), (void*)&ev1); // repaint background
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
    return sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&tip);
}

long OcularView::onQueryHelp(FXObject* sender, FXSelector, void*)
{
    if (cur_label_){
        FXString help;
        help.format("Label %d, angle %f", cur_label_->getId(), cur_label_->getAngle());
        return sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&help);
    }
    return 0;
}

void OcularView::drawAspects(FXDC& dc)
{
	double r = radius_ / DENOMINATOR;
	double zouter = dimensions_.zodiacInnerR * r;
	double zinner = dimensions_.aspectR * r;
	dc.setForeground(colors_.aspectTickColor);
	dc.setLineWidth(2);
	FXPoint pt[2];
	BOOST_FOREACH (AstroLabel* al, labels_) {
		if (al->getType() == AstroLabel::TYPE_PLANET) {
			double ang = al->getAngle() + zero_angle_;
			pt[0] = getXYdeg(ang, zouter);
			pt[1] = getXYdeg(ang, zinner);
			dc.drawLines(pt, 2);
		}
	}
}

void OcularView::drawPlanetLines(FXDC& dc)
{
	double r = radius_ / DENOMINATOR;
	double zouter = dimensions_.zodiac5dgrR * r;
	double zinner = dimensions_.innerPlanetLabelR * r;
	double zdgr = (dimensions_.zodiacInnerR + dimensions_.zodiac5dgrR) / 2 * r;
	dc.setLineWidth(1);
	FXPoint pt[2];

	FXString strDegree;
	FXFont* dgrFont = GlyphManager::get_const_instance().getFont(dimensions_.degreeFontSize * radius_ / DENOMINATOR, FF_ASTRO);

	BOOST_FOREACH (AstroLabel* al, labels_) {
		if (al->getType() == AstroLabel::TYPE_PLANET) {
            dc.setForeground(colors_.planetTickColor);
			double ang = al->getAngle() + zero_angle_;
			double angv = al->getVisibleAngle() + zero_angle_;
			double planet_r = al->getRect().w / 2;
			pt[0] = getXYdeg(ang, zouter);
			pt[1] = getXYdeg(angv, zinner);
			double dx = pt[1].x - pt[0].x, dy = pt[0].y - pt[1].y;
			double ang0 = atan(dy / dx);
			if (dx < 0)
				ang0 += PI;
			double hyp = sqrt(dx * dx + dy * dy) - planet_r;
			pt[1].x = pt[0].x + cos(ang0) * hyp;
			pt[1].y = pt[0].y - sin(ang0) * hyp;
			dc.drawLines(pt, 2);

			strDegree.format("%02d%c", ((int)al->getAngle() % DEG_PER_SIGN) + 1, GlyphManager::get_const_instance().getDegreeSign(FF_ASTRO));
			pt[1] = getXYdeg(angv, zdgr);
			FXint tw = dgrFont->getTextWidth(strDegree);
			FXint th = dgrFont->getTextHeight(strDegree);
			dc.setForeground(FXRGB(0, 0, 0));
			dc.setFont(dgrFont);
			dc.drawText(pt[1].x - tw / 2, pt[1].y + th / 2, strDegree);
		}
	}
}

void OcularView::drawHouseLines(FXDC& dc)
{
	double r = radius_ / DENOMINATOR;
	double zinner = dimensions_.zodiacInnerR * r;
	dc.setLineWidth(1);
	dc.setBackground(getBackColor());
	FXPoint pt[2];

	FXString strDegree;
	FXFont* dgrFont = GlyphManager::get_const_instance().getFont(dimensions_.degreeFontSize * radius_ / DENOMINATOR, FF_ASTRO);

	BOOST_FOREACH (AstroLabel* al, labels_) {
		if (al->getType() == AstroLabel::TYPE_HOUSE) {
			double ang = al->getAngle() + zero_angle_;
			HouseLabel::HouseFlag hf = (HouseLabel::HouseFlag)al->getFlags();
			pt[0] = getXYdeg(ang, zinner);
			if (hf == HouseLabel::hf_Undef) {
				pt[1] = getXYdeg(ang, dimensions_.zodiac5dgrR * r);
				dc.setForeground(colors_.planetTickColor);
				dc.drawLines(pt, 2);
			}
			else {
				dc.setForeground(colors_.arrowColor);
				switch (hf) {
					case HouseLabel::hf_Asc:
					case HouseLabel::hf_MC: {
						double r_ascmc = dimensions_.ascArrowR * r;
						pt[1] = getXYdeg(ang, r_ascmc);
						dc.drawLines(pt, 2);
						pt[0] = getXYdeg(ang - 1, r_ascmc * 0.98);
						dc.drawLines(pt, 2);
						pt[0] = getXYdeg(ang + 1, r_ascmc * 0.98);
						dc.drawLines(pt, 2);
						dc.setForeground(FXRGB(0, 0, 0));
						dc.setFont(dgrFont);
						if (hf == HouseLabel::hf_Asc) {
							pt[1] = getXYdeg(ang, r_ascmc * 0.98);
							strDegree.format("%02d%c",
								(int)al->getAngle() % DEG_PER_SIGN + 1,
								GlyphManager::get_const_instance().getDegreeSign(FF_ASTRO));
							dc.drawText(pt[1].x, pt[1].y - 1, strDegree);
							strDegree.format("%02d'",
								(int)(al->getAngle() - (int)al->getAngle()) * 60 + 1);
							FXint th = dgrFont->getTextHeight(strDegree);
							dc.drawText(pt[1].x, pt[1].y + th, strDegree);
						}
						else {
							pt[1] = getXYdeg(ang, r_ascmc * 0.96);
							strDegree.format("%02d%c%02d'",
								(int)al->getAngle() % DEG_PER_SIGN + 1,
								GlyphManager::get_const_instance().getDegreeSign(FF_ASTRO),
								(int)(al->getAngle() - (int)al->getAngle()) * 60 + 1);
							FXint tw = dgrFont->getTextWidth(strDegree);
							FXint th = dgrFont->getTextHeight(strDegree);
							dc.drawText(pt[1].x - tw / 2, pt[1].y + th / 2, strDegree);
						}
						}
						break;
					case HouseLabel::hf_Dsc: {
						double r_dsc = dimensions_.ascArrowR * 0.93 * r;
						double r_circle = dimensions_.ascArrowR * 0.015 * r;
						pt[1] = getXYdeg(ang, r_dsc);
						dc.drawLines(pt, 2);
						pt[1] = getXYdeg(ang, r_dsc + r_circle);
						dc.drawEllipse(pt[1].x - r_circle, pt[1].y - r_circle, 2 * r_circle, 2 * r_circle); }
						break;
					case HouseLabel::hf_IC: {
						double r_ic = dimensions_.ascArrowR * 0.96 * r;
						pt[1] = getXYdeg(ang, r_ic);
						dc.drawLines(pt, 2);
						pt[0] = getXYdeg(ang + 2, r_ic);
						pt[1] = getXYdeg(ang - 2, r_ic);
						dc.drawLines(pt, 2); }
						break;
				}
			}
		}
	}
}

void OcularView::dragResize (FXint x, FXint y)
{
//	is_resizing_ = true;
	WheelView::dragResize (x, y);
}
