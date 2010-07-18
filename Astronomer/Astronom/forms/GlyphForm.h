#pragma once
#include <fx.h>
#include "../utils/constants.h"

class GlyphForm : public FXMainWindow
{
	FXDECLARE(GlyphForm)
public:
	GlyphForm(FXApp* a);
	~GlyphForm(void);
	void create();
	
	// Messages for our class
	enum{
		ID_TABLE=FXMainWindow::ID_LAST,
		ID_LAST
	};

	long onTableDblClicked(FXObject*, FXSelector, void*);

protected:
	GlyphForm(){}
private:
	FXTable* tabFont;
};
