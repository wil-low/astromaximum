#include "AstroLabel.h"
#include "GlyphManager.h"
#include "DraggableView.h"

FXDEFMAP(AstroLabel) AstroLabelMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             0, AstroLabel::onDrawOnParent),
	FXMAPFUNC(SEL_CLICKED,           0, AstroLabel::onClicked),
};

FXIMPLEMENT(AstroLabel, FXObject, AstroLabelMessageMap, ARRAYNUMBER(AstroLabelMessageMap))

AstroLabel::AstroLabel(DraggableView* p, const FXString& text, FXint x, FXint y, FXint w, FXint h)
: rect_(x, y, w, h)
, font_(GlyphManager::fntAstro)
, text_(text)
{
}

AstroLabel::~AstroLabel(void)
{
}

long AstroLabel::onClicked(FXObject*, FXSelector, void*)
{
	return 1;
}

long AstroLabel::onDrawOnParent(FXObject*, FXSelector, void* ptr)
{
    FXTRACE((10, "%s: %d %d %d %d\n", __FUNCTION__, rect_.x, rect_.y, rect_.w, rect_.h));
    FXDC* dc = (FXDC*)ptr;
    dc->setForeground(FXRGB(0, 255, 0));
    dc->setClipRectangle (rect_.x, rect_.y, rect_.w, rect_.h);
//    dc->drawRectangle (rect_.x, rect_.y, rect_.w - 1, rect_.h - 1);
    FXint tw = font_->getTextWidth(text_);
    FXint th = font_->getTextHeight(text_);
    dc->drawText(rect_.x + (rect_.w - tw) / 2, rect_.y + (rect_.h + th) / 2, text_);
    return 0;
}

void AstroLabel::position(FXint x, FXint y, FXint w, FXint h)
{
    if (w != -1)
        rect_.w = w;
    if (h != -1)
        rect_.h = h;
    rect_.x = x - rect_.w / 2;
    rect_.y = y - rect_.h / 2;
}

