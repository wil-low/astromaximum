#pragma once
#include "BaseModel.h"
#include "../utils/OcularDefs.h"

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
};
