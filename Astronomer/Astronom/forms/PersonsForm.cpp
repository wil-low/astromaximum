#include "PersonsForm.h"
#include "../Astronom.h"
#include "../widgets/MaskedTextField.h"

FXDEFMAP(PersonsForm) PersonsFormMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           FXDialogBox::ID_ACCEPT,   PersonsForm::onCmdAccept),
};

FXIMPLEMENT(PersonsForm, FXDialogBox, PersonsFormMessageMap, ARRAYNUMBER(PersonsFormMessageMap))

PersonsForm::PersonsForm(FXWindow* wo)
: FXDialogBox(wo,"PersonsForm", DECOR_TITLE|DECOR_CLOSE|DECOR_BORDER|DECOR_SHRINKABLE|DECOR_STRETCHABLE,
			  100, 100, 600, 400, 0, 0, 0, 0 ,0, 0)
{
    new FXTabBook(this, NULL, 0, TABBOOK_NORMAL|LAYOUT_FILL_X|LAYOUT_FILL_Y);
//	FXVerticalFrame* vframe=new FXVerticalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

}

PersonsForm::~PersonsForm(void)
{
}

void PersonsForm::create()
{
	FXDialogBox::create();
/*
	for (int i = 0; i < 20; ++i) {
        lAtlasCountry_->appendItem("country");
        lAtlasState_->appendItem("state");
        lAtlasCity_->appendItem("city");
	}*/
}

long PersonsForm::onCmdAccept(FXObject* o, FXSelector, void*)
{
	((FXWindow*)o)->hide();
	return 1;
}
