#pragma once
#include <fx.h>
class MaskedTextField;

class InputForm : public FXDialogBox
{
	FXDECLARE(InputForm)
public:
	InputForm(FXWindow* wo);
	~InputForm();

	// Messages for our class
	enum{
		ID_NAME=FXDialogBox::ID_LAST,
		ID_DATE,
		ID_ERA,
		ID_TIME,
		ID_LOCATION,
		ID_LON,
		ID_LAT,
		ID_TZDIFF,
		ID_DEFAULT_LOC,
		ID_SEARCH_POINT,
		ID_SEARCH_STR,
		ID_SEARCH,
		ID_ATLAS_COUNTRY,
		ID_ATLAS_STATE,
		ID_ATLAS_CITY,
		ID_LAST
	};
	void create();

	long onCmdSearch(FXObject*, FXSelector, void*);
	long onCmdAccept(FXObject*, FXSelector, void*);
	long onCmdCancel(FXObject*, FXSelector, void*);
	long onCmdShow(FXObject* o, FXSelector sel, void* ptr);

protected:
	InputForm(){}
private:
	FXString str_data[7];
    double extrLat (const FXString& txt);
    double extrLon (const FXString& txt);
    FXList *lAtlasCountry_, *lAtlasState_, *lAtlasCity_;
    FXTextField *tfName_;
    FXComboBox* cbLoc_;
    MaskedTextField *mtfDate_, *mtfTime_, *mtfLon_, *mtfLat_, *mtfTzDiff_;
};
