#pragma once
#include <fx.h>

// zero point of wheel
enum {
	ZERO_ASC = 0,
	ZERO_ARIES = 1,
};

struct OcularDimensions {
    int radius;
	// all lengths in percents
	int ascArrowR; // main arrow
	int zodiacOuterR; // ring of interleaved signs
	int zodiac10dgrR;
	int zodiac5dgrR;
	int zodiac30dgrR;
	int innerPlanetLabelR;
	int innerPlanetR;
	int zodiacInnerR;
	int aspectR;
};

struct OcularColors {
	FXColor ocularColor;
	FXColor contourColor;
	FXColor mainLineColor;
	FXColor labelColor;
	FXColor fillColor;
	FXColor arrowColor;
	FXColor cuspidColor;
	FXColor tick10Color;
	FXColor innerRColor;
	FXColor planetTickColor;
	FXColor aspectTickColor;
};

namespace astro {
enum {
	ID_SET_ZERO = 1000,
	ID_SET_OCULAR_DIM,
	ID_SET_OCULAR_COLOR,
	ID_UPDATE_CHART,
};
}
