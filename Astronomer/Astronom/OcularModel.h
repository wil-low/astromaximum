#pragma once
#include "OcularDefs.h"
class DraggableView;

class OcularModel
{
public:
	OcularModel();
	virtual ~OcularModel(void);
	void setView(DraggableView*);
private:
	DraggableView* view_;
	OcularDimensions dimensions_;
};
