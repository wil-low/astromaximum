#pragma once
#include "WheelView.h"
#include "../utils/OcularDefs.h"
#include "../labels/AstroLabelContainer.h"
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
    long onQueryTip(FXObject*, FXSelector, void*);
    long onQueryHelp(FXObject*, FXSelector, void*);
    long onMouseMove(FXObject*, FXSelector, void*);
    long onRightBtnPress(FXObject*, FXSelector, void*);

	void setZeroPoint (ZeroPoint val) {zero_point_ = val;} // ZERO_*

	FXPoint getXYrad(double radian, double len);
	FXPoint getXYdeg(double radian, double len);
	FXPoint getCenter();
	void drawCircle (FXDC& dc, double radius, double x, double y);
	void drawCircle (FXDC& dc, double radius); // centered
	void fillArc (FXDC& dc, double radius, double ang1, double ang2);
	void fillCircle (FXDC& dc, double radius);
protected:
	OcularView(){}
	virtual void dragResize (FXint x, FXint y);
private:
	typedef std::vector<AstroLabel*> AstroLabelVector;

    void drawLabels (FXDC& dc);
	void drawAspects(FXDC& dc);
	void drawPlanetLines(FXDC& dc);
    void drawHouseLines(FXDC& dc);

	void reorderLabels();

	void spreadLabels (int chart, AstroLabel::label_type_t type, double r);
	void addHouse (int chart_id, int id, int cusp_count);

	ZeroPoint zero_point_;
	double zero_angle_;
	OcularDimensions dimensions_;
	OcularColors colors_;

	AstroLabelContainer labels_;
	AstroLabel* cur_label_;
    FXPopup* popup_;
	bool is_resizing_;
};
