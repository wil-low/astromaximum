#include "BaseModel.h"

BaseModel::BaseModel(Ephemeris* ephemeris)
: ephemeris_(ephemeris)
, view_(0)
{
}

BaseModel::~BaseModel()
{
}
