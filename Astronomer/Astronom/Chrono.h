#pragma once
#include <fx.h>
class Chrono : public FXTopWindow
{
	FXDECLARE(Chrono)
public:
	Chrono(FXWindow* wo);
	~Chrono(void);
	
	// Messages for our class
	enum{
		ID_TABLE=FXMainWindow::ID_LAST,
		ID_LAST
	};

protected:
	Chrono(){}
};
