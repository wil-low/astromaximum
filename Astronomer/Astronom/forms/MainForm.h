#pragma once
#include <fx.h>

class MainForm : public FXMainWindow {
	// Macro for class hierarchy declarations
	FXDECLARE(MainForm)
public:

	// Messages for our class
	enum{
		ID_CANVAS=FXMainWindow::ID_LAST,
		ID_CLEAR,
		ID_ADD,
		ID_LOCK,
		ID_LAST
	};

	// MainForm's constructor
	MainForm(FXApp* a);
	virtual void create();
	void init();
	virtual ~MainForm();


	long onAddView(FXObject*, FXSelector, void*);
	long onCmdGlyph(FXObject*, FXSelector, void*);
	long onCmdLock(FXObject*, FXSelector, void*);

	FXFont *fntAstro;

	FXVerticalFrame   *canvasFrame;             // Canvas frame
private:
	FXHorizontalFrame *contents;                // Content frame
	FXVerticalFrame   *buttonFrame;             // Button frame
	FXMenuPane* filemenu;
	FXMenuPane* housemenu;
	FXCheckButton* btnLock;
protected:
	MainForm(){}
};
