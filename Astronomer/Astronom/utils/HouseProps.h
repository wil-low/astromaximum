#pragma once

class HouseProps {
public:
	HouseProps();
    enum house_method {
        hp_Placidus = 'P',
        hp_Koch = 'K',
        hp_Regiomontanus = 'R',
        hp_Campanus = 'C',
        hp_Equal = 'E',
        hp_Gaquelin = 'G',
    } method;
	int getCuspCount() const;
	double cusps[37];
	double ascmc[10];
};

