#pragma once
#include <fx.h>
#include <map>

class MainForm;
class GlyphManager;
class OcularModel;
class DraggableView;

class Astronom : public FXApp {
	// Macro for class hierarchy declarations
	FXDECLARE(Astronom)
public:
	Astronom(const FXString& name, const FXString& vendor);
	virtual ~Astronom();
	virtual void create();

	void setOcular(DraggableView* dv);
	FXFont* getAstroFont (int size);

	enum{
		ID_GLYPH=FXApp::ID_LAST,
		ID_LAST
	};

	GlyphManager* fGlyphManager;
	FXImage* offscreen;

	long onCmdGlyph(FXObject*, FXSelector, void*);
	long onCmdClose(FXObject*, FXSelector, void*);
	long onQueryTip(FXObject*, FXSelector, void*);
private:
	void clearFonts();
	void loadFont(const FXString& face);

	MainForm* fMain;
	OcularModel* mOcular;
	FXToolTip* tooltip_;
	std::map<int, FXFont*> astrofont_map_;
protected:
	Astronom(){}
};
