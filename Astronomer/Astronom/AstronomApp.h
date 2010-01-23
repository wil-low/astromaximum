#pragma once
#include <fx.h>

class Astronom : public FXMainWindow {
	// Macro for class hierarchy declarations
	FXDECLARE(Astronom)
public:

	// Messages for our class
	enum{
		ID_CANVAS=FXMainWindow::ID_LAST,
		ID_CLEAR,
		ID_ADD,
		ID_LAST
	};

	// Astronom's constructor
	Astronom(FXApp* a);
	virtual void create();
	virtual ~Astronom();
	long onAddView(FXObject*, FXSelector, void*);

	FXFont *fntAstro;

private:
	FXHorizontalFrame *contents;                // Content frame
	FXVerticalFrame   *canvasFrame;             // Canvas frame
	FXVerticalFrame   *buttonFrame;             // Button frame
protected:
	Astronom(){}
};
