#pragma once
#include "TimeLoc.h"
#include "utils/BodyProps.h"
#include "utils/HouseProps.h"
#include <map>
#include <list>

class Chart
{
public:
	Chart();
	~Chart();
	typedef std::map<int, BodyProps> BodyPropsMap;
	BodyPropsMap bodies_;
	TimeLoc time_loc_;
	HouseProps houses_;
	int id_;
};

typedef std::list<Chart> ChartList;
