//---------------------------------------------------------------------------

#pragma hdrstop
#include <time.h>

#include "evclass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
double Event::startJD=0;
int Event::startYear=0;

Event::Event(double jd, unsigned char planet)
{
  julianDay=jd;
  date[0]=date[1]=packDate(jd);
  planetId[0]=planetId[1]=planet;
  degree=0;
}

double Event::calcJD(long date)
{
  tm *st=gmtime(&date);
  double jd=swe_julday(st->tm_year+1900, st->tm_mon+1,st->tm_mday,st->tm_hour+
    st->tm_min/60.+st->tm_sec/3600.,SE_GREG_CAL);
  return jd;
}

int Event::getDayNumber()
{
  return (julianDay-startJD)/MINUTE_STEP;
}

int Event::packDate(double date)
{
  int y,m,d; double hour;
  swe_revjul(date, SE_GREG_CAL, &y, &m, &d, &hour);
  struct tm now;
  now.tm_year=y-1900;
  now.tm_mon=m-1;
  now.tm_mday=d;
  now.tm_hour=hour;
  now.tm_min=(hour-now.tm_hour)*60;
  now.tm_sec=0;
  now.tm_isdst = 0;
  return mktime(&now)-_timezone;
}


void Event::dump()
{
  tm *st=gmtime(&date[0]);
  printf("\n\n* %s - ", asctime(st));
  st=gmtime(&date[1]);
  printf("%s", asctime(st));
  int dgr=degree&0x3fff;
  int goodbad=degree>>14;
  printf("  degree=%d",dgr);
  if(goodbad)
   printf(" %s",goodbad==2? "good": "bad");
  printf("  planets %u - %u",planetId[0],planetId[1]);
}
