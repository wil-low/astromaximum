#pragma once
#include "AstroLabel.h"

class ZodiacLabel : public AstroLabel
{
	FXDECLARE(ZodiacLabel)
public:
	ZodiacLabel(int sign, DraggableView* p, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
	virtual ~ZodiacLabel(void);
	virtual long onClicked(FXObject*, FXSelector, void*);
	virtual double getAngle();
    virtual int getType();
//	virtual void position(FXint x, FXint y, FXint w = -1, FXint h = -1);
	ZodiacLabel(){}
private:
    int sign_;
};
