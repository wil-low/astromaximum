#pragma once
const int ZODIAC_SIGN_COUNT = 12;
const int DEG_PER_SIGN = 30;
const int LAST_PLANET_ID = 9;
const double DENOMINATOR = 10000.0;

const double STABLESPEED=6./60/60;

const char UNNAMED_DOC[] = "<Celestial>";
void normAngle(double &a);

enum {
	HOUSE_ID_ASC = 99998,
	HOUSE_ID_MC = 99998,
	HOUSE_ID_FIRST = 100000,
};

enum deg_mode {
	dm_Absolute = 0,
	dm_Longitude,
	dm_RectAsc,
	dm_OblAsc,
	dm_LatDecl,
};

enum body_type_t {
    TYPE_ZODIAC = 0,
    TYPE_PLANET,
    TYPE_HOUSE,
    TYPE_ASPECT,
    TYPE_LAST,
};

enum font_face_t {
	FF_ASTRO = 0,
	FF_ARIAL,
};

enum astro_flag_t {
	af_Undef = 0,
	af_Asc = 1,
	af_MC = 2,
	af_Dsc = 3,
	af_IC = 4,
	af_Retrograde = 8,
};

static const char* HOUSE_NAMES[] = {
	"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII"
};

namespace astro {
enum {
	ID_SET_ZERO = 1000,
	ID_SET_OCULAR_DIM,
	ID_SET_OCULAR_COLOR,
	ID_CHART_RESET,
	ID_CHART_APPEND,
	ID_CHART_REMOVE,
	ID_FILL_PLANET_LIST,
	ID_GET_DEG_MODE,
	ID_REORDER_LABELS,
	ID_SELECT_LABEL,
};
}
