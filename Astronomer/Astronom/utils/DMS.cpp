#include "DMS.h"
#include "constants.h"
#include <math.h>
#include <stdlib.h>

DMS::DMS()
: deg(0), min(0), sec(0), zodiac(0), zod_deg(0)
{
}

DMS::DMS (double angle)
{
	calculate (angle);
}

void DMS::calculate (double angle)
{
	deg = int(angle);
	angle = fabs(angle);
	angle -= abs(deg);
	angle *= 60;
	min = int(angle);
	angle -= min;
	angle *= 60;
	sec = int(angle);
	angle = deg / DEG_PER_SIGN;
	zodiac = int(angle);
	zod_deg = int(deg - zodiac * DEG_PER_SIGN);
}
