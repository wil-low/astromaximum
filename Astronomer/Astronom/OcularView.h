#pragma once
#include "WheelView.h"
#include "common.h"
#include <vector>

class AstroLabel;
class Chart;
class OcularView : public WheelView
{
	FXDECLARE(OcularView)
public:
	OcularView(FXComposite* p, FXint x, FXint y, FXint r);
	virtual ~OcularView(void);

	void create();
	virtual long onPaint(FXObject*, FXSelector, void*);
	long onCmdSetZero(FXObject*, FXSelector, void*);
	long onCmdSetDimensions(FXObject*, FXSelector, void*);
	long onCmdSetColors(FXObject*, FXSelector, void* ptr);
	long onCmdUpdateChart(FXObject*, FXSelector, void* ptr);
	long onConfigure(FXObject*, FXSelector, void*);

	void setZeroPoint (int val) {zero_point_ = val;} // ZERO_*

	FXPoint getXYrad(double radian, double len);
	FXPoint getXYdeg(double radian, double len);
	FXPoint getCenter();
	void drawCircle (FXDC& dc, int radius, int x, int y);
	void drawCircle (FXDC& dc, int radius); // centered
	void fillArc (FXDC& dc, int radius, int ang1, int ang2);
	void fillCircle (FXDC& dc, int radius);
protected:
	OcularView(){}
private:
	typedef std::vector<AstroLabel*> AstroLabelVector;
    void drawLabels (FXDC& dc, const AstroLabelVector& ar);
	void reorderLabels();
	void reorderPlanets(int chart_id, const Chart* chart);

	int zero_point_;
	float zero_angle_;
	OcularDimensions dimensions_;
	OcularColors colors_;

	AstroLabelVector zodiac_list_, planet_list_, aspect_list_;
};
