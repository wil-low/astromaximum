#include "GlyphManager.h"
#include "Astronom.h"
/*
FXDEFMAP(GlyphManager) GlyphManagerMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           FXMainWindow::ID_CLOSE,   GlyphManager::onCmdClose),
};
*/
FXIMPLEMENT(GlyphManager, FXMainWindow, 0, 0);//GlyphManagerMessageMap, ARRAYNUMBER(GlyphManagerMessageMap))

GlyphManager::GlyphManager(FXApp* a)
: FXMainWindow(a,"Glyph Manager",NULL,NULL,DECOR_ALL,0,0,800,600)
{
    setTarget(a);
	fntAstro_ = dynamic_cast<Astronom*>(a)->fntAstro;
	tabFont = new FXTable(this, NULL, ID_TABLE, JUSTIFY_CENTER_X|LAYOUT_FILL_X|LAYOUT_FILL_Y);

	tabFont->setFont(fntAstro_);
	tabFont->setEditable(false);
	tabFont->setRowHeaderMode(LAYOUT_MIN_WIDTH);
	tabFont->setTableSize(16, 16);
	for (int i = 0; i < 16; ++i)
		tabFont->setRowHeight(i, 34);
	for (int i = 0; i < 16; ++i)
		tabFont->setColumnWidth(i, 34);

	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			tabFont->setItemText(i, j, FXString().assign(FXwchar(i * 16 + j)));
		}
	}
}

GlyphManager::~GlyphManager(void)
{
}

FXFont* GlyphManager::getFont() const
{
	return fntAstro_;
}

// Create and initialize
void GlyphManager::create()
{
	// Create the windows
	FXMainWindow::create();
}

FXchar GlyphManager::getSignLabel(int sign)
{
	return sign + '@';
}

FXchar GlyphManager::getPlanetLabel(int planet)
{
	return planet + '0' + 32;
}
