#pragma once
#include <fx.h>
class GlyphManager : FXMainWindow
{
	FXDECLARE(GlyphManager)
public:
	GlyphManager(FXApp* a);
	~GlyphManager(void);
	void create();
	
	// Messages for our class
	enum{
		ID_TABLE=FXMainWindow::ID_LAST,
		ID_LAST
	};

	static FXFont* fntAstro;
	FXTable* tabFont;
	virtual long onCmdClose(FXObject*,FXSelector,void*);
protected:
	GlyphManager(){}
};
