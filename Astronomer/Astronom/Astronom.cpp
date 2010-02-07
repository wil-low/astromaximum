#include "Astronom.h"
#include "MainForm.h"
#include "GlyphManager.h"
#include "OcularModel.h"

FXDEFMAP(Astronom) AstronomMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_GLYPH,     Astronom::onCmdGlyph),
	FXMAPTYPE(SEL_CLOSE,             Astronom::onCmdClose),
};

FXIMPLEMENT(Astronom, FXApp, AstronomMessageMap, ARRAYNUMBER(AstronomMessageMap))

Astronom::Astronom(const FXString& name, const FXString& vendor)
: FXApp (name, vendor)
, fntAstro(NULL)
, fMain(NULL)
, fGlyphManager(NULL)
{
	mOcular = new OcularModel;
}

void Astronom::create()
{
	FXApp::create();
	new FXToolTip(this);
	fntAstro = new FXFont(this, "Astronom",
		28, FXFont::Normal, FXFont::Straight,FONTENCODING_UNICODE);
	fntAstro->create();
	// Create the application's windows
	fGlyphManager = new GlyphManager(this);
	fGlyphManager->create();
	fMain = new MainForm(this);
	fMain->create();
	fMain->maximize();
}

Astronom::~Astronom()
{
	delete fntAstro;
	delete mOcular;
}

long Astronom::onCmdGlyph(FXObject*, FXSelector, void*)
{
	fGlyphManager->show(PLACEMENT_SCREEN);//handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_SHOW), NULL);
	return 1;
}

long Astronom::onCmdClose(FXObject* o, FXSelector, void*)
{
    if (o == fMain)
        handle (this, FXSEL(SEL_COMMAND, ID_QUIT), NULL);
    else
        o->handle (this, FXSEL(SEL_COMMAND, FXWindow::ID_HIDE), NULL);
	return 1;
}

void Astronom::setOcular(DraggableView* dv)
{
	mOcular->setView(dv);
}
