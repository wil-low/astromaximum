#pragma once
#include <fx.h>
#include <map>
#include <boost/serialization/singleton.hpp>
#include "../utils/constants.h"

class GlyphManager : public boost::serialization::singleton<GlyphManager>
{
public:
	void init(FXApp* a);
	void fini();
	
	FXchar getSignLabel(int sign) const;
	FXchar getPlanetLabel(int planet) const;
	FXchar getDegreeSign(font_face_t face) const;

	FXFont* getFont(int size, font_face_t face) const;
private:
	typedef std::map<int, FXFont*> font_map;
	void clearFonts(font_map& map);
	void loadFont(font_map& map, const FXString& face);
	font_map astrofont_map_;
	font_map arialfont_map_;
	FXApp* app_;
};
