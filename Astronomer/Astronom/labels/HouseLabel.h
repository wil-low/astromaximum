#pragma once
#include "AstroLabel.h"

class HouseLabel : public AstroLabel
{
	FXDECLARE(HouseLabel)
public:
	HouseLabel(DraggableView* p, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
	virtual ~HouseLabel(void);
	long onClicked(FXObject*, FXSelector, void*);
	virtual double getAngle() const;
    virtual label_type_t getType() const;
	virtual double getVisibleAngle() const;

	virtual void setAngle(double ang);
	virtual void setVisibleAngle(double ang);
	HouseLabel(){}
private:
    int planet_id_;
    double lon_;
    double visibleLon_;
};
