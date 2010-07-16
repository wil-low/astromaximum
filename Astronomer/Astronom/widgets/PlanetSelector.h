#pragma once
#include <fx.h>

class GlyphManager;
class AstroLabel;

class PlanetSelector : public FXVerticalFrame
{
	FXDECLARE(PlanetSelector)
public:
	PlanetSelector(FXComposite* p, GlyphManager* gm);
	~PlanetSelector();
	enum{
		ID_PLANETS = FXVerticalFrame::ID_LAST,
		ID_DEGMODE,
		ID_LAST
	};
	long onCmdFillPlanetList(FXObject* sender, FXSelector sel, void* ptr);
	long onClickedPlanetList(FXObject*, FXSelector, void*);
	long onCmdGetDegMode(FXObject*, FXSelector, void*);
	long onCmdSetDegMode(FXObject*, FXSelector, void*);

	virtual void create();
protected:
	PlanetSelector(){}
private:
	GlyphManager* gm_;
	FXTabBar* tabbar;
	FXList* lstPlanets;
	void selectAstroLabel(AstroLabel* al);
};
