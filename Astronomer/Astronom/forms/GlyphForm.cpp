#include "GlyphForm.h"
#include "../utils/GlyphManager.h"
/*
FXDEFMAP(GlyphForm) GlyphFormMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           FXMainWindow::ID_CLOSE,   GlyphForm::onCmdClose),
};
*/
FXIMPLEMENT(GlyphForm, FXMainWindow, 0, 0);//GlyphFormMessageMap, ARRAYNUMBER(GlyphFormMessageMap))

GlyphForm::GlyphForm(FXApp* a)
: FXMainWindow(a,"Glyph Manager",NULL,NULL,DECOR_ALL,0,0,800,600)
{
    setTarget(a);
	tabFont = new FXTable(this, NULL, ID_TABLE, JUSTIFY_CENTER_X|LAYOUT_FILL_X|LAYOUT_FILL_Y);

	tabFont->setEditable(false);
	tabFont->setRowHeaderMode(LAYOUT_MIN_WIDTH);
	tabFont->setTableSize(16, 16);
	for (int r = 0; r < 16; ++r)
		tabFont->setRowHeight(r, 34);
	for (int c = 0; c < 16; ++c)
		tabFont->setColumnWidth(c, 34);

	for (int r = 0; r < 16; ++r) {
		for (int c = 0; c < 16; ++c) {
			tabFont->setItemJustify(r, c, FXTableItem::CENTER_X|FXTableItem::CENTER_Y);
			tabFont->setItemText(r, c, FXString().assign(FXwchar(r * 16 + c)));
		}
	}
}

GlyphForm::~GlyphForm(void)
{
}

// Create and initialize
void GlyphForm::create()
{
	FXMainWindow::create();
	tabFont->setFont(GlyphManager::get_const_instance().getFont(15, FF_ASTRO));
}
