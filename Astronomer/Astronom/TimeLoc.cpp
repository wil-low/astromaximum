#include "TimeLoc.h"
#include "Ephemeris.h"
#include <fx.h>

const char *FMT_DATE[] = {
	"%04d%c%02d%c%02d", // YMD
	"%02d%c%02d%c%04d", // DMY
	"%02d%c%02d%c%04d", // MDY
};

const char FMT_TIME[] = "%02d:%02d:%02d";
const char FMT_LAT[]  = "%02d°%02d'%c";
const char FMT_LON[]  = "%03d°%02d'%c";
const char FMT_TZ[]   = "%c%02d:%02d";

const char sep = ';';

date_fmt_t TimeLoc::date_fmt_ = DF_DMY;
char TimeLoc::date_sep_ = '.';

TimeLoc::TimeLoc()
{
	for (int i = TL_DATE; i < TL_LAST; ++i)
		data_[i] = 0;
}

TimeLoc::~TimeLoc()
{
}

FXString TimeLoc::formatDate (int y, int m, int d)
{
	FXString res;
	const char* p_fmt = FMT_DATE[date_fmt_];
	switch (date_fmt_) {
		case DF_YMD:
			res.format(p_fmt, y, date_sep_, m, date_sep_, d);
			break;
		case DF_DMY:
			res.format(p_fmt, d, date_sep_, m, date_sep_, y);
			break;
		case DF_MDY:
			res.format(p_fmt, m, date_sep_, d, date_sep_, y);
			break;
	}
	return res;
}

const FXString& TimeLoc::getName () const
{
	return name_;
}

const FXString& TimeLoc::getLocation () const
{
	return location_;
}

double TimeLoc::get (timeloc_t idx) const
{
	return data_[idx];
}

const FXString& TimeLoc::getStr (timeloc_t idx)
{
	if (str_[idx].empty()) {
		FXString res;
		double val = data_[idx];
		switch (idx) {
			case TL_DATE:
			{
				int y = 0, m = 0, d = 0, h = 0, min = 0, s = 0;
				Ephemeris::revjul (val, &y, &m, &d, &h, &min, &s);
				res = formatDate(y, m, d);
			}
				break;
			case TL_TIME:
			{
				int y = 0, m = 0, d = 0, h = 0, min = 0, s = 0;
				Ephemeris::revjul (val, &y, &m, &d, &h, &min, &s);
				res.format (FMT_TIME, h, min, s);
			}
				break;
			case TL_LAT:
			{
				char c = val >= 0 ? 'N' : 'S';
				int d = val;
				int m = (val - d) * 60;
				res.format (FMT_LAT, d, m, c);
			}
				break;
			case TL_LON:
			{
				char c = val >= 0 ? 'E' : 'W';
				int d = val;
				int m = (val - d) * 60;
				res.format (FMT_LON, d, m, c);
			}
				break;
			case TL_TZ:
			{
				char c = val >= 0 ? '+' : '-';
				int d = val;
				int m = (val - d) * 60;
				res.format (FMT_TZ, c, d, m);
			}
				break;
			case TL_ELV:
				break;
		}
		str_[idx] = res;
	}
	return str_[idx];
}

void TimeLoc::setName (const FX::FXString& text)
{
	name_ = text;
}

void TimeLoc::setLocation (const FX::FXString& text)
{
	location_ = text;
}

void TimeLoc::set (timeloc_t idx, double val)
{
	if (idx == TL_TIME) {
		data_[idx] = val;
		str_[idx].clear();
		idx = TL_DATE;
	}
	data_[idx] = val;
	str_[idx].clear();
}

void TimeLoc::set (timeloc_t idx, const FX::FXString& text, bool recalculate)
{
	if (recalculate == false) {
		str_[idx] = text;
		return;
	}
	str_[idx].clear();
	double res = 0;
	switch (idx) {
		case TL_DATE:
		{
			// format is "2010/05/08 16:50:12"
			int y = 0, m = 0, d = 0, h = 0, min = 0, s = 0;
			if (text.scan (FMT_DATE[date_fmt_], &y, &m, &d, &h, &min, &s) >= 3) {// time optional?
				data_[idx] = Ephemeris::julday (y, m, d, h, min, s);
				str_[idx] = text;
			}
		}
		break;
		case TL_LAT:
		{
			char c; int d, m;
			if (text.scan (FMT_LAT, &d, &m, &c) == 3) {
				res = d + m / 60.L;
				if (c == 'S')
					res = -res;
				data_[idx] = res;
				str_[idx] = text;
			}
		}

		case TL_LON:
		{
			char c; int d, m;
			if (text.scan (FMT_LON, &d, &m, &c) == 3) {
				res = d + m / 60.L;
				if (c == 'W')
					res = -res;
				data_[idx] = res;
				str_[idx] = text;
			}
		}
		case TL_TZ:
		{
			int hour,min;
			char c;
			if (text.scan(FMT_TZ, &c, &hour, &min) == 3) {
				res = (hour + min / 60.L) / 24.L;
				if (c == '-')
				  res = -res;
				data_[idx] = res;
				str_[idx] = text;
			}
		}
	}
	if (str_[idx].empty()) {
		FXTRACE((10, "%s: Cannot set item %d to '%s'\n", __FUNCTION__, idx, text.text()));
	}
}

void TimeLoc::serialize(FXString& output)
{
	output =
		name_ + sep +
		getStr(TL_DATE) + sep +
		getStr(TL_TIME) + sep +
		getStr(TL_TZ) + sep +
		location_ + sep +
		getStr(TL_LAT) + sep +
		getStr(TL_LON) + sep;
}

void TimeLoc::deserialize(const FXString& input)
{
	setName(input.section(sep, 0));
	set(TL_DATE, input.section(sep, 1), false);
	set(TL_TIME, input.section(sep, 2), false);
	set(TL_TZ, input.section(sep, 3), false);
	setLocation(input.section(sep, 4));
	set(TL_LAT, input.section(sep, 5), false);
	set(TL_LON, input.section(sep, 6), false);
}

