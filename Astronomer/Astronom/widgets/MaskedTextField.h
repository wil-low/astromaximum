#pragma once
#include <fx.h>

class MaskedTextField : public FXTextField
{
	FXDECLARE(MaskedTextField)
public:
	MaskedTextField(FXComposite* p,FXint ncols,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=TEXTFIELD_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);
	virtual ~MaskedTextField(void);
	
	long onVerify(FXObject*, FXSelector, void*);

	void setMask(const FXString& mask);
protected:
	MaskedTextField(){}
private:
	FXString mask_;
	FXRex rex_;
};
