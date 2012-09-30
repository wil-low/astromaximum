#include "datafile.h"
#include <stdlib.h>
#include <string.h>

const int TRANSITION_NAME_LEN = 20;

int readInt(pDatafile df) {
	int res = df->read_pos_[0];
	res <<= 8;
	res += df->read_pos_[1];
	res <<= 8;
	res += df->read_pos_[2];
	res <<= 8;
	res += df->read_pos_[3];
	df->read_pos_ += 4;
	return res;
}

int readShort(pDatafile df) {
	short res = df->read_pos_[0];
	res <<= 8;
	res += df->read_pos_[1];
	df->read_pos_ += 2;
	return res;
}

int readUnsignedByte(pDatafile df) {
	unsigned char res = df->read_pos_[0];
	++df->read_pos_;
	return res;
}

int readByte(pDatafile df) {
	char res = df->read_pos_[0];
	++df->read_pos_;
	return res;
}

size_t readUTF(pDatafile df, char* dest) {
	size_t len = readShort(df);
	strncpy(dest, df->read_pos_, len);
	dest[len] = 0;
	df->read_pos_ += len;
	return len;
}

int dateBetween(int date, int start, long end) {
	if (date < start) {
		return -1;
	}
	if (date >= end) {
		return 1;
	}
	return 0;
}

int is_event_in_period(const pEvent ev, int start, int end, int is_special) {
	if (ev->date0_ == 0) {
		return 0;
	}
	int f = dateBetween(ev->date0_, start, end) + dateBetween(ev->date0_, start, end);
	if (f == 2 || f == -2)
		return 0;
	if (is_special) {
		if (f == -1)
			return 0;
	}
	return 1;
}

/*
int isInPeriod(const pEvent e, long start, long end) {
	if (!e->date0_) {
		return 0;
	}
	return dateBetween(e->date0_, start, end) == 0;
}
 */
void freeCommonHeader(pHeaderCommon hdr) {
	if (hdr) {
		free(hdr->data_);
		hdr->data_ = 0;
		free(hdr);
	}
}

void freeLocationHeader(pHeaderLocation hdr) {
	if (hdr) {
		free(hdr->transitionTimes_);
		free(hdr->transitionOffsets_);
		int i = 0;
		for (; i < hdr->transitionCount_; ++i)
			free(hdr->transitionNames_[i]);
		free(hdr->transitionNames_);
		free(hdr);
	}
}

pDatafile datafile_create() {
	setenv("TZ", ":Etc/GMT", 1);
	pDatafile result = malloc(sizeof (struct datafile));
	result->hdr_common_ = 0;
	result->hdr_location_ = 0;
	result->read_pos_ = 0;
	return result;
}

