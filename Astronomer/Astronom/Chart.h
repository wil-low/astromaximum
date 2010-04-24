#pragma once
#include "TimeLoc.h"
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
	int id_;
};

typedef std::list<Chart> ChartList;
