#include "Ephemeris.h"
#include "TimeLoc.h"
#include <swephexp.h>
#include <fxdefs.h>

Ephemeris::Ephemeris(char* path)
{
	swe_set_ephe_path(path);
}

Ephemeris::~Ephemeris()
{
	swe_close();
}

double Ephemeris::julday (int y, int m, int d, int h, int min, int s, int gregflag)
{
	return swe_julday (y, m, d, h + min / 60 + s / 3600, gregflag);
}

long Ephemeris::calc_body (BodyProps& props, int body, long flags, const TimeLoc& time_loc)
{
	char serr[256] = "";
	long result = swe_calc_ut (time_loc.jday_, body, flags, props.prop, serr);
	if (result < 0 || serr[0] != 0)
		FXTRACE((10, "%s: %s\n", __FUNCTION__, serr));
	return result;
}
