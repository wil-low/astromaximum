#ifndef OCULAR_DEFS_H
#define OCULAR_DEFS_H
#include <fx.h>

// zero point of wheel
enum ZeroPoint {
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
	int zodiacInner2R;
	int zodiacInnerR;
	int aspectR;
	int planetFontSize;
	int zodiacFontSize;
	int degreeFontSize;
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

#endif
