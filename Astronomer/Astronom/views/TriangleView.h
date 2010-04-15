#pragma once
#include "DraggableView.h"

// right angle orientation
enum right_angle_t {
    RIGHT_ANGLE_NW = 0,
    RIGHT_ANGLE_NE,
    RIGHT_ANGLE_SE,
    RIGHT_ANGLE_SW
};

class TriangleView : public DraggableView
{
	FXDECLARE(TriangleView)
public:
	TriangleView(FXComposite* p, FXint x, FXint y, FXint w, FXint h, right_angle_t right_angle);
	virtual long onPaint(FXObject*, FXSelector, void*);
	long onConfigure(FXObject*, FXSelector, void*);
	virtual ~TriangleView(void);

protected:
	virtual hotspot_t hotSpot (FXint x, FXint y, FXbool down, FXDefaultCursor& cursor);
	virtual void dragResize (FXint x, FXint y);
	virtual void dragMove (FXint x, FXint y);

	TriangleView(){}
private:
	FXbool is_right_resize_;
	FXint side_flag_;
	right_angle_t right_angle_;
	FXPoint vertex_[4];
};
