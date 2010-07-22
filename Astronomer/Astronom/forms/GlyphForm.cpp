#include "GlyphForm.h"
#include "../utils/GlyphManager.h"

FXDEFMAP(GlyphForm) GlyphFormMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_DOUBLECLICKED, GlyphForm::ID_TABLE,   GlyphForm::onTableDblClicked),
};

FXIMPLEMENT(GlyphForm, FXMainWindow, GlyphFormMessageMap, ARRAYNUMBER(GlyphFormMessageMap))

GlyphForm::GlyphForm(FXApp* a)
: FXMainWindow(a,"Glyph Manager",NULL,NULL,DECOR_ALL,0,0,800,600)
{
    setTarget(a);
	tabFont = new FXTable(this, this, ID_TABLE, JUSTIFY_CENTER_X|LAYOUT_FILL_X|LAYOUT_FILL_Y);

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
	tabFont->setFont(GlyphManager::get_const_instance().getFont(15, FF_ARIAL));
}

long GlyphForm::onTableDblClicked(FXObject*, FXSelector, void* ptr)
{
	int row = tabFont->getSelStartRow(), col = tabFont->getSelStartColumn();
	if (row != -1 && col != -1) {
		int pos = row * tabFont->getNumColumns() + col;
		FXString s;
		s.format("%d ('%c')", pos, pos);
		FXMessageBox::information(getApp(), MBOX_OK, "Astronom", s.text());
	}
	return 1;
}
