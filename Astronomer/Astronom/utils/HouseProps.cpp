#include "HouseProps.h"

HouseProps::HouseProps()
: method(hp_Placidus) 
{
}

int HouseProps::getCuspCount() const
{
	return (method == hp_Gaquelin ? 36 : 12);
}
