#include "Chrono.h"
#include "../Astronom.h"
/*
FXDEFMAP(GlyphManager) GlyphManagerMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           FXMainWindow::ID_CLOSE,   GlyphManager::onCmdClose),
};
*/
FXIMPLEMENT(Chrono, FXTopWindow, 0, 0);//GlyphManagerMessageMap, ARRAYNUMBER(GlyphManagerMessageMap))

Chrono::Chrono(FXWindow* wo)
: FXTopWindow(wo,"Chrono", NULL, NULL, DECOR_TITLE|DECOR_CLOSE|DECOR_SHRINKABLE|DECOR_STRETCHABLE,
			  100, 100, 200, 400, 0, 0, 0, 0 ,0, 0)
{
	setTarget(getApp());
	FXLabel* label = new FXLabel(this,"Button Frame",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
	label->setBackColor(FXRGB(247,240,255));
	FXSpinner* spinner = new FXSpinner(this, 1);
	spinner->setBackColor(FXRGB(247,240,255));
	setBackColor(FXRGB(247,240,255));
}

Chrono::~Chrono(void)
{
}

