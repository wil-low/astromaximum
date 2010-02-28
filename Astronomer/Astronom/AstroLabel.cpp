#include "AstroLabel.h"
#include "GlyphManager.h"
#include "DraggableView.h"

FXDEFMAP(AstroLabel) AstroLabelMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             0, AstroLabel::onDrawOnParent),
	FXMAPFUNC(SEL_PAINT,             AstroLabel::ID_FOCUS, AstroLabel::onDrawFocus),
	FXMAPFUNC(SEL_CLICKED,           0, AstroLabel::onClicked),
	FXMAPFUNC(SEL_COMMAND,          AstroLabel::ID_SELECT, AstroLabel::onCmdSelect),
};

FXIMPLEMENT(AstroLabel, FXObject, AstroLabelMessageMap, ARRAYNUMBER(AstroLabelMessageMap))

AstroLabel::AstroLabel(DraggableView* p, FXint x, FXint y, FXint w, FXint h)
: rect_(x, y, w, h)
, font_(NULL)
, selected_(false)
{
}

AstroLabel::~AstroLabel(void)
{
}

void AstroLabel::setText(const FXString& text, FXFont* font)
{
	text_ = text;
	font_ = font;
}

long AstroLabel::onClicked(FXObject*, FXSelector, void*)
{
	return 1;
}

long AstroLabel::onDrawOnParent(FXObject* o, FXSelector sel, void* ptr)
{
    FXDC* dc = (FXDC*)ptr;
    dc->setFont(font_);
    dc->setClipRectangle (rect_.x, rect_.y, rect_.w, rect_.h);
    dc->setForeground(FXRGB(0, 0, 0));
    FXint tw = font_->getTextWidth(text_);
    FXint th = font_->getTextHeight(text_);
    dc->drawText(rect_.x + (rect_.w - tw) / 2, rect_.y + (rect_.h + th) / 2, text_);
    if (selected_)
        onDrawFocus(o, sel, ptr);
    return 1;
}

long AstroLabel::onDrawFocus(FXObject*, FXSelector, void* ptr)
{
    FXDC* dc = (FXDC*)ptr;
    dc->setClipRectangle (rect_.x, rect_.y, rect_.w, rect_.h);
    dc->setForeground(selected_ ? FXRGB(255, 0, 0) : dc->getBackground());
    dc->drawRectangle(rect_.x, rect_.y, rect_.w - 1, rect_.h - 1);
    return 1;
}

long AstroLabel::onCmdSelect(FXObject* o, FXSelector sel, void* ptr)
{
    selected_ = ptr != 0;
//    FXTRACE((10, selected_ ? "sel\n" : "unsel\n"));
    return 1;
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

double AstroLabel::getAngle()
{
    return -1;
}

int AstroLabel::getType()
{
    return TYPE_LAST;
}

bool AstroLabel::contains(FXint x, FXint y)
{
    return rect_.contains(x, y);
}

