#pragma once
#include <fx.h>
class Chart;
class TimeLoc;
class BodyProps;
class HouseProps;

namespace Ephemeris
{
	void init(char* path);
	void fini();
	double now ();
	double julday (int y, int m, int d, int h, int min, int s, int gregflag = 1);
	void revjul (double julday, int *y, int *m, int *d, int *h, int *min, int *s, int gregflag = 1);
	long calc_body (BodyProps& props, int body, long flags, const TimeLoc& time_loc);
	long calc_house (HouseProps& props, int method, const TimeLoc& time_loc);
	FXString planet_name (int id);
};
