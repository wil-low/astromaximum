#include "HouseLabel.h"

FXDEFMAP(HouseLabel) HouseLabelMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             0, HouseLabel::onDrawOnParent),
	FXMAPFUNC(SEL_CLICKED,           0, HouseLabel::onClicked),
};

FXIMPLEMENT(HouseLabel, AstroLabel, HouseLabelMessageMap, ARRAYNUMBER(HouseLabelMessageMap))

HouseLabel::HouseLabel(DraggableView* p, HouseFlag flag, FXint x, FXint y, FXint w, FXint h)
: AstroLabel(p, x, y, w, h)
, lon_(0)
{
	setFlags(flag);
}

HouseLabel::~HouseLabel(void)
{
}

double HouseLabel::getAngle() const
{
	return lon_;
}

long HouseLabel::onClicked(FXObject*, FXSelector, void*)
{
	return 1;
}

AstroLabel::label_type_t HouseLabel::getType() const
{
    return TYPE_HOUSE;
}

double HouseLabel::getVisibleAngle() const
{
	return visibleLon_;
}

void HouseLabel::setAngle(double ang)
{
	lon_ = ang;
}

void HouseLabel::setVisibleAngle(double ang)
{
    visibleLon_ = ang;
}

HouseLabel::HouseFlag HouseLabel::flagOfHouse(int num, int cusp_count)
{
	HouseFlag hf = hf_Undef;
	if (num == (1 + 0 * cusp_count / 4))
		hf = hf_Asc;
	else if (num == (1 + 1 * cusp_count / 4))
		hf = hf_IC;
	else if (num == (1 + 2 * cusp_count / 4))
		hf = hf_Dsc;
	else if (num == (1 + 3 * cusp_count / 4))
		hf = hf_MC;
	return hf;
}
