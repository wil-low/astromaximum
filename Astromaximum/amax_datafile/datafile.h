/* 
 * File:   datafile.h
 * Author: willow
 *
 * Created on 26 вересня 2012, 0:26
 */

#ifndef DATAFILE_H
#define	DATAFILE_H

#include <stdio.h>

#define DATAFILE_EXPORT __attribute__ ((visibility ("default")))

#ifdef	__cplusplus
extern "C" {
#endif

static const char SE_SUN = 0;
static const char SE_MOON = 1;
static const char SE_MERCURY = 2;
static const char SE_VENUS = 3;
static const char SE_MARS = 4;
static const char SE_JUPITER = 5;
static const char SE_SATURN = 6;
static const char SE_URANUS = 7;
static const char SE_NEPTUNE = 8;
static const char SE_PLUTO = 9;
static const char SE_TRUE_NODE = 10;
static const char SE_MEAN_APOG = 11;
static const char SE_WHITE_MOON = 12;

static const int EV_VOC = 0; // void of course
static const int EV_SIGN_ENTER = 1; // enter into sign
static const int EV_ASP_EXACT = 2; // exact aspect
static const int EV_RISE = 3;  // rising & setting
static const int EV_DEGREE_PASS = 4;  // entering degree
static const int EV_VIA_COMBUSTA = 5;  // good & bad degrees
static const int EV_RETROGRADE = 6;
static const int EV_ECLIPSE = 7;
static const int EV_TITHI = 8;
static const int EV_NAKSHATRA = 9;
static const int EV_SET = 10;  // rising & setting
static const int EV_DECL_EXACT = 11;  // declination
static const int EV_NAVROZ = 12;  // Navroz
static const int EV_TOP_DAY = 13;  // week days
static const int EV_PLANET_HOUR = 14;  // planetary hours
static const int EV_STATUS = 15;
static const int EV_SUN_RISE = 16;
static const int EV_MOON_RISE = 17;
static const int EV_MOON_MOVE = 18;
static const int EV_SEL_DEGREES = 19;
static const int EV_DAY_HOURS = 20;
static const int EV_NIGHT_HOURS = 21;
static const int EV_SUN_DAY = 22;
static const int EV_MOON_DAY = 23;
static const int EV_TOP_MONTH = 24;
static const int EV_MOON_PHASE = 25;
static const int EV_ZODIAC_SIGN = 26;
static const int EV_PANEL = 27;
static const int EV_TOPIC_BUTTON = 28;
static const int EV_DEG_2ND = 29; // degrees on second page
static const int EV_WEEK_GRID = 30;
static const int EV_MONTH_GRID = 31;
static const int EV_DECUMBITURE = 32;
static const int EV_DECUMB_ASPECT = 33;
static const int EV_DECUMB_BEGIN = 34;
static const int EV_SUN_DEGREE_LARGE = 35;
static const int EV_MOON_SIGN_LARGE = 36;
static const int EV_HELP = 37;
static const int EV_ASP_EXACT_MOON = 38;
static const int EV_DEGPASS0 = 39;
static const int EV_DEGPASS1 = 40;
static const int EV_DEGPASS2 = 41;
static const int EV_DEGPASS3 = 42;
static const int EV_HELP0 = 43;
static const int EV_HELP1 = 44;
static const int EV_ASTRORISE = 45;
static const int EV_ASTROSET = 46;
static const int EV_APHETICS = 47;
static const int EV_FAST = 48;
static const int EV_ASCAPHETICS = 49;
static const int EV_MSG = 50;
static const int EV_BACK = 51;
static const int EV_TATTVAS = 52;
static const int EV_LAST = 53;

enum datafile_type {
    DFT_COMMON = 0,
    DFT_LOCATION = 1,
};

struct datafile_event {
    long date0_, date1_;
    short degree_;
    char planet0_, planet1_;
};
typedef struct datafile_event* pEvent;

struct header_common {
    int start_year_;
    char start_month_;
    char start_day_;
    size_t custom_data_len_;
    char custom_data_[10];
    size_t day_count_;
    size_t data_len_;
    unsigned char* data_;
};
typedef struct header_common* pHeaderCommon;

struct header_location {
    int start_year_;
    char start_month_;
    char start_day_;
    size_t day_count_;
    int city_id_;
    int coords[3];
    char city_[50];
    char state_[50];
    char country_[50];
    char timezone_[50];
    size_t custom_data_len_;
    char custom_data_[10];
    size_t transitionCount_;
    long* transitionTimes_;
    long* transitionOffsets_;
    char** transitionNames_;
    size_t data_len_;
    unsigned char* data_;
};
typedef struct header_location* pHeaderLocation;

struct datafile {
    unsigned char* read_pos_;
    enum datafile_type type_;
    pHeaderCommon hdr_common_;
    pHeaderLocation hdr_location_;
};
typedef struct datafile* pDatafile;

DATAFILE_EXPORT
pDatafile datafile_create ();

DATAFILE_EXPORT
int datafile_init (pDatafile df, enum datafile_type df_type, size_t len, const char* data);

DATAFILE_EXPORT
void datafile_fini (pDatafile df);

#ifdef	__cplusplus
}
#endif

#endif	/* DATAFILE_H */

