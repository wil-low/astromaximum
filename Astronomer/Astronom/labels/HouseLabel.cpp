#include "HouseLabel.h"
#include "../utils/constants.h"

FXDEFMAP(HouseLabel) HouseLabelMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             0, HouseLabel::onDrawOnParent),
	FXMAPFUNC(SEL_CLICKED,           0, HouseLabel::onClicked),
};

FXIMPLEMENT(HouseLabel, AstroLabel, HouseLabelMessageMap, ARRAYNUMBER(HouseLabelMessageMap))

HouseLabel::HouseLabel(DraggableView* p, astro_flag_t flag, FXint x, FXint y, FXint w, FXint h)
: AstroLabel(p, x, y, w, h)
{
	setFlags(flag);
}

HouseLabel::~HouseLabel(void)
{
}

double HouseLabel::getAngle() const
{
	return props_.prop[BodyProps::bp_Lon];
}

long HouseLabel::onClicked(FXObject*, FXSelector, void*)
{
	return 1;
}

body_type_t HouseLabel::getType() const
{
    return TYPE_HOUSE;
}

double HouseLabel::getVisibleAngle() const
{
	return visibleLon_;
}

void HouseLabel::setVisibleAngle(double ang)
{
    visibleLon_ = ang;
}

astro_flag_t HouseLabel::flagOfHouse(int num, int cusp_count)
{
	astro_flag_t af = af_Undef;
	if (num == (1 + 0 * cusp_count / 4));
//		af = af_Asc;
	else if (num == (1 + 1 * cusp_count / 4))
		af = af_IC;
	else if (num == (1 + 2 * cusp_count / 4))
		af = af_Dsc;
//	else if (num == (1 + 3 * cusp_count / 4))
//		af = af_MC;
	return af;
}
