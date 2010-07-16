#include "TimeLoc.h"
#include "Ephemeris.h"
#include <fx.h>

const char *FMT_DATE[] = {
	"%04d%c%02d%c%02d", // YMD
	"%02d%c%02d%c%04d", // DMY
	"%02d%c%02d%c%04d", // MDY
};

const FXString DEG_STR("°");
const FXString BACKTICK_STR("`");

const char* FMT_REX[] = {
	"(\\d+)\\%c(\\d+)\\%c(\\d+)", // DATE
	"(\\d+):(\\d+):(\\d+)", // TIME
	"([+-])(\\d+):(\\d+):(\\d+)", // TZ
	"(\\d+)`(\\d+)'(\\d+)''([NS])", // LAT
	"(\\d+)`(\\d+)'(\\d+)''([EW])", // LON
	"(\\d+)`(\\d+)`(\\d+)", // ELV
};

const char FMT_TIME[] = "%02d:%02d:%02d";
const char FMT_LAT[]  = "%03d`%02d'%02d''%c";
const char FMT_LON[]  = "%02d`%02d'%02d''%c";
const char FMT_TZ[]   = "%c%02d:%02d:%02d";

const char sep = ';';

FXRex TimeLoc::rex_[TL_LAST];
date_fmt_t TimeLoc::date_fmt_ = DF_DMY;
char TimeLoc::date_sep_;

TimeLoc::TimeLoc()
{
	for (int i = TL_DATE; i < TL_LAST; ++i)
		data_[i] = 0;
}

void TimeLoc::initRex(char date_sep)
{
	date_sep_ = date_sep;
	char pattern[100];
	sprintf (pattern, FMT_REX[TL_DATE], date_sep_, date_sep_);
	FXRex::Error err = rex_[TL_DATE].parse(pattern, FXRex::Capture);
	if (err != FXRex::ErrOK)
		FXTRACE((10, "%s: rex TL_DATE %s\n", __FUNCTION__, FXRex::getError(err)));
	for (int i = TL_TIME; i < TL_LAST; ++i) {
		err = rex_[i].parse(FMT_REX[i], FXRex::Capture);
		if (err != FXRex::ErrOK)
			FXTRACE((10, "%s: date_rex %s\n", __FUNCTION__, FXRex::getError(err)));
	}
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

int TimeLoc::scan (timeloc_t idx, const FXString &str, int *out)
{
	FXString s(str);
	toBackTick(s);
	FXint beg[5], end[5];
	int value[4];
	if(rex_[idx].match(s, beg, end, FXRex::Forward, 5)) {
		value[0] = str.mid(beg[1], end[1] - beg[1]).toInt();
		value[1] = str.mid(beg[2], end[2] - beg[2]).toInt();
		value[2] = str.mid(beg[3], end[3] - beg[3]).toInt();
		value[3] = str.mid(beg[4], end[4] - beg[4]).toInt();
		if (idx == TL_DATE) {
			switch (date_fmt_) {
				case DF_YMD:
					out[0] = value[0]; out[1] = value[1]; out[2] = value[2];
					break;
				case DF_DMY:
					out[2] = value[0]; out[1] = value[1]; out[0] = value[2];
					break;
				case DF_MDY:
					out[1] = value[0]; out[2] = value[1]; out[0] = value[2];
					break;
			}
		}
		else {
			out[0] = value[0]; out[1] = value[1]; out[2] = value[2]; out[3] = value[3];
		}
		return 0;
	}
	return -1;
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
				double m = (val - d) * 60;
				int s = (m - (int)m) * 60;
				res.format (FMT_LAT, d, (int)m, s, c);
				fromBackTick(res);
			}
				break;
			case TL_LON:
			{
				char c = val >= 0 ? 'E' : 'W';
				int d = val;
				double m = (val - d) * 60;
				int s = (m - (int)m) * 60;
				res.format (FMT_LON, d, (int)m, s, c);
				fromBackTick(res);
			}
				break;
			case TL_TZ:
			{
				char c = val >= 0 ? '+' : '-';
				int d = val;
				double m = (val - d) * 60;
				int s = (m - (int)m) * 60;
				res.format (FMT_TZ, c, d, (int)m, s);
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
	if ((idx == TL_DATE) || (idx == TL_TIME)) {
		data_[TL_DATE] = data_[TL_TIME] = val;
		str_[TL_DATE] = str_[TL_TIME] = "";
		return;
	}
	data_[idx] = val;
	str_[idx].clear();
}

void TimeLoc::set (timeloc_t idx, FX::FXString text, bool recalculate)
{
	text.trim();
	if (recalculate == false) {
		str_[idx] = text;
		return;
	}
	str_[idx].clear();
	int out[5];
	double res = 0;
	switch (idx) {
		case TL_DATE:
		{
			// format is "2010/05/08 16:50:12"
			int outtm[5];
			FXString date = text.section(' ', 0);
			FXString time = text.section(' ', 1);

			if (scan (TL_DATE, date, out) == 0 &&
                scan (TL_TIME, time, outtm) == 0) {
                    data_[TL_DATE] = data_[TL_TIME] = Ephemeris::julday (out[0], out[1], out[2], outtm[0], outtm[1], outtm[2]);
                    str_[TL_DATE] = date;
                    str_[TL_TIME] = time;
			}
		}
		break;
		case TL_LAT:
		{
			TimeLoc::toBackTick(text);
			if (scan (TL_LAT, text, out) == 0) {
				res = out[0] + out[1] / 60.L + out[2] / 3600.L;
				if (out[3] == 'S')
					res = -res;
				data_[idx] = res;
				str_[idx] = text;
				TimeLoc::fromBackTick(str_[idx]);
			}
		}
		break;
		case TL_LON:
		{
			TimeLoc::toBackTick(text);
			if (scan (TL_LON, text, out) == 0) {
				res = out[0] + out[1] / 60.L + out[2] / 3600.L;
				if (out[3] == 'W')
					res = -res;
				data_[idx] = res;
				str_[idx] = text;
				TimeLoc::fromBackTick(str_[idx]);
			}
		}
		break;
		case TL_TZ:
		{
			if (scan(TL_TZ, text, out) == 0) {
				res = (out[1] + out[2] / 60.L + out[3] / 3600.L) / 24.L;
				if (out[0] == '-')
				  res = -res;
				data_[idx] = res;
				str_[idx] = text;
			}
		}
		break;
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

void TimeLoc::asTitle(FXString& output)
{
    output = name_ + " " +
		getStr(TL_DATE) + " " +
		getStr(TL_TIME) + " " +
		getStr(TL_TZ) + ", " +
		location_ + " " +
		getStr(TL_LAT) + ", " +
		getStr(TL_LON);
}

FXString& TimeLoc::toBackTick(FXString& str)
{
	return str.substitute(DEG_STR, BACKTICK_STR);
}

FXString& TimeLoc::fromBackTick(FXString& str)
{
	return str.substitute(BACKTICK_STR, DEG_STR);
}
