#include "AstroLabel.h"
#include "../forms/GlyphManager.h"
#include "../views/DraggableView.h"

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
, chart_id_(-1)
, id_(-1)
, flags_(0)
, parent_(p)
{
}

AstroLabel::~AstroLabel(void)
{
}

void AstroLabel::setChartId(int id)
{
	chart_id_ = id;
}

int AstroLabel::getChartId() const
{
	return chart_id_;
}

int AstroLabel::getId() const
{
	return id_;
}

int AstroLabel::getFlags() const
{
	return flags_;
}

FXWindow* AstroLabel::getParent() const
{
	return parent_;
}

void AstroLabel::setFlags(int flags)
{
	flags_ = flags;
}

void AstroLabel::setId(int id, const FXString& text)
{
	id_ = id;
	text_ = text;
}

void AstroLabel::setFont(FXFont* font)
{
	font_ = font;
	rect_.h = rect_.w = font->getFontHeight() + 3;
}

FXFont* AstroLabel::getFont() const
{
	return font_;
}

const FXString& AstroLabel::getText() const
{
	return text_;
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

double AstroLabel::getAngle() const
{
    return -1;
}

double AstroLabel::getVisibleAngle() const
{
    return -1;
}

AstroLabel::label_type_t AstroLabel::getType() const
{
    return TYPE_LAST;
}

bool AstroLabel::contains(FXint x, FXint y)
{
    return rect_.contains(x, y);
}

const FXRectangle& AstroLabel::getRect() const
{
	return rect_;
}

FXString AstroLabel::toString() const
{
    FXString s;
    s.format ("AstroLabel type %d, id %d, angle %.2f", getType(), getId(), getAngle());
	return s;
}

unsigned int AstroLabel::getIdentity() const
{
	return (getChartId() << 24) + (getType() << 16) + (getId());
}

void AstroLabel::setProps(const BodyProps& props)
{
	props_ = props;
}

double AstroLabel::getProp(BodyProps::body_property p) const
{
	return props_.prop[p];
}
