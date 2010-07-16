#include "Localizer.h"

FXIMPLEMENT(Localizer, FXTranslator, 0, 0)

Localizer::Localizer()
{
}

void Localizer::load_lang(const FXString& lang_file)
{
    settings_.clear();
    settings_.parseFile(lang_file, false);
}

const FXchar* Localizer::tr(const FXchar* context,const FXchar* message,const FXchar* hint,FXint count) const
{
    FXStringDict* dict = settings_.find(context);
    if (dict) {
        const FXchar* translated = dict->find(message);
        if (translated)
            return translated;
    }
    return message;
}
