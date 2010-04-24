#include "InputForm.h"
#include "../Astronom.h"
#include "../widgets/MaskedTextField.h"

FXDEFMAP(InputForm) InputFormMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_SEARCH,   InputForm::onCmdSearch),
};

FXIMPLEMENT(InputForm, FXDialogBox, InputFormMessageMap, ARRAYNUMBER(InputFormMessageMap))

InputForm::InputForm(FXWindow* wo)
: FXDialogBox(wo,"InputForm", DECOR_TITLE|DECOR_CLOSE|DECOR_BORDER|DECOR_SHRINKABLE|DECOR_STRETCHABLE,
			  100, 100, 400, 400, 0, 0, 0, 0 ,0, 0)
{
	FXVerticalFrame* vframe=new FXVerticalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

	FXMatrix* matrix=new FXMatrix(vframe,4,MATRIX_BY_ROWS|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
		new FXButton(matrix, tr("NewChart"),NULL,NULL);
		new FXButton(matrix, tr("Now"),NULL,NULL);
		new FXButton(matrix, tr("Here"),NULL,NULL);
		new FXButton(matrix, tr("Atlas"),NULL);

		new FXTextField(matrix, 30, NULL, ID_NAME);
		{
		FXHorizontalFrame* hframe=new FXHorizontalFrame(matrix,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
			MaskedTextField* mtfDate = new MaskedTextField(hframe, 10, NULL, ID_NAME, TEXTFIELD_NORMAL|TEXTFIELD_OVERSTRIKE);
			mtfDate->setText("10.10.2000");
			mtfDate->setMask("^\\d\\d\\.\\d\\d\\.\\d{4}$");
			FXComboBox* cbEra = new FXComboBox(hframe, 1, NULL, ID_ERA, TEXTFIELD_NORMAL|COMBOBOX_STATIC);
			cbEra->fillItems("AC\nBC");
			MaskedTextField* mtfTime = new MaskedTextField(hframe, 8, NULL, ID_TIME, TEXTFIELD_NORMAL|TEXTFIELD_OVERSTRIKE);
			mtfTime->setText("10:10:20");
			mtfTime->setMask("^\\d\\d\\:\\d\\d\\:\\d{4}$");
		}
		FXComboBox* cbLoc = new FXComboBox(matrix, 1, NULL, ID_LOCATION, TEXTFIELD_NORMAL|LAYOUT_FILL_X);
		{
		FXHorizontalFrame* hframe=new FXHorizontalFrame(matrix,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
			MaskedTextField* mtfLon = new MaskedTextField(hframe, 8, NULL, ID_LON, TEXTFIELD_NORMAL|TEXTFIELD_OVERSTRIKE);
			mtfLon->setText("030°31'E");
			mtfLon->setMask("^\\d{3}°\\d{2}'[EW]$");
			MaskedTextField* mtfLat = new MaskedTextField(hframe, 7, NULL, ID_LAT, TEXTFIELD_NORMAL|TEXTFIELD_OVERSTRIKE);
			mtfLat->setText("50°25'N");
			mtfLat->setMask("^\\d{2}°\\d{2}'[NS]$");
			MaskedTextField* mtfTzDiff = new MaskedTextField(hframe, 6, NULL, ID_TZDIFF, TEXTFIELD_NORMAL|TEXTFIELD_OVERSTRIKE|LAYOUT_FILL_X);
			mtfTzDiff->setText("+03:00");
			mtfTzDiff->setMask("^[\\+\\-]\\d{2}:\\d{2}$");
		}

		{
		FXHorizontalFrame* hframe=new FXHorizontalFrame(matrix,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
			new FXButton(hframe, tr("Enter"), NULL, this, ID_ACCEPT);
			new FXButton(hframe, tr("Cancel"), NULL, this, ID_CANCEL);
			new FXButton(hframe, tr("DefaultLoc"), NULL,NULL,ID_DEFAULT_LOC);
		}
		FXComboBox* cbUnknown = new FXComboBox(matrix, 1, NULL, 0, TEXTFIELD_NORMAL|COMBOBOX_STATIC);
		new FXButton(matrix, tr("Search point"), NULL,NULL,ID_SEARCH_POINT);
		{
		FXHorizontalFrame* hframe=new FXHorizontalFrame(matrix,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
			new FXTextField(hframe, 10, NULL, ID_SEARCH_STR);
			new FXButton(hframe, tr("Search"), NULL, this, ID_SEARCH);
		}
}

InputForm::~InputForm(void)
{
}

void InputForm::create()
{
	FXDialogBox::create();
}

long InputForm::onCmdSearch(FXObject* o, FXSelector, void*)
{
	((FXWindow*)o)->hide();
	return 1;
}
