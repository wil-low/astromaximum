#pragma once
#include <fx.h>

class MainForm;
class GlyphManager;

class Astronom : public FXApp {
	// Macro for class hierarchy declarations
	FXDECLARE(Astronom)
public:
	Astronom(const FXString& name, const FXString& vendor);
	virtual ~Astronom();
	virtual void create();
	FXFont *fntAstro;
	enum{
		ID_GLYPH=FXApp::ID_LAST,
		ID_LAST
	};
	
	long onCmdGlyph(FXObject*, FXSelector, void*);

private:
	MainForm* fMain;
	GlyphManager* fGlyphManager;
protected:
	Astronom(){}
};
