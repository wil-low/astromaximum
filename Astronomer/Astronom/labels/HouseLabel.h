#pragma once
#include "AstroLabel.h"

class HouseLabel : public AstroLabel
{
	FXDECLARE(HouseLabel)
public:
	enum HouseFlag {
		hf_Undef = 0,
		hf_Asc,
		hf_MC,
		hf_Dsc,
		hf_IC,
	};
	HouseLabel(DraggableView* p, HouseFlag flag, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
	virtual ~HouseLabel(void);
	long onClicked(FXObject*, FXSelector, void*);
	virtual double getAngle() const;
    virtual label_type_t getType() const;
	virtual double getVisibleAngle() const;

	virtual void setVisibleAngle(double ang);
	HouseLabel(){}

	static HouseFlag flagOfHouse(int num, int cusp_count);

private:
    int planet_id_;
    double visibleLon_;
};
