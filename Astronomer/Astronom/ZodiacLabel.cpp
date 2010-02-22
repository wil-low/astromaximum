#include "ZodiacLabel.h"
#include "constants.h"
/*
FXDEFMAP(ZodiacLabel) PlanetLabelMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_CLICKED,           0, ZodiacLabel::onClicked),
};
*/
FXIMPLEMENT(ZodiacLabel, AstroLabel,
            0,0)
//            PlanetLabelMessageMap, ARRAYNUMBER(PlanetLabelMessageMap))

ZodiacLabel::ZodiacLabel(int sign, DraggableView* p, FXint x, FXint y, FXint w, FXint h)
: AstroLabel(p, x, y, w, h)
, sign_(sign)
{
}

ZodiacLabel::~ZodiacLabel(void)
{
}

double ZodiacLabel::getAngle()
{
	return sign_ * DEG_PER_SIGN + DEG_PER_SIGN / 2;
}

long ZodiacLabel::onClicked(FXObject*, FXSelector, void*)
{
	return 1;
}

int ZodiacLabel::getType()
{
    return TYPE_ZODIAC;
}
