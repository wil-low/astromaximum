#pragma once
#include "BaseModel.h"
#include "../utils/OcularDefs.h"
#include "../utils/HouseProps.h"

class OcularModel : BaseModel
{
public:
	OcularModel();
	virtual ~OcularModel();
	virtual void setView(DraggableView*);
	virtual void setData();
	virtual void setData(const TimeLoc*);
private:
	OcularDimensions dimensions_;
	HouseProps houses_;
};
