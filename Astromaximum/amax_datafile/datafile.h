/* 
 * File:   datafile.h
 * Author: willow
 *
 * Created on 26 вересня 2012, 0:26
 */

#ifndef DATAFILE_H
#define	DATAFILE_H

#include <stdio.h>
#include <time.h>

#define DATAFILE_EXPORT __attribute__ ((visibility ("default")))

#ifdef	__cplusplus
extern "C" {
#endif

	static const int SEC_IN_DAY = 24 * 60 * 60;
	
	enum planet_id {
		SE_SUN = 0,
		SE_MOON = 1,
		SE_MERCURY = 2,
		SE_VENUS = 3,
		SE_MARS = 4,
		SE_JUPITER = 5,
		SE_SATURN = 6,
		SE_URANUS = 7,
		SE_NEPTUNE = 8,
		SE_PLUTO = 9,
		SE_TRUE_NODE = 10,
		SE_MEAN_APOG = 11,
		SE_WHITE_MOON = 12,
	};
	
	static const char* PLANET_NAME[] = {
		"??",
		"SO",
		"MO",
		"ME",
		"VE",
		"MA",
		"JU",
		"SA",
		"UR",
		"NE",
		"PL",
		"TN",
		"AP",
		"WM",
	};
	
	enum event_type {
		EV_VOC = 0, // void of course
		EV_SIGN_ENTER = 1, // enter into sign
		EV_ASP_EXACT = 2, // exact aspect
		EV_RISE = 3, // rising & setting
		EV_DEGREE_PASS = 4, // entering degree
		EV_VIA_COMBUSTA = 5, // good & bad degrees
		EV_RETROGRADE = 6,
		EV_ECLIPSE = 7,
		EV_TITHI = 8,
		EV_NAKSHATRA = 9,
		EV_SET = 10, // rising & setting
		EV_DECL_EXACT = 11, // declination
		EV_NAVROZ = 12, // Navroz
		EV_TOP_DAY = 13, // week days
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
		EV_DEG_2ND = 29, // degrees on second page
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
		EV_TATTVAS = 52,
		EV_LAST = 53,
	};
	
	enum datafile_type {
		DFT_COMMON = 0,
		DFT_LOCATION = 1,
	};

	struct datafile_event {
		int date0_, date1_;
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
		time_t* transitionTimes_;
		int* transitionOffsets_;
		char** transitionNames_;
		size_t data_len_;
		unsigned char* data_;
	};
	typedef struct header_location* pHeaderLocation;

	struct datafile {
		unsigned char* read_pos_;
		enum datafile_type type_;
		int startJD_;
		int finalJD_;
		pHeaderCommon hdr_common_;
		pHeaderLocation hdr_location_;
	};
	typedef struct datafile* pDatafile;

	DATAFILE_EXPORT
	const char* event_dump(pEvent ev, char* buffer);

	DATAFILE_EXPORT
	const char* time_t2str(const time_t* datetime, char* buffer);
	
	DATAFILE_EXPORT
	time_t date2time_t(int year, int month, int day, int hour, int min);

	DATAFILE_EXPORT
	pDatafile datafile_create();

	DATAFILE_EXPORT
	int datafile_init(pDatafile df, enum datafile_type df_type, size_t len, const char* data);

	DATAFILE_EXPORT
	void datafile_fini(pDatafile df);


	DATAFILE_EXPORT
	int datafile_tz_offset(const pDatafile df, const time_t* date);
	
	DATAFILE_EXPORT
	int datafile_get_events(const pDatafile df, int evtype, int planet, const time_t* dayStart, const time_t* dayEnd, pEvent events);

#ifdef	__cplusplus
}
#endif

#endif	/* DATAFILE_H */

