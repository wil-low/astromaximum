#include "InputForm.h"
#include "../Astronom.h"
#include "../widgets/MaskedTextField.h"

FXDEFMAP(InputForm) InputFormMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_SHOW,     InputForm::onCmdShow),
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_SEARCH,   InputForm::onCmdSearch),
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_ACCEPT,   InputForm::onCmdAccept),
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_CANCEL,   InputForm::onCmdCancel),
};

FXIMPLEMENT(InputForm, FXDialogBox, InputFormMessageMap, ARRAYNUMBER(InputFormMessageMap))

InputForm::InputForm(FXWindow* wo)
: FXDialogBox(wo,"InputForm", DECOR_TITLE|DECOR_CLOSE|DECOR_BORDER|DECOR_SHRINKABLE|DECOR_STRETCHABLE,
			  100, 100, 600, 400, 0, 0, 0, 0 ,0, 0)
{
	FXVerticalFrame* vframe=new FXVerticalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

	FXMatrix* matrix=new FXMatrix(vframe,4,MATRIX_BY_ROWS|LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0);
		new FXButton(matrix, tr("NewChart"),NULL,NULL);
		new FXButton(matrix, tr("Now"),NULL,NULL);
		new FXButton(matrix, tr("Here"),NULL,NULL);
		new FXButton(matrix, tr("Atlas"),NULL);

		tfName_ = new FXTextField(matrix, 30, NULL, ID_NAME);
		{
		FXHorizontalFrame* hframe=new FXHorizontalFrame(matrix,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
			mtfDate_ = new MaskedTextField(hframe, 10, NULL, ID_NAME, TEXTFIELD_NORMAL);
			mtfDate_->setText("10.10.2000");
			mtfDate_->setMask("^\\d{,2}\\.\\d{,2}.\\d{,4}$");
			FXComboBox* cbEra = new FXComboBox(hframe, 1, NULL, ID_ERA, TEXTFIELD_NORMAL|COMBOBOX_STATIC);
			cbEra->fillItems("AC\nBC");
            mtfTime_ = new MaskedTextField(hframe, 8, NULL, ID_TIME, TEXTFIELD_NORMAL);
			mtfTime_->setText("10:10:20");
			mtfTime_->setMask("^\\d{,2}\\:\\d{,2}\\:\\d{,4}$");
		}
		cbLoc_ = new FXComboBox(matrix, 1, NULL, ID_LOCATION, TEXTFIELD_NORMAL|LAYOUT_FILL_X);
		{
		FXHorizontalFrame* hframe=new FXHorizontalFrame(matrix,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
			mtfLon_ = new MaskedTextField(hframe, 8, NULL, ID_LON, TEXTFIELD_NORMAL);
			mtfLon_->setText("030 31'E");
			mtfLon_->setMask("^\\d{,3} \\d{,2}'[EW]$");
			mtfLat_ = new MaskedTextField(hframe, 7, NULL, ID_LAT, TEXTFIELD_NORMAL);
			mtfLat_->setText("50 25'N");
			mtfLat_->setMask("^\\d{,2} \\d{,2}'[NS]$");
			mtfTzDiff_ = new MaskedTextField(hframe, 6, NULL, ID_TZDIFF, TEXTFIELD_NORMAL|LAYOUT_FILL_X);
			mtfTzDiff_->setText("+03:00");
			mtfTzDiff_->setMask("^[\\+\\-]?\\d{,2}:\\d{,2}$");
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
    {
    FXHorizontalFrame* hframe=new FXHorizontalFrame(vframe,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
        lAtlasCountry_ = new FXList(hframe, NULL, ID_ATLAS_COUNTRY, LIST_BROWSESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
        lAtlasState_ = new FXList(hframe, NULL, ID_ATLAS_STATE, LIST_BROWSESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
        lAtlasCity_ = new FXList(hframe, NULL,ID_ATLAS_CITY, LIST_BROWSESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    }
}

InputForm::~InputForm(void)
{
}

void InputForm::create()
{
	FXDialogBox::create();
	for (int i = 0; i < 20; ++i) {
        lAtlasCountry_->appendItem("country");
        lAtlasState_->appendItem("state");
        lAtlasCity_->appendItem("city");
	}
}

long InputForm::onCmdShow(FXObject* o, FXSelector sel, void* ptr)
{
    str_data[0]=tfName_->getText();
    str_data[1]=mtfDate_->getText();
    str_data[2]=mtfTime_->getText();
    str_data[3]=cbLoc_->getText();
    str_data[4]=mtfLon_->getText();
    str_data[5]=mtfLat_->getText();
    str_data[6]=mtfTzDiff_->getText();
    FXDialogBox::onCmdShow(o, sel, ptr);
	return 1;
}

long InputForm::onCmdSearch(FXObject* o, FXSelector, void*)
{
	((FXWindow*)o)->hide();
	return 1;
}

long InputForm::onCmdAccept(FXObject* o, FXSelector sel, void* ptr)
{
	return FXDialogBox::onCmdAccept(o, sel, ptr);
}

long InputForm::onCmdCancel(FXObject* o, FXSelector sel, void* ptr)
{
    tfName_->setText(str_data[0]);
    mtfDate_->setText(str_data[1]);
    mtfTime_->setText(str_data[2]);
    cbLoc_->setText(str_data[3]);
    mtfLon_->setText(str_data[4]);
    mtfLat_->setText(str_data[5]);
    mtfTzDiff_->setText(str_data[6]);

	return FXDialogBox::onCmdCancel(o, sel, ptr);
}

double InputForm::extrLat (const FXString& txt)
{
	char c; int d, m;
 	if (txt.scan ("%2d%2d%c", &d, &m, &c) != 3)
        return 0;
    double res = d + m / 60.L;
    if (c == 'S') res = -res;
    return res;
}

double InputForm::extrLon (const FXString& txt)
{
    char c; int d, m;
    if (txt.scan ("%3d%2d%c", &d, &m, &c) != 3)
        return 0;
    double res = d + m / 60.L;
    if (c == 'W') res = -res;
    return res;
}
