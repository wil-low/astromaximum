#include <stdio.h>
#include <time.h>
#include <assert.h>
// datafile.h
typedef enum{
    EF_DATE=0x1, // contains 2nd date - 4b
    EF_PLANET1=0x2, // contains 1nd planet - 1b
    EF_PLANET2=0x4, // contains 2nd planet - 1b
    EF_DEGREE=0x8, // contains degree or angle - 2b
    EF_CUMUL_DATE_B=0x10, // date are cumulative from 1st 4b - 1b
    EF_CUMUL_DATE_W=0x20, // date are cumulative from 1st 4b - 2b
    EF_SHORT_DEGREE=0x40, // contains angle 0..180 - 1b
    EF_NEXT_DATE2=0x80 // 2nd date is 1st in next event
} EventFlag;


long dstStart=0, dstEnd=0, tzOffset=0;
int isSouthern=0, dstExists=0;

short swapShort(short var) {
    var=(var & 0xff)<<8 | ((var >> 8) & 0xff);
    return var;
}

int swapInt(int var) {
    int res=0, i=0;
    for(i=0; i<4; i++){
        res<<=8;
        res|=(var & 0xff);
        var>>=8;
    }
    return res;
}

int readInt(FILE* fn){
    int res=0;
    fread(&res, sizeof(res), 1, fn);
    return swapInt(res);
}

int readShort(FILE* fn){
    short res=0;
    fread(&res, sizeof(res), 1, fn);
    return swapShort(res);
}

int readUnsignedByte(FILE* fn){
    unsigned char res=0;
    fread(&res, sizeof(res), 1, fn);
    return res;
}

int readByte(FILE* fn){
    char res=0;
    fread(&res, sizeof(res), 1, fn);
    return res;
}

struct Event_ {
    long date0, date1;
};

int dateBetween(long date0, long start, long end) {
    if (date0 < start) {
        return -1;
    }
    if (date0 >= end) {
        return 1;
    }
    return 0;
}

int isInPeriod(const struct Event_ *e, long start, long end) {
    if (!e->date0) {
        return 0;
    }
    return dateBetween(e->date0, start, end) == 0;
}
    
int printLocalTime(time_t date, int isSunrise){
    int dst=0;
    date+=tzOffset;
    if (dstExists) {
        int inn = dateBetween(date, dstStart, dstEnd);
        if (!inn ^ isSouthern) {
            date += 60*60;
            dst++;
        }
    }
    struct tm risetime;
    if(isSunrise)
        gmtime_r(&date, &risetime);
    else
    localtime_r(&date, &risetime);
    
    printf("%04d-%02d-%02d %02d:%02d\n", 
        risetime.tm_year+1900, risetime.tm_mon+1, risetime.tm_mday,
        risetime.tm_hour, risetime.tm_min);
    return dst;
}

