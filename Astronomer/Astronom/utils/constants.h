#pragma once

const int ZODIAC_SIGN_COUNT = 12;
const int DEG_PER_SIGN = 30;
const double DENOMINATOR = 10000.0;

const char UNNAMED_DOC[] = "<Celestial>";
void normAngle(double &a);

const int HOUSE_ID_START = 100000;

enum deg_mode {
	dm_Absolute = 0,
	dm_Longitude,
	dm_RectAsc,
	dm_OblAsc,
	dm_LatDecl,
};

namespace astro {
enum {
	ID_SET_ZERO = 1000,
	ID_SET_OCULAR_DIM,
	ID_SET_OCULAR_COLOR,
	ID_UPDATE_CHART,
	ID_FILL_PLANET_LIST,
	ID_GET_DEG_MODE,
};
}