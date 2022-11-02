//---------------------------------------------------------------------------

#include <time.h>
#include <sstream>
#include <iomanip>
#include "evclass.h"
#include "events.h"

#define OLDCALC
//---------------------------------------------------------------------------
double Event::startJD = 0;
int Event::startYear = 0;
long Event::_timezone_ = 0;
double Event::EPOCH = 0;
static const double SECINDAY = 24 * 3600;

const char *month_name[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

Event::Event(double jd, unsigned char planet) {
    julianDay = jd;
    date[0] = date[1] = packDate(jd);
    planetId[0] = planetId[1] = planet;
    degree = 0;
}

double Event::calcJD(long date) {
    tm *st = gmtime(&date);
    double jd = swe_julday(st->tm_year + 1900, st->tm_mon + 1, st->tm_mday, st->tm_hour +
            st->tm_min / 60. + st->tm_sec / 3600., SE_GREG_CAL);
    return jd;
}

int Event::getDayNumber() {
    return (int) ((julianDay - startJD) / MINUTE_STEP);
}

long Event::packDate(double date) {
    int y, m, d;
    double hour;
    swe_revjul(date, SE_GREG_CAL, &y, &m, &d, &hour);
    struct tm now;
    now.tm_year = y - 1900;
    now.tm_mon = m - 1;
    now.tm_mday = d;
    now.tm_hour = (int) hour;
    now.tm_min = (int) ((hour - now.tm_hour) * 60 + 0.5);
    now.tm_sec = 0;
    now.tm_isdst = 0;
    return mktime(&now) - Event::_timezone_;
}

void Event::dump() {
    std::stringstream sstream;
    tm *st = gmtime(&date[0]);
    sstream << "* " << std::put_time(st, "%c") << " - ";
    st = gmtime(&date[1]);
    sstream << std::put_time(st, "%c");
    int dgr = degree & 0x3fff;
    int goodbad = degree >> 14;
    sstream << "  degree=" << dgr;
    if (goodbad)
        sstream << (goodbad == 2 ? "good" : "bad");
    sstream << "  planets " << (int)planetId[0] << " - " << (int)planetId[1];
    printf("%s\n", sstream.str().c_str());
}

void Event::dump2() {
    tm *st = gmtime(&date[0]);
    printf("\n* %s - ", asctime(st));
    st = gmtime(&date[1]);
    printf("%s", asctime(st));
    printf("   %ld - %ld\n", date[0], date[1]);
    printf("  degree=%X", degree);
    printf("  planets %u - %u", planetId[0], planetId[1]);
}

void Event::dump3() {
    printf("%ld - %ld", date[0], date[1]);
    printf("\tdegree=%X", degree);
    printf("  planets %u - %u", planetId[0], planetId[1]);
}

char *Event::date_sql(char *str, int i) {
    tm *st = gmtime(&date[i]);
    strftime(str, 200, "'%Y-%m-%d %H-%M-%S'", st);
    return str;
}

void Event::print_date(int i) {
#ifdef OLDCALC
    char str[200];
    tm *st = gmtime(&date[i]);
    strftime(str, 200, "'%Y-%m-%d %H:%M:%S'", st);
    printf("%s", str);
#else
    int y, mon, day, min;
    double hr;
    swe_revjul(date[i] / SECINDAY + EPOCH, SE_GREG_CAL, &y, &mon, &day, &hr);
    min = (int) ((hr - (int) hr)*60 + 0.5);
    printf("%04d-%02d-%02d %02d:%02d", y, mon, day, (int) hr, min);
#endif
}

char *Event::getInsertStr(char *s, int type) {
    char buf0[100], buf1[100];
    sprintf(s, "%d, %s, %s, %d, %d, %d", type,
        date_sql(buf0, 0), date_sql(buf1, 1), planetId[0], planetId[1], degree);
    return s;
}
// # vi:et:ts=4:sw=4
