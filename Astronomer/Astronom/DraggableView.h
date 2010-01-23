#pragma once
#include <fx.h>
class DraggableView : public FXCanvas
{
	FXDECLARE(DraggableView)
public:
	DraggableView(FXComposite* p,FXuint opts=FRAME_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

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

private:
	enum mouse_flag_t {
		MF_NONE = 0,
		MF_DOWN,
		MF_MOVE,
		MF_RESIZE
	};
	mouse_flag_t mouse_flag_;                  // Mouse flag
protected:
	DraggableView(){}
};