int datafile_init(pDatafile result, enum datafile_type df_type, size_t len, const char* data) {
	result->type_ = df_type;
	switch (df_type) {
		case DFT_COMMON:
		{
			freeCommonHeader(result->hdr_common_);
			result->hdr_common_ = 0;
			pHeaderCommon hdr = malloc(sizeof (struct header_common));
			hdr->data_len_ = len;
			hdr->data_ = data;
			result->read_pos_ = hdr->data_;
			hdr->start_year_ = readShort(result);
			hdr->start_month_ = readUnsignedByte(result);
			hdr->start_day_ = readUnsignedByte(result);
			hdr->custom_data_len_ = readUTF(result, hdr->custom_data_);
			hdr->day_count_ = readShort(result);
			result->startJD_ = date2time_t(hdr->start_year_, hdr->start_month_, hdr->start_day_, 00, 00);
			result->finalJD_ = result->startJD_ + hdr->day_count_ * SEC_IN_DAY - 1;
			hdr->data_len_ = len - (result->read_pos_ - hdr->data_);
			hdr->data_ = malloc(hdr->data_len_);
			memcpy(hdr->data_, result->read_pos_, hdr->data_len_);

			result->hdr_common_ = hdr;
		}
			break;
		case DFT_LOCATION:
		{
			freeLocationHeader(result->hdr_location_);
			//printf ("tzname1 = %s, %s\n", tzname[0], tzname[1]);
			result->hdr_location_ = 0;
			pHeaderLocation hdr = malloc(sizeof (struct header_location));
			hdr->data_len_ = len;
			hdr->data_ = data;
			result->read_pos_ = hdr->data_;
			int signature = readInt(result);
			if (signature != 0x53265741) {
				free(hdr);
				return 1;
			}
			char version = readByte(result);
			if (version != 2) {
				free(hdr);
				return 2;
			}
			hdr->start_year_ = readShort(result);
			hdr->start_month_ = readUnsignedByte(result);
			hdr->start_day_ = readUnsignedByte(result);
			hdr->day_count_ = readShort(result);
			hdr->city_id_ = readInt(result);
			hdr->coords[0] = readShort(result); // latitude
			hdr->coords[1] = readShort(result); // longitude
			hdr->coords[2] = readShort(result); // altitude
			readUTF(result, hdr->city_);
			readUTF(result, hdr->state_);
			readUTF(result, hdr->country_);
			readUTF(result, hdr->timezone_);
			hdr->custom_data_len_ = readUTF(result, hdr->custom_data_);
			hdr->transitionCount_ = readByte(result);
			hdr->transitionTimes_ = malloc(hdr->transitionCount_ * sizeof (time_t));
			hdr->transitionOffsets_ = malloc(hdr->transitionCount_ * sizeof (int));
			hdr->transitionNames_ = malloc(hdr->transitionCount_ * sizeof (char*));
			int i = 0;
			for (; i < hdr->transitionCount_; ++i) {
				hdr->transitionTimes_[i] = readInt(result); // start_date
				hdr->transitionOffsets_[i] = readShort(result); // gmt_ofs_min
				hdr->transitionOffsets_[i] *= 60;
				hdr->transitionNames_[i] = malloc(TRANSITION_NAME_LEN);
				readUTF(result, hdr->transitionNames_[i]); // name
				/*
				char buffer[30];
				struct tm* dt = gmtime (&hdr->transitionTimes_[i]);
				sprintf (buffer, "%d-%02d-%02d %02d:%02d\n", 
						dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday, dt->tm_hour, dt->tm_min);
				printf ("%d, %s, %d - %s > %s\n",
						hdr->transitionTimes_[i],
						asctime(gmtime(&hdr->transitionTimes_[i])),
						hdr->transitionOffsets_[i],
						hdr->transitionNames_[i],
						buffer);
				 */
			}
			//printf ("tzname2 = %s, %s\n", tzname[0], tzname[1]);

			hdr->data_len_ = len - (result->read_pos_ - hdr->data_);
			hdr->data_ = malloc(hdr->data_len_);
			memcpy(hdr->data_, result->read_pos_, hdr->data_len_);

			result->hdr_location_ = hdr;
		}
			break;
	}
	return 0;
}

void datafile_fini(pDatafile df) {
	if (df) {
		freeCommonHeader(df->hdr_common_);
		df->hdr_common_ = 0;
		freeLocationHeader(df->hdr_location_);
		df->hdr_location_ = 0;
		free(df);
	}
}

int datafile_tz_offset(const pDatafile df, const time_t* date) {
	int offset = df->hdr_location_->transitionOffsets_[0];
	int i = 1;
	for (; i < df->hdr_location_->transitionCount_; ++i) {
		if (df->hdr_location_->transitionTimes_[i] >= *date) {
			offset = df->hdr_location_->transitionOffsets_[i - 1];
			break;
		}
	}
	return offset;
}

time_t date2time_t(int year, int month, int day, int hour, int min) {
	struct tm datetime;
	datetime.tm_year = year - 1900;
	datetime.tm_mon = month - 1;
	datetime.tm_mday = day;
	datetime.tm_hour = hour;
	datetime.tm_min = min;
	datetime.tm_sec = 0;
	datetime.tm_isdst = -1;
	time_t result = mktime(&datetime);
	//printf ("%d-%02d-%02d %02d:%02d = %d\n", year, month, day, hour, min, result);
	return result;
}

void make_event(pEvent event, int date0, int date1) {
	event->date0_ = date0;
	event->date1_ = date1;
	event->planet0_ = event->planet1_ = -1;
	event->degree_ = 127;
}

void copy_event(pEvent dest, pEvent src) {
	memcpy(dest, src, sizeof (struct datafile_event));
}

