#pragma once
class Chart;
class TimeLoc;
class BodyProps;

class Ephemeris
{
public:
	Ephemeris(char* path);
	virtual ~Ephemeris();
	double julday (int y, int m, int d, int h, int min, int s, int gregflag = 1);
	long calc_body (BodyProps& props, int body, long flags, const TimeLoc& time_loc);
};
