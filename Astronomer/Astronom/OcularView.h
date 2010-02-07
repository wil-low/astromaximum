#pragma once
#include "WheelView.h"
#include "common.h"

class AstroLabel;

class OcularView : public WheelView
{
	FXDECLARE(OcularView)
public:
	OcularView(FXComposite* p, FXint x, FXint y, FXint w, FXint h);
	virtual ~OcularView(void);
	virtual long onPaint(FXObject*, FXSelector, void*);
	long onCmdSetZero(FXObject*, FXSelector, void*);
	long onCmdSetDimensions(FXObject*, FXSelector, void*);
	long onConfigure(FXObject*, FXSelector, void*);

	void setZeroPoint (int val) {zero_point_ = val;} // ZERO_*

	FXPoint getXYrad(double radian, double len);
	FXPoint getXYdeg(double radian, double len);
	FXPoint getCenter();
	void drawCircle (FXDC& dc, int radius, int x, int y);
	void drawCircle (FXDC& dc, int radius); // centered
protected:
	OcularView(){}
private:
	void reorderLabels();
	int zero_point_;
	float zero_angle_; 
	OcularDimensions dimensions_;
	AstroLabel** zodiac_label_;
};
