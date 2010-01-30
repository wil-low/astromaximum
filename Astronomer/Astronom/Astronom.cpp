#include "Astronom.h"
#include "MainForm.h"
#include "GlyphManager.h"

FXDEFMAP(Astronom) AstronomMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_GLYPH,     Astronom::onCmdGlyph),
};

FXIMPLEMENT(Astronom, FXApp, AstronomMessageMap, ARRAYNUMBER(AstronomMessageMap))

Astronom::Astronom(const FXString& name, const FXString& vendor)
: FXApp (name, vendor)
{
	fntAstro = new FXFont(this, "Astronom",
		20, FXFont::Normal, FXFont::Straight,FONTENCODING_UNICODE);
	fntAstro->create();
}

void Astronom::create()
{
	FXApp::create();
	// Create the application's windows
	fMain = new MainForm(this);
	fMain->create();
	fGlyphManager = new GlyphManager(this);
	fGlyphManager->create();
}

Astronom::~Astronom()
{
	delete fntAstro;
}

long Astronom::onCmdGlyph(FXObject*, FXSelector, void*)
{
	fGlyphManager->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_SHOW), NULL);
	return 1;
}
