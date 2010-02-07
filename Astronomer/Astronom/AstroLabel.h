#pragma once
#include "DraggableView.h"

class AstroLabel : public FXLabel
{
	FXDECLARE(AstroLabel)
public:
	AstroLabel(FXComposite* p,const FXString& text,FXIcon* ic=0,FXuint opts=LABEL_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);
	virtual ~AstroLabel(void);
	long onClicked(FXObject*, FXSelector, void*);
	enum{
		ID_FRAME = FXLabel::ID_LAST,
		ID_LAST
	};
	AstroLabel(){}
private:
};
