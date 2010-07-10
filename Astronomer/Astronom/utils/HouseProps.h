#pragma once

class HouseProps {
public:
	HouseProps() : method(hp_Placidus) {}
    enum house_method {
        hp_Placidus = 'P',
        hp_Koch = 'K',
        hp_Regiomontanus = 'R',
        hp_Campanus = 'C',
        hp_Equal = 'E',
        hp_Gaquelin = 'G',
    } method;
	int getCuspCount() const {return (method == hp_Gaquelin) ? 36 : 12;}
	double cusps[37];
	double ascmc[10];
};

