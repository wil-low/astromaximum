#include "InputForm.h"
#include "../Astronom.h"
#include "../Ephemeris.h"
#include "../utils/constants.h"
//#include "../widgets/MaskedTextField.h"
#include "FX88591Codec.h"
#include "FXCP1252Codec.h"
#include "FXUTF16Codec.h"

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
	FXMAPFUNC(SEL_CLIPBOARD_LOST,    0,					     InputForm::onClipboardLost),
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
			mtfLon_ = new FXTextField(hframe, 12, NULL, ID_LON, TEXTFIELD_NORMAL);
//			mtfLon_->setMask("^\\d{,3} \\d{,2}'[EW]$");
			mtfLat_ = new FXTextField(hframe, 12, NULL, ID_LAT, TEXTFIELD_NORMAL);
//			mtfLat_->setMask("^\\d{,2} \\d{,2}'[NS]$");
			mtfTzDiff_ = new FXTextField(hframe, 12, NULL, ID_TZDIFF, TEXTFIELD_NORMAL|LAYOUT_FILL_X);
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

	textType_ = getApp()->registerDragType(textTypeName);
	utf8Type_ = getApp()->registerDragType(utf8TypeName);
	utf16Type_ = getApp()->registerDragType(utf16TypeName);
}

void InputForm::init()
{
	timeloc_.setName(UNNAMED_DOC);
	timeloc_.set(TL_DATE, Ephemeris::now ());
	timeloc_.set(TL_LAT, 45);
	timeloc_.set(TL_LON, 34);
	timeloc_.set(TL_ELV, 0);
	restoreData(timeloc_);

	for (int i = 0; i < 20; ++i) {
        lAtlasCountry_->appendItem("country");
        lAtlasState_->appendItem("state");
        lAtlasCity_->appendItem("city");
	}
}

long InputForm::onCmdShow(FXObject* o, FXSelector sel, void* ptr)
{
	saveData(true);
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
	FXString title;
	tl.asTitle(title);
	title += " - " + getApp()->getAppName();
	getOwner()->handle (this, FXSEL(SEL_COMMAND, ID_SETSTRINGVALUE), &title);
	return FXDialogBox::onCmdAccept(o, sel, ptr);
}

long InputForm::onCmdCancel(FXObject* o, FXSelector sel, void* ptr)
{
	restoreData(timeloc_);
	return FXDialogBox::onCmdCancel(o, sel, ptr);
}

long InputForm::onCmdNow(FXObject* o, FXSelector sel, void* ptr)
{
	timeloc_.set(TL_DATE, Ephemeris::now());
	restoreData(timeloc_);
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

void InputForm::restoreData (TimeLoc& tl)
{
    tfName_->setText(tl.getName());
	mtfDate_->setText(tl.getStr(TL_DATE));
    mtfTime_->setText(tl.getStr(TL_TIME));
//    cbLoc_->setText(str_data[3]);
    mtfLat_->setText(tl.getStr(TL_LAT));
    mtfLon_->setText(tl.getStr(TL_LON));
    mtfTzDiff_->setText(tl.getStr(TL_TZ));
}

long InputForm::onCmdCopy(FXObject* o, FXSelector sel, void* ptr)
{
    FXDragType types[4];
	types[0]=stringType;
	types[1]=textType;
	types[2]=utf8Type;
	types[3]=utf16Type;
    acquireClipboard(types,4);
	return 1;
}

long InputForm::onCmdPaste(FXObject* o, FXSelector sel, void* ptr)
{
	FXString string;
	do {
		// First, try UTF-8
		if(getDNDData(FROM_CLIPBOARD,utf8Type,string)){
			FXTRACE((100,"Paste UTF8\n"));
#ifdef WIN32
			dosToUnix(string);
#endif
			break;
		}

		// Next, try UTF-16
		if(getDNDData(FROM_CLIPBOARD,utf16Type,string)){
			FXUTF16LECodec unicode;           // FIXME maybe other endianness for unix
			FXTRACE((100,"Paste UTF16\n"));
			string=unicode.mb2utf(string);
#ifdef WIN32
			dosToUnix(string);
#endif
			break;
		}

		// Next, try good old Latin-1
		if(getDNDData(FROM_CLIPBOARD,stringType,string)){
			FX88591Codec ascii;
			FXTRACE((100,"Paste ASCII\n"));
			string=ascii.mb2utf(string);
#ifdef WIN32
			dosToUnix(string);
#endif
			break;
		}
		getApp()->beep();
		return 0;
	} while (false);
	TimeLoc tl;
	tl.deserialize (string);
	restoreData(tl);
	return 1;
}

long InputForm::onClipboardLost(FXObject* o, FXSelector sel, void* ptr)
{
	FXTRACE((10, "%s for %s\n", __FUNCTION__, o->getClassName()));
	return 1;
}

long InputForm::onClipboardGained(FXObject* o, FXSelector sel, void* ptr)
{
	FXTRACE((10, "%s for %s\n", __FUNCTION__, o->getClassName()));
	timeloc_.serialize(clipboardText_);
	return 1;
}

long InputForm::onClipboardRequest(FXObject* o, FXSelector sel, void* ptr)
{
	// See if base class knows how to deal with the requested clipboard type
	if (FXDialogBox::onClipboardRequest (o, sel, ptr))
		return 1;

	FXDragType dtype = ((FXEvent*)ptr)->target;
	// Requested data from clipboard
	if(dtype == stringType || dtype == textType || dtype == utf8Type || dtype == utf16Type) {
		FXString string = clipboardText_;

		// Expand newlines to CRLF on Windows
#ifdef WIN32
		unixToDos(string);
#endif

		// Return clipped text as as UTF-8
		if(dtype==utf8Type){
			FXTRACE((100,"Request UTF8\n"));
			setDNDData(FROM_CLIPBOARD,dtype,string);
			return 1;
		}

		// Return clipped text translated to 8859-1
		if(dtype==stringType || dtype==textType){
			FX88591Codec ascii;
			FXTRACE((100,"Request ASCII\n"));
			setDNDData(FROM_CLIPBOARD,dtype,ascii.utf2mb(string));
			return 1;
		}

		// Return text of the selection translated to UTF-16
		if(dtype==utf16Type){
			FXUTF16LECodec unicode;             // FIXME maybe other endianness for unix
			FXTRACE((100,"Request UTF16\n"));
			setDNDData(FROM_CLIPBOARD,dtype,unicode.utf2mb(string));
			return 1;
		}
	}
	else {
		FXString name = getApp()->getDragTypeName(dtype);
		FXTRACE((10, "%s: unknown DragTypeName '%s'\n", __FUNCTION__, name.text()));
	}
	return 0;
}
