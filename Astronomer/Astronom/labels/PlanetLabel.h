#pragma once
#include "AstroLabel.h"

class PlanetLabel : public AstroLabel
{
	FXDECLARE(PlanetLabel)
public:
	PlanetLabel(DraggableView* p, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
	virtual ~PlanetLabel(void);
	long onClicked(FXObject*, FXSelector, void*);
	virtual double getAngle() const;
    virtual body_type_t getType() const;
	virtual double getVisibleAngle() const;

	virtual void setVisibleAngle(double ang);
	virtual void setProps(const BodyProps& props);
	PlanetLabel(){}
private:
    int planet_id_;
    double visibleLon_;
};
