#pragma once
#include <fx.h>
#include <map>

class Ephemeris;
class MainForm;
class InputForm;
class PersonsForm;
class Chrono;
class GlyphManager;
class OcularModel;
class DraggableView;

class Astronom : public FXApp {
	// Macro for class hierarchy declarations
	FXDECLARE(Astronom)
public:
	Astronom(const FXString& name, const FXString& vendor);
	virtual ~Astronom();
	virtual void create();

	void setOcular(DraggableView* dv);
	FXFont* getAstroFont (int size);

	enum{
		ID_GLYPH=FXApp::ID_LAST,
		ID_CHRONO,
		ID_INC_HOUR,
		ID_INPUTDATA,
		ID_PERSONS,
		ID_LAST
	};

	GlyphManager* fGlyphManager;
	FXImage* offscreen;

	long onCmdInputData(FXObject*, FXSelector, void*);
	long onCmdPersons(FXObject*, FXSelector, void*);
	long onCmdGlyph(FXObject*, FXSelector, void*);
	long onCmdToggleChrono(FXObject*, FXSelector, void*);
	long onCmdIncHour(FXObject*, FXSelector, void*);
	long onCmdClose(FXObject*, FXSelector, void*);
	long onQueryTip(FXObject*, FXSelector, void*);
private:
	void clearFonts();
	void loadFont(const FXString& face);

	Ephemeris* ephemeris;
	MainForm* fMain;
	InputForm* fInputData;
	PersonsForm* fPersons;
	Chrono* fChrono;
	OcularModel* mOcular;
	FXToolTip* tooltip_;
	std::map<int, FXFont*> astrofont_map_;
protected:
	Astronom(){}
};
