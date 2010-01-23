#include "GlyphManager.h"

FXFont* GlyphManager::fntAstro = NULL;

GlyphManager::GlyphManager(FXApp* a)
: FXMainWindow(a,"Glyph Manager",NULL,NULL,DECOR_ALL,0,0,800,600)
{
	fntAstro = new FXFont(getApp(), "Astronom", //"HamburgSymbols",
		20, FXFont::Normal, FXFont::Straight,FONTENCODING_UNICODE);//""WinStarTT");
	fntAstro->create();
	tabFont = new FXTable(this, NULL, ID_TABLE, JUSTIFY_CENTER_X|LAYOUT_FILL_X|LAYOUT_FILL_Y);

	tabFont->setFont(fntAstro);
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
