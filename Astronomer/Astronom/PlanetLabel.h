#pragma once
#include "AstroLabel.h"

class PlanetLabel : public AstroLabel
{
	FXDECLARE(PlanetLabel)
public:
	PlanetLabel(double lon, DraggableView* p, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
	virtual ~PlanetLabel(void);
	long onClicked(FXObject*, FXSelector, void*);
	long onDrawOnParent(FXObject*, FXSelector, void*);
	double getLon();

//	virtual void position(FXint x, FXint y, FXint w = -1, FXint h = -1);
	PlanetLabel(){}
private:
    int planet_id_;
    double lon_;
};
