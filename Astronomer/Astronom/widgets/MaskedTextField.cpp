#include "MaskedTextField.h"

FXDEFMAP(MaskedTextField) MaskedTextFieldMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_VERIFY,             0, MaskedTextField::onVerify),
};

FXIMPLEMENT(MaskedTextField, FXTextField, MaskedTextFieldMessageMap, ARRAYNUMBER(MaskedTextFieldMessageMap))

MaskedTextField::MaskedTextField(FXComposite* p,FXint ncols,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb)
: FXTextField(p, ncols, tgt, sel, opts, x, y, w, h, pl, pr, pt, pb)
{
}

MaskedTextField::~MaskedTextField(void)
{
}

void MaskedTextField::setMask(const FXString& mask)
{
	mask_ = mask;
	if (!mask_.empty())
		rex_.parse(mask_);
}

long MaskedTextField::onVerify(FXObject*, FXSelector, void* ptr)
{
	const FXchar* str = (const FXchar*)ptr;
	FXTRACE((10, "%s: %s\n", __FUNCTION__, str));
	bool matched = true;
	if (!mask_.empty())
		matched = rex_.match(str, strlen(str));
	return matched ? 0 : 1;
}