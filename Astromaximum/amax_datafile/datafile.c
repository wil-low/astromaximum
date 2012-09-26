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

int dateBetween(long date0, long start, long end) {
    if (date0 < start) {
        return -1;
    }
    if (date0 >= end) {
        return 1;
    }
    return 0;
}

int isInPeriod(const pEvent e, long start, long end) {
    if (!e->date0_) {
        return 0;
    }
    return dateBetween(e->date0_, start, end) == 0;
}

void freeCommonHeader (pHeaderCommon hdr) {
    if (hdr) {
		free (hdr->data_);
		hdr->data_ = 0;
		free (hdr);
	}
}

void freeLocationHeader (pHeaderLocation hdr) {
    if (hdr) {
        free (hdr->transitionTimes_);
        free (hdr->transitionOffsets_);
        int i = 0;
        for (; i < hdr->transitionCount_; ++i)
            free (hdr->transitionNames_[i]);
        free (hdr->transitionNames_);
	    free (hdr);
    }
}

pDatafile datafile_create () {
    pDatafile result = malloc (sizeof(struct datafile));
    result->hdr_common_ = 0;
    result->hdr_location_ = 0;
    result->read_pos_ = 0;
    return result;
}

int datafile_init (pDatafile result, enum datafile_type df_type, size_t len, const char* data) {
    result->type_ = df_type;
    switch (df_type) {
        case DFT_COMMON: {
            freeCommonHeader (result->hdr_common_);
	        result->hdr_common_ = 0;
            pHeaderCommon hdr = malloc (sizeof(struct header_common));
            hdr->data_len_ = len;
            hdr->data_ = data;
			result->read_pos_ = hdr->data_;
            hdr->start_year_ = readShort(result);
            hdr->start_month_ = readUnsignedByte(result);
            hdr->start_day_ = readUnsignedByte(result);
            hdr->custom_data_len_ = readUTF(result, hdr->custom_data_);
            hdr->day_count_ = readShort(result);

            hdr->data_len_ = len - (result->read_pos_ - hdr->data_);
            hdr->data_ = malloc (hdr->data_len_);
            memcpy (hdr->data_, result->read_pos_, hdr->data_len_);
            
            result->hdr_common_ = hdr; }
            break;
        case DFT_LOCATION: {
            freeLocationHeader (result->hdr_location_);
	        result->hdr_location_ = 0;
            pHeaderLocation hdr = malloc (sizeof(struct header_location));
            hdr->data_len_ = len;
            hdr->data_ = data;
			result->read_pos_ = hdr->data_;
            int signature = readInt(result);
            if (signature != 0x53265741) {
                free (hdr);
                return 1;
            }
            char version = readByte(result);
            if (version != 2) {
                free (hdr);
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
            hdr->transitionTimes_ = malloc (hdr->transitionCount_ * sizeof(long));
            hdr->transitionOffsets_ = malloc (hdr->transitionCount_ * sizeof(long));
            hdr->transitionNames_ = malloc (hdr->transitionCount_ * sizeof(char*));
            int i = 0;
            for (; i < hdr->transitionCount_; ++i) {
                hdr->transitionTimes_[i] = readInt(result); // start_date
                hdr->transitionOffsets_[i] = readShort(result); // gmt_ofs_min
                hdr->transitionOffsets_[i] *= 60;
                hdr->transitionNames_[i] = malloc (TRANSITION_NAME_LEN);
                readUTF(result, hdr->transitionNames_[i]); // name
            }

            hdr->data_len_ = len - (result->read_pos_ - hdr->data_);
            hdr->data_ = malloc (hdr->data_len_);
            memcpy (hdr->data_, result->read_pos_, hdr->data_len_);

            result->hdr_location_ = hdr; }
            break;
    }
	return 0;
}

void datafile_fini (pDatafile df) {
    if (df) {
        freeCommonHeader(df->hdr_common_);
        df->hdr_common_ = 0;
        freeLocationHeader(df->hdr_location_);
		df->hdr_location_ = 0;
        free (df);
    }
}
