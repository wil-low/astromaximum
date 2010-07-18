#pragma once
#include <fx.h>
#include "../utils/constants.h"

class AstroLabel;

class PlanetSelector : public FXVerticalFrame
{
	FXDECLARE(PlanetSelector)
public:
	PlanetSelector(FXComposite* p);
	~PlanetSelector();
	enum{
		ID_PLANETS = FXVerticalFrame::ID_LAST,
		ID_DEGMODE,
		ID_LAST
	};
	long onCmdFillPlanetList(FXObject* sender, FXSelector sel, void* ptr);
	long onListSelChanged(FXObject*, FXSelector, void*);
	long onRBtnRelease(FXObject*,FXSelector,void*);
	long onCmdGetDegMode(FXObject*, FXSelector, void*);
	long onCmdSetDegMode(FXObject*, FXSelector, void*);

	virtual void create();
protected:
	PlanetSelector(){}
private:
	FXTabBar* tabbar;
	FXList* lstPlanets;
	deg_mode deg_mode_;
	void selectAstroLabel(AstroLabel* al);
};
