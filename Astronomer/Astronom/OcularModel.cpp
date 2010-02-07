#include "OcularModel.h"
#include "DraggableView.h"

OcularModel::OcularModel()
: view_(0)
{
}

OcularModel::~OcularModel(void)
{
}

void OcularModel::setView(DraggableView* view)
{
	view_ = view;
	view_->handle (0, FXSEL(SEL_COMMAND, astro::ID_SET_ZERO), (void*)ZERO_ARIES);
	OcularDimensions odim(90, 80, 68, 60, 50);
	view_->handle (0, FXSEL(SEL_COMMAND, astro::ID_SET_OCULAR_DIM), (void*)&odim);
}
