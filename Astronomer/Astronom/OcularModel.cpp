#include "OcularModel.h"
#include "DraggableView.h"
#include "Chart.h"

OcularModel::OcularModel()
: view_(0)
{
}

OcularModel::~OcularModel(void)
{
}

void OcularModel::setView(DraggableView* view)
{
	view_ = view;
	view_->handle (0, FXSEL(SEL_COMMAND, astro::ID_SET_ZERO), (void*)ZERO_ARIES);

	//TODO: move to config file
	OcularDimensions odim;
	odim.ascArrowR = 905.0/906.0 * DENOMINATOR;
	odim.zodiacOuterR = 718.0/906.0 * DENOMINATOR;
	odim.zodiac10dgrR = 696.0/906.0 * DENOMINATOR;
	odim.zodiac5dgrR = 628.0/906.0 * DENOMINATOR;
	odim.innerPlanetLabelR = 590.0/906.0 * DENOMINATOR;
	odim.zodiac30dgrR = 0/906.0 * DENOMINATOR;
	odim.innerPlanetR = 558/906.0 * DENOMINATOR;
	odim.zodiacInnerR = 338.0/906.0 * DENOMINATOR;
	odim.aspectR = 328/906.0 * DENOMINATOR;
	view_->handle (0, FXSEL(SEL_COMMAND, astro::ID_SET_OCULAR_DIM), (void*)&odim);

	OcularColors ocolors;
	ocolors.ocularColor = FXRGB(247,240,255); // almost grey
	ocolors.contourColor = FXRGB(0,0,0);
	ocolors.mainLineColor = FXRGB(128,0,192); //light violet
	ocolors.labelColor = ocolors.contourColor;
	ocolors.fillColor = FXRGB(240,224,255);
	ocolors.arrowColor = FXRGB(255,0,0);
	ocolors.cuspidColor = FXRGB(0,192,128);
	ocolors.tick10Color = FXRGB(192,0,255);
	ocolors.innerRColor = FXRGB(192,0,255);
	ocolors.planetTickColor = FXRGB(0,192,255);
	ocolors.aspectTickColor = FXRGB(0,0,0);
	view_->handle (0, FXSEL(SEL_COMMAND, astro::ID_SET_OCULAR_COLOR), (void*)&ocolors);

	Chart chart;
	chart.bodies_[0].prop[BodyProps::bp_Lon] = 112.4; 
	chart.bodies_[1].prop[BodyProps::bp_Lon] = 12.4; 
	chart.bodies_[2].prop[BodyProps::bp_Lon] = 88.4; 
	view_->handle (0, FXSEL(SEL_COMMAND, astro::ID_UPDATE_CHART), (void*)&chart);
}
