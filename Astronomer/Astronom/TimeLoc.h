#pragma once
#include <fx.h>

enum timeloc_t {
	TL_DATE = 0,
	TL_TIME,
	TL_TZ,
	TL_LAT,
	TL_LON,
	TL_ELV,
	TL_LAST, // do not use, always last
};

enum date_fmt_t {
	DF_YMD = 0,
	DF_DMY,
	DF_MDY
};

class TimeLoc
{
public:
	static void initRex(char date_sep);
	TimeLoc();
	virtual ~TimeLoc();
	void set (timeloc_t idx, FX::FXString text, bool recalculate = true);
	void set (timeloc_t idx, double val);
	void setName (const FX::FXString& text);
	void setLocation (const FX::FXString& text);

	double get (timeloc_t idx) const;
	const FXString& getStr (timeloc_t idx);
	const FXString& getName () const;
	const FXString& getLocation () const;

	void serialize(FXString& output);
	void deserialize(const FXString& input);
	void asTitle(FXString& output);

	static FXString formatDate (int y, int m, int d);
	static int scan (timeloc_t idx, const FXString &str, int *out);

	static FXString& toBackTick (FXString& str);
	static FXString& fromBackTick (FXString& str);
private:
	double data_[TL_LAST];
	FXString str_[TL_LAST];
	FXString name_;
	FXString location_;

	static date_fmt_t date_fmt_;
	static char date_sep_;
	static FXRex rex_[TL_LAST];
};

class BodyProps {
public:
	enum body_property {
		bp_Lon = 0,
		bp_Lat,
		bp_Dist,
		bp_LonSpeed,
		bp_LatSpeed,
		bp_DistSpeed
	};
	double prop[6];
};
