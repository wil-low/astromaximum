//---------------------------------------------------------------------------

#ifndef evclassH
#define evclassH

#include "events.h"
using namespace std;

//---------------------------------------------------------------------------
class Event
{
public:
  long date[2];
  unsigned char planetId[2];
  unsigned short degree;
  double julianDay;
  static double startJD;
  static int startYear;
  static double calcJD(long date);
  Event(double jd, unsigned char planet);
  static long packDate(double date);
  int getDayNumber();
  void dump();
private:
};
#endif
