#pragma once
#include <fx.h>
class DraggableView;

class AstroLabel : public FXObject
{
	FXDECLARE(AstroLabel)
public:
	AstroLabel(DraggableView* p, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
	virtual ~AstroLabel(void);
	long onClicked(FXObject*, FXSelector, void*);
	long onDrawOnParent(FXObject*, FXSelector, void*);
	enum{
		ID_FRAME = FXLabel::ID_LAST,
		ID_LAST
	};

	void setText(const FXString& text, FXFont* font);

	virtual void position(FXint x, FXint y, FXint w = -1, FXint h = -1);
	AstroLabel(){}
protected:
    FXRectangle rect_;
    FXFont* font_;
    FXString text_;
};
