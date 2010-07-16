#pragma once

class DMS 
{
public:
	DMS();
	DMS (double angle);
	void calculate (double angle);
	int deg, min, sec;
    int zodiac;
    int zod_deg;
};
