#pragma once

class TimeLoc
{
public:
	TimeLoc(void);
	virtual ~TimeLoc(void);
	// date
	double jday_;
	// location
	double longitude_;
	double latitude_;
	double elevation_;
};

class BodyProps {
public:
	enum body_property {
		bp_Lon = 0,
		bp_Lat,
		bp_Dist,
		bp_LonSpeed,
		bp_LatSpeed,
		bp_DistSpeed
	};
	double prop[6];
};
