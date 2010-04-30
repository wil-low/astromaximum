#pragma once
#include <fx.h>
#include <FXTranslator.h>

class Localizer : public FX::FXTranslator
{
    FXDECLARE(Localizer)
public:
    Localizer(FXApp* a);
    virtual ~Localizer() {};
    void load_lang(const FXString& lang);
    virtual const FXchar* tr(const FXchar* context,const FXchar* message,const FXchar* hint=NULL) const;
private:
    Localizer(){}
    FXSettings settings_;
};
