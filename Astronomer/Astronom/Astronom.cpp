#include "Astronom.h"
#include "MainForm.h"
#include "GlyphManager.h"
#include "OcularModel.h"

FXDEFMAP(Astronom) AstronomMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_QUERY_TIP,         0,     Astronom::onQueryTip),
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_GLYPH,     Astronom::onCmdGlyph),
	FXMAPTYPE(SEL_CLOSE,             Astronom::onCmdClose),
};

FXIMPLEMENT(Astronom, FXApp, AstronomMessageMap, ARRAYNUMBER(AstronomMessageMap))

Astronom::Astronom(const FXString& name, const FXString& vendor)
: FXApp (name, vendor)
, fntAstro(NULL)
, fMain(NULL)
, fGlyphManager(NULL)
, tooltip_(NULL)
{
	tooltip_ = new FXToolTip(this);
	mOcular = new OcularModel;
	fntAstro = new FXFont(this, "Astronom",
		10, FXFont::Normal, FXFont::Straight,FONTENCODING_UNICODE);
	fGlyphManager = new GlyphManager(this);
	fMain = new MainForm(this);
}

void Astronom::create()
{
	FXApp::create();
	fMain->show();
	fMain->maximize();
//	popup_->show();
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

long Astronom::onQueryTip(FXObject* o, FXSelector, void* ptr)
{
//	tooltip_->handle(o, FXSEL(SEL_UPDATE, 0), ptr);
    tooltip_->hide();
	tooltip_->show();//handle(o, FXSEL(SEL_TIMEOUT, FXToolTip::ID_TIP_HIDE), ptr);
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
