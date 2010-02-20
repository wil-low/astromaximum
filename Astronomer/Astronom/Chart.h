#pragma once
#include <map>
#include <list>

struct BodyProps {
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

typedef std::map<int, BodyProps> BodyPropsMap;

class Chart
{
public:
	Chart();
	~Chart();
	BodyPropsMap bodies_;
};

typedef std::list<Chart> ChartList;
