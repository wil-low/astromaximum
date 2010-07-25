#include "GlyphManager.h"

const FXString DEG_STR("°");
const FXString BACKTICK_STR("`");

void GlyphManager::init(FXApp* a)
{
	app_ = a;
	reg_ = new FXSettings;
	reg_->parseFile("settings/global.txt", false);
	loadFont(astrofont_, "font:Astronom");
	loadFont(arialfont_, "font:Arial");
}

void GlyphManager::fini()
{
	clearFonts(astrofont_);
	clearFonts(arialfont_);
	delete reg_;
}

FXFont* GlyphManager::getFont(int size, font_face_t face) const
{
	FXFont* fnt = NULL;
	const size_font_map& map = ((face == FF_ASTRO) ? astrofont_ : arialfont_).fonts_;
	size_font_map::const_iterator it = map.lower_bound(size);
	if (it != map.end())
		fnt = it->second;
	else
		fnt = map.rbegin()->second;
	return fnt;
}

void GlyphManager::clearFonts(font_glyph_t& font)
{
	for (size_font_map::iterator it = font.fonts_.begin(); it != font.fonts_.end(); ++it)
		delete (*it).second;
	font.fonts_.clear();
	font.planets_.clear();
	font.zodiac_signs_.clear();
}

void GlyphManager::loadFont(font_glyph_t& font, const FXString& face)
{
	const int FONT_SIZES[] = {8, 9, 10, 11, 12, 13, 14, 16, 18, 22, 30, 36, 40, 48, 56, 60};
	clearFonts(font);
	for (int i = 0; i < ARRAYNUMBER(FONT_SIZES); ++i) {
		FXFont* fnt = new FXFont(app_, face,
			FONT_SIZES[i], FXFont::Normal, FXFont::Straight, FONTENCODING_UNICODE);
		if (fnt != NULL) {
			fnt->create();
			font.fonts_[FONT_SIZES[i]] = fnt;
		}
	}
	font.glyphs_ = reg_->find(face);
	const char DELIMITER = ' ';
	s = reg_->readStringEntry(face, "zodiac", "");
	space_count = s.contains(DELIMITER);
	for (int i = 0; i <= space_count; ++i)
		font.zodiac_signs_.push_back(s.section(DELIMITER, i).toInt());
}

FXchar GlyphManager::getLabel(body_type_t type, int id) const
{
	switch (type) {
		case TYPE_ZODIAC:
			return astrofont_.zodiac_signs_[id];
		case TYPE_PLANET:
			return astrofont_.planets_[id];
		case TYPE_HOUSE:
			switch ((astro_flag_t)id) {
				case af_Asc:
					return 'L';
				case af_IC:
					return 'O';
				case af_Dsc:
					return 'M';
				case af_MC:
					return 'N';
			}
	}
	return '?';
}

FXchar GlyphManager::getDegreeSign(font_face_t face) const
{
	if (face == FF_ASTRO)
		return '9' + 2;
	else
		return '`';
}

FXString GlyphManager::getHouseLabel(int id, astro_flag_t af) const
{
	if (af == af_Undef)
		return FXString(HOUSE_NAMES[id - HOUSE_ID_FIRST - 1]);
	FXString s;
	s.format("%c", getLabel(TYPE_HOUSE, (int)af));
	return s;
}

FXString& GlyphManager::toBackTick(FXString& str)
{
	return str.substitute(DEG_STR, BACKTICK_STR);
}

FXString& GlyphManager::fromBackTick(FXString& str)
{
	return str.substitute(BACKTICK_STR, DEG_STR);
}
