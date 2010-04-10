#include "Chrono.h"
#include "Astronom.h"
/*
FXDEFMAP(GlyphManager) GlyphManagerMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           FXMainWindow::ID_CLOSE,   GlyphManager::onCmdClose),
};
*/
FXIMPLEMENT(Chrono, FXTopWindow, 0, 0);//GlyphManagerMessageMap, ARRAYNUMBER(GlyphManagerMessageMap))

Chrono::Chrono(FXWindow* wo)
: FXTopWindow(wo,"Chrono", NULL, NULL, DECOR_TITLE|DECOR_CLOSE|DECOR_BORDER|DECOR_SHRINKABLE|DECOR_STRETCHABLE,
			  100, 100, 200, 400, 0, 0, 0, 0 ,0, 0)
{
	setTarget(getApp());
	new FXLabel(this,"Button Frame",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);
	new FXSpinner(this, 1);
}

Chrono::~Chrono(void)
{
}

