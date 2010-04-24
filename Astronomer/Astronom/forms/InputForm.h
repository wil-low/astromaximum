#pragma once
#include <fx.h>
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
		ID_LAST
	};
	void create();

	long onCmdSearch(FXObject*, FXSelector, void*);
	struct input_data_t {
		FXString name;
	} input_data_;
protected:
	InputForm(){}

};
