#include "PlanetLabel.h"

FXDEFMAP(PlanetLabel) PlanetLabelMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             0, PlanetLabel::onDrawOnParent),
	FXMAPFUNC(SEL_CLICKED,           0, PlanetLabel::onClicked),
};

FXIMPLEMENT(PlanetLabel, AstroLabel, PlanetLabelMessageMap, ARRAYNUMBER(PlanetLabelMessageMap))

PlanetLabel::PlanetLabel(DraggableView* p, FXint x, FXint y, FXint w, FXint h)
: AstroLabel(p, x, y, w, h)
, lon_(0)
{
}

PlanetLabel::~PlanetLabel(void)
{
}

double PlanetLabel::getAngle() const
{
	return lon_;
}

long PlanetLabel::onClicked(FXObject*, FXSelector, void*)
{
	return 1;
}

int PlanetLabel::getType() const
{
    return TYPE_PLANET;
}

double PlanetLabel::getVisibleAngle() const
{
	return visibleLon_;
}

void PlanetLabel::setAngle(double ang)
{
	lon_ = ang;
}

void PlanetLabel::setVisibleAngle(double ang)
{
    visibleLon_ = ang;
}

