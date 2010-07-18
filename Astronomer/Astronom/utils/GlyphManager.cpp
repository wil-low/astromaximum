#include "GlyphManager.h"

void GlyphManager::init(FXApp* a)
{
	app_ = a;
	loadFont(astrofont_map_, "Astronom");
	loadFont(arialfont_map_, "Arial");
}

void GlyphManager::fini()
{
	clearFonts(astrofont_map_);
	clearFonts(arialfont_map_);
}

FXFont* GlyphManager::getFont(int size, font_face_t face) const
{
	FXFont* fnt = NULL;
	const font_map& map = (face == FF_ASTRO) ? astrofont_map_ : arialfont_map_;
	font_map::const_iterator it = map.lower_bound(size);
	if (it != map.end())
		fnt = it->second;
	else
		fnt = map.rbegin()->second;
	return fnt;
}

void GlyphManager::clearFonts(font_map& map)
{
	for (font_map::iterator it = map.begin(); it != map.end(); ++it)
		delete (*it).second;
	map.clear();
}

void GlyphManager::loadFont(font_map& map, const FXString& face)
{
	const int FONT_SIZES[] = {8, 9, 10, 11, 12, 13, 14, 16, 18, 22, 30, 36, 40, 48, 56, 60};
	clearFonts(map);
	for (int i = 0; i < ARRAYNUMBER(FONT_SIZES); ++i) {
		FXFont* fnt = new FXFont(app_, face,
			FONT_SIZES[i], FXFont::Normal, FXFont::Straight, FONTENCODING_UNICODE);
		if (fnt != NULL) {
			fnt->create();
			map[FONT_SIZES[i]] = fnt;
		}
	}
}

FXchar GlyphManager::getSignLabel(int sign) const
{
	return sign + '@';
}

FXchar GlyphManager::getPlanetLabel(int planet) const
{
	return planet + '0' + 32;
}

FXchar GlyphManager::getDegreeSign(font_face_t face) const
{
	if (face == FF_ASTRO)
		return '9' + 2;
	else
		return '°';
}
