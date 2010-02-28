#pragma once
#include <fx.h>
class GlyphManager : public FXMainWindow
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

	FXchar getSignLabel(int sign);
	FXchar getPlanetLabel(int planet);

	FXFont* getFont(int size) const;
	FXTable* tabFont;
protected:
	GlyphManager(){}
};
