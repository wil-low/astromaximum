#pragma once
#include <fx.h>
#include <FXTranslator.h>

class Localizer : public FX::FXTranslator
{
    FXDECLARE(Localizer)
public:
    Localizer();
    virtual ~Localizer() {};
    void load_lang(const FXString& lang_file);
	virtual const FXchar* tr(const FXchar* context,const FXchar* message,const FXchar* hint=NULL,FXint count=-1) const;// FX_FORMAT(2);
private:
    FXSettings settings_;
};
