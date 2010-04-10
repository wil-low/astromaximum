#pragma once
class DraggableView;
class Ephemeris;

class BaseModel
{
public:
	BaseModel(Ephemeris* ephemeris);
	virtual ~BaseModel();
	virtual void setView(DraggableView*) = 0;
protected:
	Ephemeris* ephemeris_;
	DraggableView* view_;
};
