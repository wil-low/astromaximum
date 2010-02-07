#include "AstroLabel.h"
#include "GlyphManager.h"

FXDEFMAP(AstroLabel) AstroLabelMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             0, AstroLabel::onPaint),
//	FXMAPFUNC(SEL_LEFTBUTTONRELEASE,           0, AstroLabel::onClicked),
};

FXIMPLEMENT(AstroLabel, FXLabel, AstroLabelMessageMap, ARRAYNUMBER(AstroLabelMessageMap))

AstroLabel::AstroLabel(FXComposite* p,const FXString& text,FXIcon* ic,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb)
: FXLabel (p, text, ic, opts, x, y, w, h, pl, pr, pt, pb)
{
	setBackColor(FXRGBA(255,255,255,0));
	setTextColor(FXRGBA(255,0,0,0));
	setFont(GlyphManager::fntAstro);
	setDefaultCursor(getApp()->getDefaultCursor(DEF_CROSSHAIR_CURSOR));
//	setTipText("It's me");
}

AstroLabel::~AstroLabel(void)
{
}

long AstroLabel::onClicked(FXObject*, FXSelector, void*)
{
	return 1;
}
