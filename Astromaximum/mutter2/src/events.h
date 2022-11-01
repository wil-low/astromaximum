#ifndef eventsH
#define eventsH
#pragma pack(1)

#ifndef __BCPLUSPLUS__
#include "sweodef.h"
extern "C" {
    double swe_julday(int, int, int, double, int);
    void swe_revjul(double, int, int*, int*, int*, double*);
    int swe_calc_ut(double, int, int, double*, char*);
    int32 swe_rise_trans(
        double, int, char*, int, int, double*, double, double, double*, char*);
    int32 swe_sol_eclipse_when_glob(double, int, int, double*, int, char*);
    int32 swe_lun_eclipse_when(double, int, int, double*, int, char*);
    void swe_set_ephe_path(const char*);
    void swe_close(void);
    int swe_houses(double, double, double, int, double*, double*);
    int swe_day_of_week(double);
}
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
    EV_SIGN_ENTER = 1, // enter into sign
    EV_ASP_EXACT = 2, // exact aspect
    EV_RISE = 3, // rising & setting
    EV_DEGREE_PASS = 4, // entering degree
    EV_VIA_COMBUSTA = 5,
    EV_RETROGRADE = 6,
    EV_ECLIPSE = 7,
    EV_TITHI = 8,
    EV_NAKSHATRA = 9,
    EV_SET = 10, // setting
    EV_DECL_EXACT = 11, // declination
    EV_NAVROZ = 12, // Navroz
    EV_TOP_DAY = 13, // week
    EV_PLANET_HOUR = 14, // planetary hours
    EV_STATUS = 15,
    EV_SUN_RISE = 16,
    EV_MOON_RISE = 17,
    EV_MOON_MOVE = 18,
    EV_SEL_DEGREES = 19,
    EV_DAY_HOURS = 20,
    EV_NIGHT_HOURS = 21,
    EV_SUN_DAY = 22,
    EV_MOON_DAY = 23,
    EV_TOP_MONTH = 24,
    EV_MOON_PHASE = 25,
    EV_ZODIAC_SIGN = 26,
    EV_PANEL = 27,
    EV_TOPIC_BUTTON = 28,
    EV_DEG_2ND = 29,
    EV_WEEK_GRID = 30,
    EV_MONTH_GRID = 31,
    EV_DECUMBITURE = 32,
    EV_DECUMB_ASPECT = 33,
    EV_DECUMB_BEGIN = 34,
    EV_SUN_DEGREE_LARGE = 35,
    EV_MOON_SIGN_LARGE = 36,
    EV_HELP = 37,
    EV_ASP_EXACT_MOON = 38,
    EV_DEGPASS0 = 39,
    EV_DEGPASS1 = 40,
    EV_DEGPASS2 = 41,
    EV_DEGPASS3 = 42,
    EV_HELP0 = 43,
    EV_HELP1 = 44,
    EV_ASTRORISE = 45,
    EV_ASTROSET = 46,
    EV_APHETICS = 47,
    EV_FAST = 48,
    EV_ASCAPHETICS = 49,
    EV_MSG = 50,
    EV_BACK = 51,
    EV_LAST // last - do not use
} EventType;

typedef unsigned int uint;

#endif

// # vi:et:ts=4:sw=4
