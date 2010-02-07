#pragma once

const int ZODIAC_SIGN_COUNT = 12;
// zero point of wheel
enum {
	ZERO_ASC = 0,
	ZERO_ARIES = 1
};

struct OcularDimensions {
	// all lengths in percents
	int ascArrowLen;
	int cuspidLen;
	int homeLen;
	int zodiacOuterLen;
	int zodiacInnerLen;
	OcularDimensions(){}
	OcularDimensions(int asc, int cusp, int home, int zouter, int zinner)
		: ascArrowLen(asc), cuspidLen(cusp), homeLen(home), zodiacOuterLen(zouter), zodiacInnerLen(zinner)
	{}
};

namespace astro {
enum {
	ID_SET_ZERO = 1000,
	ID_SET_OCULAR_DIM,
};
}
