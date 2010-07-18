#pragma once
#include <fx.h>
#include "../utils/constants.h"

class AstroLabel;
class ExtraBodyListItem : public FXListItem
{
	FXDECLARE(ExtraBodyListItem)
public:
	ExtraBodyListItem(int id);
	~ExtraBodyListItem();
	virtual void draw(const FXList* list,FXDC& dc,FXint x,FXint y,FXint w,FXint h) const;
	void setDegMode(const FXList* list, deg_mode dm);
protected:
	ExtraBodyListItem(){}
private:
    deg_mode deg_mode_;
	int body_id_;
	FXString text[3];
};
