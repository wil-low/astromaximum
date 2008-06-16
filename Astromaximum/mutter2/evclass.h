//---------------------------------------------------------------------------

#ifndef evclassH
#define evclassH

#include "events.h"

//---------------------------------------------------------------------------
class Event {
public:
    long date[2];
    unsigned char planetId[2];
    unsigned short degree;
    double julianDay;
    static double startJD;
#ifdef ANSITZ
    static long _timezone_;
#endif
    static double EPOCH;
    static int startYear;
    static double calcJD(long date);
    void print_date(int i);
    Event(double jd, unsigned char planet);
    static long packDate(double date);
    int getDayNumber();
    void dump();
    void dump2();
    char *date_sql(char *str, int i);
private:
};
#endif
