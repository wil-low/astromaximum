#include "Astronom.h"
#include "Ephemeris.h"
#include "forms/MainForm.h"
#include "forms/Chrono.h"
#include "forms/GlyphManager.h"
#include "models/OcularModel.h"

FXDEFMAP(Astronom) AstronomMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_QUERY_TIP,         0,                      Astronom::onQueryTip),
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_GLYPH,     Astronom::onCmdGlyph),
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_CHRONO,    Astronom::onCmdToggleChrono),
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_INC_HOUR,  Astronom::onCmdIncHour),
	FXMAPTYPE(SEL_CLOSE,             Astronom::onCmdClose),
};

FXIMPLEMENT(Astronom, FXApp, AstronomMessageMap, ARRAYNUMBER(AstronomMessageMap))

Astronom::Astronom(const FXString& name, const FXString& vendor)
: FXApp (name, vendor)
, fMain(NULL)
, fGlyphManager(NULL)
, tooltip_(NULL)
{
	char ephe_path[256] = "rerye";
	ephemeris = new Ephemeris (ephe_path);
	tooltip_ = new FXToolTip(this);
	mOcular = new OcularModel(ephemeris);
	fGlyphManager = new GlyphManager(this);
	fMain = new MainForm(this);
	chrono_ = new Chrono(fMain);
}

void Astronom::create()
{
	loadFont("Astronom");
	FXApp::create();
	fMain->show();
	fMain->maximize();
//	popup_->show();
}

Astronom::~Astronom()
{
	delete mOcular;
	clearFonts();
	delete ephemeris;
}

long Astronom::onCmdGlyph(FXObject*, FXSelector, void*)
{
	fGlyphManager->show(PLACEMENT_SCREEN);//handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_SHOW), NULL);
	return 1;
}

long Astronom::onCmdToggleChrono(FXObject*, FXSelector, void*)
{
	if (chrono_->shown())
		chrono_->hide();
	else
		chrono_->show(PLACEMENT_SCREEN);
	return 1;
}

long Astronom::onCmdIncHour(FXObject*, FXSelector, void*)
{
	mOcular->incHour();
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

FXFont* Astronom::getAstroFont (int size)
{
	FXFont* fnt = NULL;
	std::map<int, FXFont*>::iterator it = astrofont_map_.lower_bound(size);
	if (it != astrofont_map_.end())
		fnt = it->second;
	else
		fnt = astrofont_map_.rbegin()->second;
	return fnt;
}

void Astronom::clearFonts()
{
	for (std::map<int, FXFont*>::iterator it = astrofont_map_.begin(); it != astrofont_map_.end(); ++it)
		delete (*it).second;
	astrofont_map_.clear();
}

void Astronom::loadFont(const FXString& face)
{
	const int FONT_SIZES[] = {8, 9, 10, 11, 12, 13, 14, 16, 18, 22, 30, 36, 40, 48, 56, 60};
	clearFonts();
	for (int i = 0; i < ARRAYNUMBER(FONT_SIZES); ++i) {
		FXFont* fnt = new FXFont(this, face,
			FONT_SIZES[i], FXFont::Normal, FXFont::Straight, FONTENCODING_UNICODE);
		if (fnt != NULL) {
			fnt->create();
			astrofont_map_[FONT_SIZES[i]] = fnt;
		}
	}
}
