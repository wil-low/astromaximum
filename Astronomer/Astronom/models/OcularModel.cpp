#include "OcularModel.h"
#include "../Ephemeris.h"
#include "../views/DraggableView.h"
#include "../Chart.h"
#include "../utils/constants.h"

OcularModel::OcularModel(Ephemeris* ephemeris)
: BaseModel(ephemeris)
, day_(1)
{
}

OcularModel::~OcularModel()
{
}

void OcularModel::setView(DraggableView* view)
{
	view_ = view;
	view_->handle (0, FXSEL(SEL_COMMAND, astro::ID_SET_ZERO), (void*)ZERO_ARIES);

	//TODO: move to config file
	OcularDimensions odim;
	odim.ascArrowR = 650.0/654.0 * DENOMINATOR;
	odim.zodiacOuterR = 520.0/654.0 * DENOMINATOR;
	odim.zodiac10dgrR = 504.0/654.0 * DENOMINATOR;
	odim.zodiac5dgrR = 454.0/654.0 * DENOMINATOR;
	odim.innerPlanetLabelR = 406.0/654.0 * DENOMINATOR;
	odim.zodiac30dgrR = 0/654.0 * DENOMINATOR;
	odim.innerPlanetR = 430/654.0 * DENOMINATOR;
	odim.zodiacInnerR = 244.0/654.0 * DENOMINATOR;
	odim.aspectR = 236/654.0 * DENOMINATOR;
	odim.planetFontSize = 500;
	odim.zodiacFontSize = 350;
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

	TimeLoc time_loc;
	time_loc.jday_ = ephemeris_->julday (2010, 4, day_, 12, 0, 0);
	time_loc.latitude_ = 45;
	time_loc.longitude_ = 34;
	time_loc.elevation_ = 0;

	int bodies[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	Chart chart;
	chart.time_loc_ = time_loc;
	BodyProps props;
	for (int i =  0; i < ARRAYNUMBER(bodies); ++i) {
		ephemeris_->calc_body (props, bodies[i], 0, time_loc);
		chart.bodies_[bodies[i]] = props;
	}
/*
	chart.bodies_[0].prop[BodyProps::bp_Lon] = 352;
	chart.bodies_[1].prop[BodyProps::bp_Lon] = 354.4;
	chart.bodies_[2].prop[BodyProps::bp_Lon] = 355.1;
	chart.bodies_[3].prop[BodyProps::bp_Lon] = 350;
	chart.bodies_[4].prop[BodyProps::bp_Lon] = 12.4;
	chart.bodies_[6].prop[BodyProps::bp_Lon] = 9.4;
	chart.bodies_[5].prop[BodyProps::bp_Lon] = 10.4;
*/
/*
	chart.bodies_[0].prop[BodyProps::bp_Lon] = 358;
	chart.bodies_[1].prop[BodyProps::bp_Lon] = 359.4;
	chart.bodies_[2].prop[BodyProps::bp_Lon] = 0.1;
	chart.bodies_[3].prop[BodyProps::bp_Lon] = 2;

	chart.bodies_[4].prop[BodyProps::bp_Lon] = 120.4;
	chart.bodies_[5].prop[BodyProps::bp_Lon] = 23.4;
	chart.bodies_[6].prop[BodyProps::bp_Lon] = 21.4;
*/
/*
	chart.bodies_[0].prop[BodyProps::bp_Lon] = 12;
	chart.bodies_[1].prop[BodyProps::bp_Lon] = 12.4;
	chart.bodies_[2].prop[BodyProps::bp_Lon] = 12.1;
	chart.bodies_[3].prop[BodyProps::bp_Lon] = 12;

	chart.bodies_[4].prop[BodyProps::bp_Lon] = 2.4;
	chart.bodies_[6].prop[BodyProps::bp_Lon] = 3.4;
	chart.bodies_[5].prop[BodyProps::bp_Lon] = 1.4;
*/
/*
	chart.bodies_[0].prop[BodyProps::bp_Lon] = 120.4;
	chart.bodies_[1].prop[BodyProps::bp_Lon] = 220.4;
	chart.bodies_[2].prop[BodyProps::bp_Lon] = 320.4V;
	chart.bodies_[4].prop[BodyProps::bp_Lon] = 20.4;
*/
	view_->handle (0, FXSEL(SEL_COMMAND, astro::ID_UPDATE_CHART), (void*)&chart);
}

void OcularModel::incHour()
{
	++day_;
	setView(view_);
}
