#include "Astronom.h"
#include "Localizer.h"
#include "Ephemeris.h"
#include "forms/MainForm.h"
#include "forms/InputForm.h"
#include "forms/PersonsForm.h"
#include "forms/Chrono.h"
#include "forms/GlyphForm.h"
#include "models/OcularModel.h"
#include "utils/GlyphManager.h"

FXDEFMAP(Astronom) AstronomMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_QUERY_TIP,         0,                      Astronom::onQueryTip),
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_INPUTDATA, Astronom::onCmdInputData),
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_PERSONS,   Astronom::onCmdPersons),
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_GLYPH,     Astronom::onCmdGlyph),
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_CHRONO,    Astronom::onCmdToggleChrono),
	FXMAPFUNC(SEL_COMMAND,           Astronom::ID_INC_HOUR,  Astronom::onCmdIncHour),

	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_INPUT_ACCEPT, Astronom::onCmdInputAccept),
	FXMAPTYPE(SEL_CLOSE,             Astronom::onCmdClose),
};

FXIMPLEMENT(Astronom, FXApp, AstronomMessageMap, ARRAYNUMBER(AstronomMessageMap))

Astronom::Astronom(const FXString& name, const FXString& vendor)
: FXApp (name, vendor)
, fMain(NULL)
, fGlyph(NULL)
, tooltip_(NULL)
{
    Localizer *localizer = new Localizer();
    localizer->load_lang ("settings/ru.lng");
	setTranslator(localizer);
	char ephe_path[256] = "rerye";
	Ephemeris::init (ephe_path);
	tooltip_ = new FXToolTip(this);
	mOcular = new OcularModel();
	fGlyph = new GlyphForm(this);
	fMain = new MainForm(this);
	fInputData = new InputForm(fMain);
	fPersons = new PersonsForm(fMain);
	fChrono = new Chrono(fMain);
}

void Astronom::create()
{
	GlyphManager::get_mutable_instance().init(this);
	FXApp::create();
	TimeLoc::initRex('.');
	fInputData->init();
	fMain->init();
	fMain->show();
	fMain->maximize();
//	popup_->show();
}

Astronom::~Astronom()
{
	delete mOcular;
	Ephemeris::fini();
	GlyphManager::get_mutable_instance().fini();
}

long Astronom::onCmdInputData(FXObject*, FXSelector, void*)
{
	fInputData->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_SHOW), NULL);
	fInputData->execute(PLACEMENT_SCREEN);
	return 1;
}

long Astronom::onCmdPersons(FXObject*, FXSelector, void*)
{
	fPersons->show();
	fPersons->execute(PLACEMENT_SCREEN);
	return 1;
}

long Astronom::onCmdGlyph(FXObject*, FXSelector, void*)
{
	fGlyph->show(PLACEMENT_SCREEN);//handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_SHOW), NULL);
	return 1;
}

long Astronom::onCmdToggleChrono(FXObject*, FXSelector, void*)
{
	if (fChrono->shown())
		fChrono->hide();
	else
		fChrono->show(PLACEMENT_SCREEN);
	return 1;
}

long Astronom::onCmdIncHour(FXObject*, FXSelector, void*)
{
//	mOcular->incHour();
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
	fInputData->onCmdAccept(0, 0, 0);
	mOcular->setData();
}

long Astronom::onCmdInputAccept(FXObject*, FXSelector, void* ptr)
{
	mOcular->setData((const TimeLoc*) ptr);
	return 1;
}
