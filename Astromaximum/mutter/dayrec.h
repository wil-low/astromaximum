//---------------------------------------------------------------------------

#ifndef dayrecH
#define dayrecH
#include "evclass.h"
#include <iostream>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
typedef vector <EventClass*> VAE;

class DayRecord
{
private:
  unsigned int dayNumber;
  VAE events;
//  TwoDateEvent riseSets[3];
//  vector<AspExactEvent> aspExact[PLANET_COUNT];
//  vector<GoodBadDegEvent> goodBadDeg;
  friend ostream & operator << (ostream& stream, DayRecord& dr)
  {
    int sz=dr.events.size();
    stream.write((const char*)&sz,2);
    for(int i=0; i<sz; i++)
      dr.events[i]->write2(stream);
    return stream;
  }
public:
  DayRecord(unsigned int num);
  unsigned int length();
  ~DayRecord();
  void addEvent(EventClass * ev);
};
#endif
