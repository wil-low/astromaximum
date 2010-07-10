#include "Ephemeris.h"
#include "TimeLoc.h"
#include "utils/BodyProps.h"
#include "utils/HouseProps.h"

#ifdef __linux
    #include <sweodef.h>
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
#include <time.h>

namespace Ephemeris
{

void init(char* path)
{
	swe_set_ephe_path(path);
}

void fini()
{
	swe_close();
}

double now ()
{
#ifdef __linux
	struct tm *t;
	time_t ltime;
	time(&ltime);
	t=gmtime(&ltime);
	return Ephemeris::julday (t->tm_year+1900, t->tm_mon+1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec);
#else
	SYSTEMTIME t;
	GetSystemTime(&t);
	return Ephemeris::julday (t.wYear, t.wMonth, t.wDay,
		t.wHour, t.wMinute, t.wSecond);
#endif
}

double julday (int y, int m, int d, int h, int min, int s, int gregflag)
{
	return swe_julday (y, m, d, h + min / 60. + s / 3600., gregflag);
}

void revjul (double julday, int *y, int *m, int *d, int *h, int *min, int *s, int gregflag)
{
	double hour, minutes;
	swe_revjul(julday, gregflag, y, m, d, &hour);
	*h = hour;
	minutes = (hour - *h) * 60;
	*min = minutes;
	*s = (minutes - *min) * 60;
}

long calc_body (BodyProps& props, int body, long flags, const TimeLoc& time_loc)
{
	char serr[256] = "";
	long result = swe_calc_ut (time_loc.get(TL_DATE), body, flags, props.prop, serr);
	if (result < 0 || serr[0] != 0)
		FXTRACE((10, "%s: %s\n", __FUNCTION__, serr));
	return result;
}

long calc_house (HouseProps& props, int method, const TimeLoc& time_loc)
{
	long result = swe_houses (time_loc.get(TL_DATE), time_loc.get(TL_LAT), time_loc.get(TL_LON),
        method, props.cusps, props.ascmc);
    props.method = (HouseProps::house_method)method;
	if (result < 0)
		FXTRACE((10, "%s: %s\n", __FUNCTION__));
	return result;
}
}
