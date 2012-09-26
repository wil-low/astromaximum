/* 
 * File:   datafile.h
 * Author: willow
 *
 * Created on 26 вересня 2012, 0:26
 */

#ifndef DATAFILE_H
#define	DATAFILE_H

#include <stdio.h>

#define DATAFILE_EXPORT __attribute__ ((visibility ("default")))

#ifdef	__cplusplus
extern "C" {
#endif

enum datafile_type {
    DFT_COMMON = 0,
    DFT_LOCATION = 1,
};

struct datafile_event {
    long date0_, date1_;
    short degree_;
    char planet0_, planet1_;
};
typedef struct datafile_event* pEvent;

struct header_common {
    int start_year_;
    char start_month_;
    char start_day_;
    size_t custom_data_len_;
    char custom_data_[10];
    size_t day_count_;
};
typedef struct header_common* pHeaderCommon;

struct header_location {
    int start_year_;
    char start_month_;
    char start_day_;
    size_t day_count_;
    int city_id_;
    int coords[3];
    char city_[50];
    char state_[50];
    char country_[50];
    char timezone_[50];
    size_t custom_data_len_;
    char custom_data_[10];
    size_t transitionCount_;
    long* transitionTimes_;
    long* transitionOffsets_;
    char** transitionNames_;
};
typedef struct header_location* pHeaderLocation;

struct datafile {
    size_t data_len_;
    unsigned char* data_;
    unsigned char* read_pos_;
    enum datafile_type type_;
    pHeaderCommon hdr_common_;
    pHeaderLocation hdr_location_;
};
typedef struct datafile* pDatafile;

DATAFILE_EXPORT
pDatafile datafile_init (enum datafile_type df_type, size_t len, const char* data);

DATAFILE_EXPORT
void datafile_fini (pDatafile df);

#ifdef	__cplusplus
}
#endif

#endif	/* DATAFILE_H */

