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
	virtual void addBody(int chart_id, int body);
	virtual void removeBody(int chart_id, int body);
private:
	OcularDimensions dimensions_;
	HouseProps houses_;
};
