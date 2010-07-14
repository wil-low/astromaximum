#pragma once
#include <fx.h>
class GlyphManager;

class DraggableView : public FXCanvas
{
	FXDECLARE(DraggableView)
public:
	DraggableView(FXComposite* p, FXint x=0,FXint y=0,FXint w=0,FXint h=0);

	virtual ~DraggableView(void);
	virtual long onPaint(FXObject*, FXSelector, void*);
	long onMouseDown(FXObject*, FXSelector,void*);
	long onMouseUp(FXObject*, FXSelector, void*);
	long onMouseMove(FXObject*, FXSelector, void*);
	long onCmdLock(FXObject*, FXSelector, void*);
	FXColor            drawColor;               // Color for the line

	void setGlyphManager(GlyphManager* gm);
	// Messages for our class
	enum{
		ID_LOCK = FXCanvas::ID_LAST,
		ID_LAST
	};
    static double distance(FXint x1, FXint y1, FXint x2, FXint y2);
protected:
	enum hotspot_t {
		HS_NONE = 0,
		HS_MOVE,
		HS_RESIZE
	};
	virtual hotspot_t hotSpot (FXint x, FXint y, FXbool down, FXDefaultCursor& cursor);
	virtual void dragResize (FXint /*x*/, FXint /*y*/) {}
	virtual void dragMove (FXint /*x*/, FXint /*y*/) {}

	hotspot_t mouse_flag_;                  // Mouse flag
	FXbool is_locked_;
	FXint pivot_x_, pivot_y_;
	static const FXint MOUSE_SENSITIVITY;
	GlyphManager* glyph_manager_;
protected:
	DraggableView(){}
};
