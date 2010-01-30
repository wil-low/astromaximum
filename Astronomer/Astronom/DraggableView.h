#pragma once
#include <fx.h>

class DraggableView : public FXCanvas
{
	FXDECLARE(DraggableView)
public:
	DraggableView(FXComposite* p, FXuint opts=FRAME_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

	virtual ~DraggableView(void);
	virtual long onPaint(FXObject*, FXSelector, void*);
	long onMouseDown(FXObject*, FXSelector,void*);
	long onMouseUp(FXObject*, FXSelector, void*);
	long onMouseMove(FXObject*, FXSelector, void*);
	FXColor            drawColor;               // Color for the line

	// Messages for our class
	enum{
		ID_VIEW = FXCanvas::ID_LAST,
		ID_LAST
	};

protected:
	enum hotspot_t {
		HS_NONE = 0,
		HS_MOVE,
		HS_RESIZE
	};
	virtual hotspot_t hotSpot (FXint x, FXint y, FXbool down, FXDefaultCursor& cursor);
	virtual void dragResize (FXint x, FXint y) {}
	virtual void dragMove (FXint x, FXint y) {}

	hotspot_t mouse_flag_;                  // Mouse flag
	FXint pivot_x_, pivot_y_;
	static const FXint MOUSE_SENSITIVITY;
protected:
	DraggableView(){}
};
