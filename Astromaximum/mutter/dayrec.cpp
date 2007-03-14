//---------------------------------------------------------------------------

#pragma hdrstop

#include "dayrec.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


DayRecord::DayRecord(unsigned int num)
{
  dayNumber=num;
}

unsigned int DayRecord::length()
{
  int sz=2;
  for(int i=0; i<events.size(); i++)
    sz+=events[i]->length();
  return sz;
}

DayRecord::~DayRecord()
{
}

void DayRecord::addEvent(EventClass * ev)
{
  events.push_back(ev);
}
