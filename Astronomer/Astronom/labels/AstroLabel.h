#pragma once
#include "../utils/BodyProps.h"
#include <fx.h>

class DraggableView;

class AstroLabel : public FXObject
{
	FXDECLARE(AstroLabel)
public:
    enum label_type_t {
        TYPE_ZODIAC = 0,
        TYPE_PLANET,
        TYPE_HOUSE,
        TYPE_ASPECT,
        TYPE_LAST,
    };

	AstroLabel(DraggableView* p, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
	virtual ~AstroLabel(void);

	virtual long onClicked(FXObject*, FXSelector, void*);
	long onDrawOnParent(FXObject*, FXSelector, void*);
    long onDrawFocus(FXObject*, FXSelector, void* ptr);
	long onCmdSelect(FXObject*, FXSelector, void*);

	enum{
		ID_FRAME = 0,
		ID_SELECT,
		ID_FOCUS,
		ID_LAST,
	};
	virtual double getAngle() const;
	virtual double getVisibleAngle() const;
	virtual label_type_t getType() const;
	const FXRectangle& getRect() const;
    const FXString& getText() const;
	int getChartId() const;
	int getId() const;
	int getFlags() const;
	unsigned int getIdentity() const;
	FXFont* getFont() const;
	double getProp(BodyProps::body_property p) const;
	FXWindow* getParent() const;

	void setId(int id, const FXString& text);
	void setChartId(int id);
	void setFont(FXFont* font);
	void setFlags(int flags);

	virtual void setProps(const BodyProps& props);

	virtual void setVisibleAngle(double /*ang*/) {};

	virtual FXString toString() const;

    bool contains(FXint x, FXint y);
	virtual void position(FXint x, FXint y, FXint w = -1, FXint h = -1);

	AstroLabel(){}
protected:
    FXRectangle rect_;
    FXFont* font_;
    FXString text_;
    bool selected_;
	int chart_id_;
	int id_;
	int flags_;
	BodyProps props_;
	FXWindow* parent_;
};
