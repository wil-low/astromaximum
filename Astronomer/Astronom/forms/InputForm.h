#pragma once
#include "../TimeLoc.h"
#include <fx.h>
class MaskedTextField;


class InputForm : public FXDialogBox
{
	FXDECLARE(InputForm)
public:
	InputForm(FXWindow* wo);
	~InputForm();

	// Messages for our class
	enum {
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
		ID_INPUT_ACCEPT,
		ID_NOW,
		ID_COPY,
		ID_PASTE,
		ID_LAST
	};

	void create();
	void init();

	long onCmdSearch(FXObject*, FXSelector, void*);
	long onCmdAccept(FXObject*, FXSelector, void*);
	long onCmdCancel(FXObject*, FXSelector, void*);
	long onCmdShow(FXObject* o, FXSelector sel, void* ptr);
	long onCmdNow(FXObject* o, FXSelector sel, void* ptr);

	long onCmdCopy(FXObject* o, FXSelector sel, void* ptr);
	long onCmdPaste(FXObject* o, FXSelector sel, void* ptr);
	long onClipboardLost(FXObject* o, FXSelector sel, void* ptr);
	long onClipboardGained(FXObject* o, FXSelector sel, void* ptr);
	long onClipboardRequest(FXObject* o, FXSelector sel, void* ptr);

protected:
	InputForm(){}
private:
    FXList *lAtlasCountry_, *lAtlasState_, *lAtlasCity_;
    FXTextField *tfName_;
    FXComboBox* cbLoc_;
    FXTextField *mtfDate_, *mtfTime_, *mtfLon_, *mtfLat_, *mtfTzDiff_;
	void saveData(bool recalculate);
	void restoreData(TimeLoc& tl);
	void makeTimeLoc ();
	TimeLoc timeloc_;
	FXString clipboardText_;
	FXDragType textType_;           // Ascii text request
	FXDragType utf8Type_;           // UTF-8 text request
	FXDragType utf16Type_;          // UTF-16 text request
};
