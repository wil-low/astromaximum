#pragma once
#include "AstroLabel.h"

class HouseLabel : public AstroLabel
{
	FXDECLARE(HouseLabel)
public:
	HouseLabel(DraggableView* p, astro_flag_t flag, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
	virtual ~HouseLabel(void);
	long onClicked(FXObject*, FXSelector, void*);
	virtual double getAngle() const;
    virtual body_type_t getType() const;
	virtual double getVisibleAngle() const;

	virtual void setVisibleAngle(double ang);
	HouseLabel(){}

	static astro_flag_t flagOfHouse(int num, int cusp_count);

private:
    int planet_id_;
    double visibleLon_;
};
