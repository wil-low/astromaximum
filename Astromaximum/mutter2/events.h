#ifndef eventsH
#define eventsH
#pragma pack(1)

#ifndef __BCPLUSPLUS__
#include "sweodef.h"
extern "C" double swe_julday(int, int, int, double, int);
extern "C" void swe_revjul(double, int, int*, int*, int*, double*);
extern "C" int swe_calc_ut(double, int, int, double*, char*);
extern "C" int32 swe_rise_trans(
        double, int, char*, int, int, double*, double, double, double*, char*);
extern "C" int32 swe_sol_eclipse_when_glob(double, int, int, double*, int, char*);
extern "C" int32 swe_lun_eclipse_when(double, int, int, double*, int, char*);
extern "C" void swe_set_ephe_path(char*);
extern "C" int swe_houses(double, double, double, int, double*, double*);
extern "C" int swe_day_of_week(double);
#endif
#include "swephexp.h"

const int PLANET_COUNT = SE_PLUTO - SE_SUN + 1;
const int EFLAG = SEFLG_SWIEPH;

const double MINUTE_STEP = (1. / 24 / 60);
const double MSECINDAY = 86400 * 1000;

struct sEphRecord {
    double data[13];
};

struct sAphRecord {
    char data[360];
};

struct sAscRecord {
    double data[2];
};

struct sMatrix {
    unsigned char ang;
    unsigned int counter;
    unsigned int step;
};

typedef enum {
    EV_VOC = 0, // void of course
    EV_SIGN_ENTER, // enter into sign
    EV_ASP_EXACT, // exact aspect
    EV_RISE, // rising & setting
    EV_DEGREE_PASS, // entering degree
    EV_VIA_COMBUSTA,
    EV_RETROGRADE,
    EV_ECLIPSE,
    EV_TITHI,
    EV_NAKSHATRA,
    EV_SET, // setting
    EV_DECL_EXACT, // declination
    EV_NAVROZ, // Navroz
    EV_WEEK, // week
    EV_PLANET_HOUR, // planetary hours
    EV_STATUS,
    EV_SUN_RISE,
    EV_MOON_RISE,
    EV_MOON_MOVE,
    EV_SEL_DEGREES,
    EV_DAY_HOURS,
    EV_NIGHT_HOURS,
    EV_SUN_DAY,
    EV_MOON_DAY,
    EV_GRID_DATE,
    EV_MOON_PHASE,
    EV_ZODIAC_SIGN,
    EV_PANEL,
    EV_FAST_BUTTON,
    EV_DEG_2ND,
    EV_WEEK_GRID,
    EV_MONTH_GRID,
    EV_DECUMBITURE,
    EV_DECUMB_ASPECT,
    EV_DECUMB_BEGIN,
    EV_SUN_DEGREE_LARGE,
    EV_MOON_SIGN_LARGE,
    EV_HELP,
    EV_ASP_EXACT_MOON,
    EV_DEGPASS0,
    EV_DEGPASS1,
    EV_DEGPASS2,
    EV_DEGPASS3,
    EV_HELP0,
    EV_HELP1,
    EV_ASTRORISE,
    EV_ASTROSET,
    EV_APHETICS,
    EV_FAST,
    EV_ASCAPHETICS,
    EV_LAST // last - do not use
} EventType;


#endif

// # vi:et:ts=4:sw=4
