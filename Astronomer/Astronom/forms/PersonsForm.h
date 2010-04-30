#pragma once
#include <fx.h>
class PersonsForm : public FXDialogBox
{
	FXDECLARE(PersonsForm)
public:
	PersonsForm(FXWindow* wo);
	~PersonsForm();

	// Messages for our class
	enum{
		ID_NAME=FXDialogBox::ID_LAST,
		ID_LAST
	};
	void create();

    long onCmdAccept(FXObject*, FXSelector, void*);
protected:
	PersonsForm(){}
private:
//    FXList *lAtlasCountry_, *lAtlasState_, *lAtlasCity_;
};
