#pragma once
#include "DraggableView.h"

class WheelView : public DraggableView
{
public:
	WheelView(FXComposite* p, FXuint opts, FXint x, FXint y, FXint w, FXint h);
	virtual ~WheelView(void);
	long onPaint(FXObject* o, FXSelector, void* ptr);
};
