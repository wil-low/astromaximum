#include "PlanetLabel.h"

FXDEFMAP(PlanetLabel) PlanetLabelMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             0, PlanetLabel::onDrawOnParent),
	FXMAPFUNC(SEL_CLICKED,           0, PlanetLabel::onClicked),
};

FXIMPLEMENT(PlanetLabel, AstroLabel, PlanetLabelMessageMap, ARRAYNUMBER(PlanetLabelMessageMap))

PlanetLabel::PlanetLabel(double lon, DraggableView* p, FXint x, FXint y, FXint w, FXint h)
: AstroLabel(p, x, y, w, h)
, lon_(lon)
{
}

PlanetLabel::~PlanetLabel(void)
{
}

double PlanetLabel::getLon()
{
	return lon_;
}

long PlanetLabel::onClicked(FXObject*, FXSelector, void*)
{
	return 1;
}

long PlanetLabel::onDrawOnParent(FXObject*, FXSelector, void* ptr)
{
    FXTRACE((10, "%s: %d %d %d %d\n", __FUNCTION__, rect_.x, rect_.y, rect_.w, rect_.h));
    FXDC* dc = (FXDC*)ptr;
    dc->setForeground(FXRGB(255, 0, 255));
    dc->setClipRectangle (rect_.x, rect_.y, rect_.w, rect_.h);
//    dc->drawRectangle (rect_.x, rect_.y, rect_.w - 1, rect_.h - 1);
    FXint tw = font_->getTextWidth(text_);
    FXint th = font_->getTextHeight(text_);
    dc->drawText(rect_.x + (rect_.w - tw) / 2, rect_.y + (rect_.h + th) / 2, text_);
    return 0;
}


