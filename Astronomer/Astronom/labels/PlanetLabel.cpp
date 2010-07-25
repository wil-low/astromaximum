#include "PlanetLabel.h"
#include "../utils/GlyphManager.h"

FXDEFMAP(PlanetLabel) PlanetLabelMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             0, PlanetLabel::onDrawOnParent),
	FXMAPFUNC(SEL_CLICKED,           0, PlanetLabel::onClicked),
};

FXIMPLEMENT(PlanetLabel, AstroLabel, PlanetLabelMessageMap, ARRAYNUMBER(PlanetLabelMessageMap))

PlanetLabel::PlanetLabel(DraggableView* p, FXint x, FXint y, FXint w, FXint h)
: AstroLabel(p, x, y, w, h)
{
}

PlanetLabel::~PlanetLabel(void)
{
}

double PlanetLabel::getAngle() const
{
	return props_.prop[BodyProps::bp_Lon];
}

long PlanetLabel::onClicked(FXObject*, FXSelector, void*)
{
	return 1;
}

body_type_t PlanetLabel::getType() const
{
    return TYPE_PLANET;
}

double PlanetLabel::getVisibleAngle() const
{
	return visibleLon_;
}

void PlanetLabel::setVisibleAngle(double ang)
{
    visibleLon_ = ang;
}

void PlanetLabel::setProps(const BodyProps& props)
{
	props_ = props;
	if (props_.prop[BodyProps::bp_LonSpeed] < -STABLESPEED)
		flags_ |= af_Retrograde;
}
