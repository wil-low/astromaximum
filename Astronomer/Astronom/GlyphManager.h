#pragma once
#include <fx.h>
class GlyphManager : FXMainWindow
{
public:
	GlyphManager(FXApp* a);
	~GlyphManager(void);
	// Messages for our class
	enum{
		ID_TABLE=FXMainWindow::ID_LAST,
		ID_LAST
	};

	static FXFont* fntAstro;
	FXTable* tabFont;
};
