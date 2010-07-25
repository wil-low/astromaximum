#pragma once
#include <fx.h>
#include <map>
#include <boost/serialization/singleton.hpp>
#include "../utils/constants.h"
#include <vector>

class GlyphManager : public boost::serialization::singleton<GlyphManager>
{
public:
	void init(FXApp* a);
	void fini();

	FXchar getLabel(body_type_t type, int id) const;
	FXchar getName(body_type_t type, int id) const;
	FXchar getDegreeSign(font_face_t face) const;
	FXString getHouseLabel(int id, astro_flag_t hf) const;

	FXFont* getFont(int size, font_face_t face) const;
	static FXString& toBackTick (FXString& str);
	static FXString& fromBackTick (FXString& str);
private:
	typedef std::map<int, FXFont*> size_font_map;
	struct font_glyph_t {
		size_font_map fonts_;
		FXStringDict* glyphs_;
		std::vector<int> planets_;
		std::vector<int> zodiac_signs_;
	};
	void clearFonts(font_glyph_t& font);
	void loadFont(font_glyph_t& font, const FXString& face);
	font_glyph_t astrofont_;
	font_glyph_t arialfont_;
	FXApp* app_;
	FXSettings* reg_;
};
