#include "Ephemeris.h"
#include "TimeLoc.h"

#ifdef __linux
    #include "sweodef.h"
    extern "C" {
        double swe_julday(int, int, int, double, int);
        void swe_revjul(double, int, int*, int*, int*, double*);
        int swe_calc_ut(double, int, int, double*, char*);
        int32 swe_rise_trans(
            double, int, char*, int, int, double*, double, double, double*, char*);
        int32 swe_sol_eclipse_when_glob(double, int, int, double*, int, char*);
        int32 swe_lun_eclipse_when(double, int, int, double*, int, char*);
        void swe_set_ephe_path(char*);
        void swe_close(void);
        int swe_houses(double, double, double, int, double*, double*);
        int swe_day_of_week(double);
    }
#endif

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
