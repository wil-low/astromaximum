#pragma once
#include "DraggableView.h"

class RectangleView : public DraggableView
{
	FXDECLARE(RectangleView)
public:
	RectangleView(FXComposite* p, FXint x, FXint y, FXint w, FXint h);
	virtual long onPaint(FXObject*, FXSelector, void*);
	virtual ~RectangleView(void);
protected:
	virtual hotspot_t hotSpot (FXint x, FXint y, FXbool down, FXDefaultCursor& cursor);
	virtual void dragResize (FXint x, FXint y);
	virtual void dragMove (FXint x, FXint y);

	RectangleView(){}
private:
	FXbool is_right_resize_;
	FXint side_flag_;
};
