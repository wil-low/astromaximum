#pragma once
#include "DraggableView.h"

class WheelView : public DraggableView
{
	FXDECLARE(WheelView)
public:
	WheelView(FXComposite* p, FXuint opts, FXint x, FXint y, FXint w, FXint h);
	virtual ~WheelView(void);
	enum{
		ID_WHEEL_VIEW = DraggableView::ID_LAST,
		ID_LAST
	};

	long onPaint(FXObject* o, FXSelector, void* ptr);
	long onConfigure(FXObject* o, FXSelector, void* ptr);
	void create();
protected:
	virtual hotspot_t hotSpot (FXint x, FXint y, FXbool down);
	virtual void dragResize (FXint x, FXint y);
	virtual void dragMove (FXint x, FXint y);
	WheelView(){}
private:
	FXint center_x_, center_y_, radius_;
};
