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

	FXchar getLabel(body_type_t type, int id) const;
	FXchar getName(body_type_t type, int id) const;
	FXchar getDegreeSign(font_face_t face) const;
	FXString getHouseLabel(int id, house_flag_t hf) const;

	FXFont* getFont(int size, font_face_t face) const;
	static FXString& toBackTick (FXString& str);
	static FXString& fromBackTick (FXString& str);
private:
	typedef std::map<int, FXFont*> font_map;
	void clearFonts(font_map& map);
	void loadFont(font_map& map, const FXString& face);
	font_map astrofont_map_;
	font_map arialfont_map_;
	FXApp* app_;
};
