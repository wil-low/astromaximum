#include "InputForm.h"
#include "../Astronom.h"
#include "../Ephemeris.h"
//#include "../widgets/MaskedTextField.h"

FXDEFMAP(InputForm) InputFormMessageMap[]={

	//________Message_Type_____________________ID____________Message_Handler_______
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_SHOW,     InputForm::onCmdShow),
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_SEARCH,   InputForm::onCmdSearch),
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_ACCEPT,   InputForm::onCmdAccept),
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_CANCEL,   InputForm::onCmdCancel),
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_CANCEL,   InputForm::onCmdCancel),
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_NOW,      InputForm::onCmdNow),

	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_COPY,     InputForm::onCmdCopy),
	FXMAPFUNC(SEL_COMMAND,           InputForm::ID_PASTE,    InputForm::onCmdPaste),
	FXMAPFUNC(SEL_CLIPBOARD_GAINED,  0,					     InputForm::onClipboardGained),
	FXMAPFUNC(SEL_CLIPBOARD_REQUEST, 0,					     InputForm::onClipboardRequest),
};

FXIMPLEMENT(InputForm, FXDialogBox, InputFormMessageMap, ARRAYNUMBER(InputFormMessageMap))

InputForm::InputForm(FXWindow* wo)
: FXDialogBox(wo,"InputForm", DECOR_TITLE|DECOR_CLOSE|DECOR_BORDER|DECOR_SHRINKABLE|DECOR_STRETCHABLE,
			  100, 100, 600, 400, 0, 0, 0, 0 ,0, 0)
{
	FXVerticalFrame* vframe=new FXVerticalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);

	FXMatrix* matrix=new FXMatrix(vframe,4,MATRIX_BY_ROWS|LAYOUT_SIDE_TOP|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0);
		new FXButton(matrix, tr("NewChart"), NULL, NULL);
		new FXButton(matrix, tr("Now"), NULL, this, ID_NOW);
		new FXButton(matrix, tr("Here"), NULL, NULL);
		new FXButton(matrix, tr("Atlas"), NULL);

		tfName_ = new FXTextField(matrix, 30, NULL, ID_NAME);
		{
		FXHorizontalFrame* hframe=new FXHorizontalFrame(matrix,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
			mtfDate_ = new FXTextField(hframe, 10, NULL, ID_NAME, TEXTFIELD_NORMAL);
//			mtfDate_->setMask("^\\d{,4}/\\d{,2}/\\d{,2}$");
			FXComboBox* cbEra = new FXComboBox(hframe, 1, NULL, ID_ERA, TEXTFIELD_NORMAL|COMBOBOX_STATIC);
			cbEra->fillItems("AC\nBC");
			cbEra->disable();
            mtfTime_ = new FXTextField(hframe, 8, NULL, ID_TIME, TEXTFIELD_NORMAL);
//			mtfTime_->setMask("^\\d{,2}\\:\\d{,2}\\:\\d{,4}$");
		}
		cbLoc_ = new FXComboBox(matrix, 1, NULL, ID_LOCATION, TEXTFIELD_NORMAL|LAYOUT_FILL_X);
		{
		FXHorizontalFrame* hframe=new FXHorizontalFrame(matrix,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
			mtfLon_ = new FXTextField(hframe, 8, NULL, ID_LON, TEXTFIELD_NORMAL);
//			mtfLon_->setMask("^\\d{,3} \\d{,2}'[EW]$");
			mtfLat_ = new FXTextField(hframe, 7, NULL, ID_LAT, TEXTFIELD_NORMAL);
//			mtfLat_->setMask("^\\d{,2} \\d{,2}'[NS]$");
			mtfTzDiff_ = new FXTextField(hframe, 6, NULL, ID_TZDIFF, TEXTFIELD_NORMAL|LAYOUT_FILL_X);
//			mtfTzDiff_->setMask("^[\\+\\-]?\\d{,2}:\\d{,2}$");
		}

		{
		FXHorizontalFrame* hframe=new FXHorizontalFrame(matrix,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
			new FXButton(hframe, tr("Enter"), NULL, this, ID_ACCEPT);
			new FXButton(hframe, tr("Cancel"), NULL, this, ID_CANCEL);
			new FXButton(hframe, tr("DefaultLoc"), NULL,NULL,ID_DEFAULT_LOC);
		}
		{
		FXHorizontalFrame* hframe=new FXHorizontalFrame(matrix,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
			FXComboBox* cbUnknown = new FXComboBox(hframe, 1, NULL, 0, TEXTFIELD_NORMAL|COMBOBOX_STATIC);
			cbUnknown->disable();
			new FXButton(hframe, tr("Copy\tCopy to clipboard"), NULL, this, ID_COPY);
			new FXButton(hframe, tr("Paste\tPaste from clipboard"), NULL, this, ID_PASTE);
		}
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
	clipDragType_ = getApp()->registerDragType("UTF8_STRING");
}

void InputForm::init()
{
	timeloc_.setName("Noname");
	timeloc_.set(TL_DATE, Ephemeris::now ());
	timeloc_.set(TL_LAT, 45);
	timeloc_.set(TL_LON, 34);
	timeloc_.set(TL_ELV, 0);
	restoreData();

	for (int i = 0; i < 20; ++i) {
        lAtlasCountry_->appendItem("country");
        lAtlasState_->appendItem("state");
        lAtlasCity_->appendItem("city");
	}
}

long InputForm::onCmdShow(FXObject* o, FXSelector sel, void* ptr)
{
	saveData(false);
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
	saveData(true);
	TimeLoc tl(timeloc_);
	FXTRACE((10, "%.2f %.2f\n", tl.get(TL_DATE), tl.get(TL_TIME)));
	getApp()->handle (this, FXSEL(SEL_COMMAND, ID_INPUT_ACCEPT), &tl);
	return FXDialogBox::onCmdAccept(o, sel, ptr);
}

long InputForm::onCmdCancel(FXObject* o, FXSelector sel, void* ptr)
{
	restoreData();
	return FXDialogBox::onCmdCancel(o, sel, ptr);
}

long InputForm::onCmdNow(FXObject* o, FXSelector sel, void* ptr)
{
	timeloc_.set(TL_DATE, Ephemeris::now());
	restoreData();
	return 1;
}

void InputForm::saveData (bool recalculate)
{
	timeloc_.setName (tfName_->getText());
	timeloc_.set (TL_DATE, mtfDate_->getText() + " " + mtfTime_->getText(), recalculate);
//    timeloc_.set (TL_LOC, cbLoc_->getText(), recalculate);
    timeloc_.set (TL_LON, mtfLon_->getText(), recalculate);
    timeloc_.set (TL_LAT, mtfLat_->getText(), recalculate);
    timeloc_.set (TL_TZ, mtfTzDiff_->getText(), recalculate);
}

void InputForm::restoreData ()
{
    tfName_->setText(timeloc_.getName());
	mtfDate_->setText(timeloc_.getStr(TL_DATE));
    mtfTime_->setText(timeloc_.getStr(TL_TIME));
//    cbLoc_->setText(str_data[3]);
    mtfLat_->setText(timeloc_.getStr(TL_LAT));
    mtfLon_->setText(timeloc_.getStr(TL_LON));
    mtfTzDiff_->setText(timeloc_.getStr(TL_TZ));
}

long InputForm::onCmdCopy(FXObject* o, FXSelector sel, void* ptr)
{
	acquireClipboard (&clipDragType_, 1);
	return 1;
}

long InputForm::onCmdPaste(FXObject* o, FXSelector sel, void* ptr)
{
    FXuchar* data;
    FXuint size;
    if (getDNDData (FROM_CLIPBOARD, clipDragType_, data, size)) {
        clipboardText_.assign((FXchar*)data, (FXint)size);
        FXFREE (&data);
        timeloc_.deserialize (clipboardText_);
        restoreData();
        return 1;
    }
	return 0;
}

long InputForm::onClipboardGained(FXObject* o, FXSelector sel, void* ptr)
{
	timeloc_.serialize(clipboardText_);
	return 1;
}

long InputForm::onClipboardRequest(FXObject* o, FXSelector sel, void* ptr)
{
	// See if base class knows how to deal with the requested clipboard type
	if (FXDialogBox::onClipboardRequest (o, sel, ptr))
		return 1;
	FXDragType dtype = ((FXEvent*)ptr)->target;
	FXString name = getApp()->getDragTypeName(dtype);
	// See if we can deal with this type ourselves
	if (dtype == clipDragType_){
		FXuchar *data;
		FXuint len = clipboardText_.length();
		FXMALLOC(&data, FXuchar, len);
		strncpy((char*)data, clipboardText_.text(), len);
		// Give the array to the system!
		setDNDData (FROM_CLIPBOARD, dtype, data, len);

		// Return 1 because it was handled here
		return 1;
	}
	FXTRACE((10, "%s: unknown DragTypeName '%s'\n", __FUNCTION__, name.text()));
	// Return 0 to signify we haven't dealt with it yet; a derived
	// class from InputForm may yet give it another try ...
	return 0;
}
