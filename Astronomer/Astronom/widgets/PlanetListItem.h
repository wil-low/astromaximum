#pragma once
#include <fx.h>
#include "../utils/constants.h"

class AstroLabel;
class PlanetListItem : public FXListItem
{
	FXDECLARE(PlanetListItem)
public:
	PlanetListItem(AstroLabel* data);
	~PlanetListItem();
	virtual void draw(const FXList* list,FXDC& dc,FXint x,FXint y,FXint w,FXint h) const;
	void setDegMode(deg_mode dm);
protected:
	PlanetListItem(){}
private:
	FXString text[3];
};
