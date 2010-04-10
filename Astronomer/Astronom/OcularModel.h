#pragma once
#include "BaseModel.h"
#include "OcularDefs.h"

class OcularModel : BaseModel
{
public:
	OcularModel(Ephemeris* ephemeris);
	virtual ~OcularModel();
	void setView(DraggableView*);
	void incHour();
private:
	OcularDimensions dimensions_;
	int day_;
};
