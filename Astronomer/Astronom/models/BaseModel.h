#pragma once
class DraggableView;
#include "../TimeLoc.h"

class BaseModel
{
public:
	BaseModel();
	virtual ~BaseModel();
	virtual void setView(DraggableView*) = 0;
	virtual void setData() {};
	virtual void addBody(int chart_id, int body) {};
	virtual void removeBody(int chart_id, int body) {};
	virtual void setData(const TimeLoc*) = 0;
protected:
	DraggableView* view_;
	TimeLoc timeloc_;
};