int readSubData(pDatafile df, int evtype, int planet, enum datafile_type type, time_t dayStart, time_t dayEnd, pEvent events) {
	const int EF_DATE = 0x1; // contains 2nd date - 4b
	const int EF_PLANET1 = 0x2; // contains 1nd planet - 1b
	const int EF_PLANET2 = 0x4; // contains 2nd planet - 1b
	const int EF_DEGREE = 0x8; // contains degree or angle - 2b
	const int EF_CUMUL_DATE_B = 0x10; // date are cumulative from 1st 4b - 1b
	const int EF_CUMUL_DATE_W = 0x20; // date are cumulative from 1st 4b - 2b
	const int EF_SHORT_DEGREE = 0x40; // contains angle 0..180 - 1b
	const int EF_NEXT_DATE2 = 0x80; // 2nd date is 1st in next event

	const int ROUNDING_SEC = 60;

	int eventsCount = 0;
	int flag;
	int skipOff;
	struct datafile_event last;
	make_event(&last, 0, 0);
	int fnext_date2;
	const char* buf = 0;
	int data_len = 0;
	if (type == DFT_COMMON) {
		buf = df->hdr_common_->data_;
		data_len = df->hdr_common_->data_len_;
	} else {
		buf = df->hdr_location_->data_;
		data_len = df->hdr_location_->data_len_;
	}
	df->read_pos_ = buf;
	unsigned char* eof = df->read_pos_ + data_len;

	int PERIOD = (evtype == EV_ASCAPHETICS) ? 2 * 60 : 24 * 60;
	while (df->read_pos_ < eof) {
		++df->read_pos_; // skip imei char
		int rub = readUnsignedByte(df);
		while (evtype != rub) {
			skipOff = readShort(df) - 3;
			df->read_pos_ += skipOff + 1;
			rub = readUnsignedByte(df);
		}
		skipOff = readShort(df);
		flag = readShort(df);
		if (planet == readByte(df)) {
			break;
		} else {
			df->read_pos_ += skipOff - 6;
		}
	}
	int count = readShort(df);
	int fcumul_date_b = (flag & EF_CUMUL_DATE_B);
	int fcumul_date_w = (flag & EF_CUMUL_DATE_W);
	int fdate = (flag & EF_DATE);
	int fplanet1 = (flag & EF_PLANET1);
	int fplanet2 = (flag & EF_PLANET2);
	int fdegree = (flag & EF_DEGREE);
	int fshort_degree = (flag & EF_SHORT_DEGREE);
	fnext_date2 = (flag & EF_NEXT_DATE2);

	char myplanet0 = planet, myplanet1 = -1;
	int mydgr = 127;
	int mydate0, mydate1;
	int cumul;
	int date = 0;
	int i = 0;
	for (; i < count; i++) {
		if (fcumul_date_b != 0) {
			if (i != 0) {
				cumul = readByte(df);
				date += (cumul + PERIOD) * 60;
			} else {
				date = readInt(df);
			}
		} else if (fcumul_date_w != 0) {
			if (i != 0) {
				cumul = readShort(df);
				date += (cumul + PERIOD) * 60;
			} else {
				date = readInt(df);
			}
		} else {
			date = readInt(df);
		}
		mydate0 = date;
		if (fdate != 0)
			mydate1 = readInt(df) - 1;
		else
			mydate1 = mydate0;
		if (fplanet1 != 0)
			myplanet0 = readByte(df);
		if (fplanet2 != 0)
			myplanet1 = readByte(df);
		if (fdegree != 0) {
			if (fshort_degree != 0)
				mydgr = readUnsignedByte(df);
			else
				mydgr = readShort(df);
		}
		if (fnext_date2 != 0) {
			last.date1_ = mydate0 - ROUNDING_SEC;
			mydate1 = df->finalJD_;
		}
		if (is_event_in_period(&last, dayStart, dayEnd, 0))
			copy_event(&(events[eventsCount++]), &last);
		else if (eventsCount > 0)
			break;
		last.planet0_ = myplanet0;
		last.planet1_ = myplanet1;
		last.degree_ = mydgr;
		last.date0_ = mydate0;
		last.date1_ = mydate1;
	}
	if (is_event_in_period(&last, dayStart, dayEnd, 0)) {
		copy_event(&events[eventsCount++], &last);
	}
	return eventsCount;
}

int datafile_get_events(const pDatafile df, int evtype, int planet, time_t dayStart, time_t dayEnd, pEvent events) {
	switch (evtype) {
		case EV_ASTRORISE:
		case EV_ASTROSET:
		case EV_RISE:
		case EV_SET:
		case EV_NAVROZ:
		case EV_ASCAPHETICS:
			return readSubData(df, evtype, planet, DFT_LOCATION, dayStart, dayEnd, events);
		default:
			return readSubData(df, evtype, planet, DFT_COMMON, dayStart, dayEnd, events);
	}
}

const char* time_t2str(const time_t* datetime, char* buffer) {
	struct tm* dt = gmtime(datetime);
	sprintf(buffer, "%d-%02d-%02d %02d:%02d",
			dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday, dt->tm_hour, dt->tm_min);
	return buffer;
}

const char* planet_name(int planet) {
	return PLANET_NAME[planet + 1];
}

const char* event_dump(pEvent ev, char* buffer) {
	char buf0[30], buf1[30];
	sprintf(buffer, "Evt %s - %s, %s-%s, dgr %d",
			time_t2str(&ev->date0_, buf0),
			time_t2str(&ev->date1_, buf1),
			planet_name(ev->planet0_),
			planet_name(ev->planet1_),
			ev->degree_);
}