int main(int argc, char** argv) { // data filename
    if(argc==1){
        printf("Usage: sunrise <filename> [YYYY-MM-DD HH:MM]\n");
        printf("Extracts today's sunrise from Astromaximum location file\n");
        return 0;
    }
    FILE *fn=fopen(argv[1], "rb");
    if(!fn)
        return -1;
    unsigned int my_year=0, my_month=0, my_day=0, my_hour=0, my_min=0;
    if(argc>=3){
        if(sscanf(argv[2], "%04d-%02d-%02d", &my_year, &my_month, &my_day) != 3){
            return 1;
        }
        if(argc==4){
            if(sscanf(argv[3], "%02d:%02d", &my_hour, &my_min) != 2){
                return 2;
            }
        }
    }
    int df_year=0;
    fread(&df_year, 2, 1, fn);
    fseek(fn, 2, SEEK_CUR);
    short cd_len=readShort(fn); // customData length
    fseek(fn, 2, SEEK_CUR);
    short len=readShort(fn); // cityName length
    fseek(fn, len, SEEK_CUR);
    tzOffset=readShort(fn);
    dstExists = (tzOffset & (1 << 15))==0;
    tzOffset &= (1 << 15) - 1;
    tzOffset -= 16 * 60;
    tzOffset *= 60;
    long d_1, d_2;
    if (dstExists) {
        d_1 = readInt(fn) * 60 - tzOffset;
        d_2 = readInt(fn) * 60 - tzOffset - 3600;
        if(d_1<d_2){ // N hemisphere
            dstStart=d_1; dstEnd=d_2;
        }
        else{
            dstStart=d_2; dstEnd=d_1; isSouthern=1;
        }
    }
    fseek(fn, cd_len, SEEK_CUR);
    struct Event_ last, result;
    result.date0=result.date1=0;
    int evtype=3; // EV_RISE
    int PERIOD = 24 * 60; int skipOff, flag;
    while (1) {
        fseek(fn, 1, SEEK_CUR);
        int rub = readUnsignedByte(fn);
        while (evtype != rub) {
            skipOff = readShort(fn) - 3;
            assert(skipOff>=0);
            fseek(fn, skipOff+1, SEEK_CUR);
            rub = readUnsignedByte(fn);
        }
        skipOff = readShort(fn);
        flag = readShort(fn);
        if (!readUnsignedByte(fn)) {
            break;
        } 
        else {
            fseek(fn, skipOff - 6, SEEK_CUR);
        }
    }
    int count = readShort(fn);
    int fcumul_date_b = (flag & EF_CUMUL_DATE_B);
    int fcumul_date_w = (flag & EF_CUMUL_DATE_W);
    int fdate = (flag & EF_DATE);
    int fplanet1 = (flag & EF_PLANET1);
    int fplanet2 = (flag & EF_PLANET2);
    int fdegree = (flag & EF_DEGREE);
    int fshort_degree = (flag & EF_SHORT_DEGREE);
    int fnext_date2 = (flag & EF_NEXT_DATE2);
    
    time_t dayStart, dayEnd, now; struct tm tm_;
    
    if(my_year && (df_year == my_year)){ // process entered time as current time
        memset(&tm_, 0, sizeof(tm_));
        tm_.tm_year   =   my_year-1900;
        tm_.tm_mon    =   my_month-1;
        tm_.tm_mday   =   my_day;
        tm_.tm_hour   =   my_hour;
        tm_.tm_min    =   my_min;
        dayStart=mktime(&tm_);
    }
    else{
        time(&dayStart);
    }
    gmtime_r(&dayStart, &tm_);
    tm_.tm_year=df_year-1900;
    dayStart=mktime(&tm_);
    now=dayStart;
    dayStart-=(tm_.tm_hour*3600 + tm_.tm_min*60 + tm_.tm_sec);
    dayEnd=dayStart+PERIOD*60;
    int planet=0; // SE_SUN

    char myplanet0 = planet, myplanet1 = -1;
    int mydgr = 127;
    long mydate0, mydate1;
/*
    int skips = 0;
    if (fdate) {
        skips += 4;
    }
    if (fplanet1) {
        ++skips;
    }
    if (fplanet2) {
        ++skips;
    }
    if (fdegree) {
        ++skips;
        if (!fshort_degree) {
            ++skips;
        }
    }
 */
    int cumul, i, res_count=0;
    long date = 0;
    for (i = 0; i < count; i++) {
        if (fcumul_date_b) {
            if (i) {
                cumul = readByte(fn);
                date += (cumul + PERIOD) * 60;
            } 
            else {
                date = readInt(fn);
            }
        } 
        else if (fcumul_date_w) {
            if (i) {
                cumul = readShort(fn);
                date += (cumul + PERIOD) * 60;
            } 
            else {
                date = readInt(fn);
            }
        } 
        else {
            date = readInt(fn);
        }
        mydate0=date;
        if (fdate) {
            mydate1=readInt(fn);
        } 
        else {
            mydate1 = mydate0;
        }
        if (fplanet1) {
            myplanet0 = readByte(fn);
        }
        if (fplanet2) {
            myplanet1 = readByte(fn);
        }
        if (fdegree) {
            if (fshort_degree) {
                mydgr = readUnsignedByte(fn);
            } 
            else {
                mydgr = readShort(fn);
            }
        }
        if (fnext_date2) {
            last.date1 = mydate0;
        }
//        printf("%d (%d, %d)\n", i, last.date0, last.date1);
        if (isInPeriod(&last, dayStart, dayEnd)) {
            res_count++;
                result=last;
                break;
        } 
/*        else{
            if(res_count){
                result=last;
                break;
            }
        }
 */
        last.date0 = mydate0;
        last.date1 = mydate1;
        if(feof(fn)) break;
    }
    if (isInPeriod(&last, dayStart, dayEnd)) {
            result=last;
    }
    fclose(fn);
    if(!result.date0){
        return -1;
    }
//    printLocalTime(dayStart);
    printLocalTime(result.date0, 1);
    int dst=printLocalTime(now, 0);
    float off=tzOffset/3600.;
    if(off==(int)off){
        printf("GMT %+d", (int)off);
    }
    else{
        printf("GMT %+0.1f", off);
    }
    printf(" %s\n", dst? "DST": "");
    
    
    return 0;
}



