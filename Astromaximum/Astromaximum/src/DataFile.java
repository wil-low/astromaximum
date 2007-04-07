import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.util.Calendar;
import java.util.Date;
import java.util.Enumeration;
import java.util.Vector;
//#define optRead
/**
 * <p>Title: Astromaximum</p>
 * 
 * <p>Description: </p>
 * 
 * <p>Copyright: Copyright (c) 2006</p>
 * 
 * <p>Company: Wiland Inc.</p>
 * 
 * 
 * @author Andrei Ivushkin
 * @version 1.0
 * @noinspection CastToConcreteClass
 */
final class DataFile{
  private static final int EF_DATE=0x1; // contains 2nd date - 4b
  private static final int EF_PLANET1=0x2; // contains 1nd planet - 1b
  private static final int EF_PLANET2=0x4; // contains 2nd planet - 1b
  private static final int EF_DEGREE=0x8; // contains degree or angle - 2b
  private static final int EF_CUMUL_DATE_B=0x10; // date are cumulative from 1st 4b - 1b
  private static final int EF_CUMUL_DATE_W=0x20; // date are cumulative from 1st 4b - 2b
  private static final int EF_SHORT_DEGREE=0x40; // contains angle 0..180 - 1b
  private static final int EF_NEXT_DATE2=0x80; // 2nd date is 1st in next event
//#if "imeiCheck" @ protection
  static int hj=-2000;
//#endif
//  static final int NDF_CACHE = 1;
//  static final int NDF_COMMON = 2;
//  static final int NDF_GEOPOS = 3;
  long startJD, finalJD;
  int dayCount;
//  private final Vector cache=new Vector();
  byte[] commonData;
  byte[] geoposData;
  static Vector ids=new Vector();
  
//  private int curRec=-1;
  Vector eclipses=null;
  /**
   * DataFile
   *
   *
   */
  DataFile() {
    final Calendar cal=Astromaximum.calendar;
    final DataInputStream is;
    try {
      //#if _Year == "2006"
//#     is=new DataInputStream(getClass().getResourceAsStream("/common2006.dat"));
//#endif
      is= new DataInputStream(getClass().getResourceAsStream("/common.dat"));
      Astromaximum.startYear=is.readShort();
      cal.set(Calendar.YEAR,Astromaximum.startYear);
      cal.set(Calendar.MONTH, is.readUnsignedByte()-1);
      cal.set(Calendar.DAY_OF_MONTH, is.readUnsignedByte());
      cal.set(Calendar.HOUR_OF_DAY, is.readUnsignedByte());
      cal.set(Calendar.MINUTE, is.readUnsignedByte());
      cal.set(Calendar.SECOND,0);
      startJD=cal.getTime().getTime();
//      System.out.println(Event.long2String(startJD,false,false));
      dayCount= is.readShort();
      finalJD=startJD+dayCount*Astromaximum.MSECINDAY;
//#debug info 
      System.out.println(dayCount);
      commonData=new byte[is.available()];
//#debug debug 
      System.out.println(is.available());
      is.read(commonData);
      is.close();
    } 
    catch (IOException e){
    }
    
    eclipses=getEvents(Event.EV_ECLIPSE,Event.SE_SUN,startJD,finalJD);
    final Vector tmp=getEvents(Event.EV_ECLIPSE,Event.SE_MOON,startJD,finalJD);
    for(int i=0; i<tmp.size(); i++) {
      eclipses.addElement(tmp.elementAt(i));
    }
//#debug error
    System.out.println(Runtime.getRuntime().freeMemory());
  }

//  void fillCache(){
//    cacheData(Event.EV_RISE,Event.SE_SUN);
//    cacheData(Event.EV_SET,Event.SE_SUN);
//    cacheData(Event.EV_RISE,Event.SE_MOON);
//    cacheData(Event.EV_SET,Event.SE_MOON);
//    cacheData(Event.EV_RISE,Event.SE_MERCURY);
//    cacheData(Event.EV_SET,Event.SE_MERCURY);
//    cacheData(Event.EV_RISE,Event.SE_VENUS);
//    cacheData(Event.EV_SET,Event.SE_VENUS);
//    cacheData(Event.EV_RISE,Event.SE_MARS);
//    cacheData(Event.EV_SET,Event.SE_MARS);
//    cacheData(Event.EV_RISE,Event.SE_JUPITER);
//    cacheData(Event.EV_SET,Event.SE_JUPITER);
//    cacheData(Event.EV_RISE,Event.SE_SATURN);
//    cacheData(Event.EV_SET,Event.SE_SATURN);
////    cacheData(Event.EV_ASP_EXACT,Event.SE_SUN);
//  }

//  /**
//   * readShort
//   *
//   * @param is DataInputStream
//   * @return short
//   * @throws IOException
//   * @noinspection MagicNumber
//   */
//  private short readShort(DataInputStream is) throws IOException {
//    return (short)(is.readByte() & 0xff | (is.readByte() & 0xff) << 8);
//  }
  
  
  /**
   * readSubData
   *
   * @param evtype int
   * @param planet int
   * @return Vector
   * @noinspection MagicNumber
   */
//#ifndef optRead
//#   Vector readSubData(byte[] buf, int evtype, int planet, boolean isCommon, long dayStart, long dayEnd) {
//#     final Vector v=new Vector();
//#     int flag;
//#     int skipOff;
//#     try {
//#       final DataInputStream is=new DataInputStream(new ByteArrayInputStream(buf));
//#       if(Astromaximum.options!=null){
//#         Astromaximum.options.addImeiChar();
//#       }
//#       while(true){
//#         int ch=is.readUnsignedByte();
//#         while (evtype != is.readUnsignedByte()) {
//#           if(isCommon && Astromaximum.options!=null){
//#             Astromaximum.options.addImeiChar(Integer.toString(ch).charAt(0));
//#           }
//#           skipOff = is.readShort()-3;
//#           is.skip(skipOff);
//#           ch=is.readUnsignedByte();
//#         }
//#         skipOff=is.readShort();
//#         flag=is.readShort();
//#         if(planet == is.readByte()){
//# //          Astromaximum.instance.log("Found!",false);
//#           break;
//#         } 
//#         else {
//#           is.skip(skipOff - 6);
//#         }
//#       }
//#       final int count=is.readShort();
//#       int fcumul_date_b=(flag & EF_CUMUL_DATE_B);
//#       int fcumul_date_w=(flag & EF_CUMUL_DATE_W);
//#       int fdate=(flag & EF_DATE);
//#       int fplanet1=(flag & EF_PLANET1);
//#       int fplanet2=(flag & EF_PLANET2);
//#       int fdegree=(flag & EF_DEGREE);
//#       int fshort_degree=(flag & EF_SHORT_DEGREE);
//#       int fnext_date2=(flag & EF_NEXT_DATE2);
//#       
//#       int skips=0;
//# //      if(evtype==Event.EV_TITHI){
//# //        dayStart-=Astromaximum.MSECINDAY;
//# //        dayEnd+=Astromaximum.MSECINDAY;
//# //      }
//#       if(fdate!=0){
//#         skips+=4;
//#       }
//#       if(fplanet1!=0){
//#         ++skips;
//#       }
//#       if(fplanet2!=0){
//#         ++skips;
//#       }
//#       if(fdegree!=0){
//#         ++skips;
//#         if (fshort_degree==0) {
//#           ++skips;
//#         }
//#       }
//#       int cumul; long date=0, date1;
//#       long sJD=startJD, fJD=finalJD;
//# //      Astromaximum.instance.log("Count="+Integer.toString(count),true);
//#       for(int i=0; i<count; i++){
//# //////////////
//#         if(fcumul_date_b!=0){
//#           if(i!=0){
//#             byte d=is.readByte();
//#             cumul=d;
//#             date+=(cumul+24*60)*60;
//#           }
//#           else{
//#             date=is.readInt();
//#           }
//#         }
//#         else if(fcumul_date_w!=0){
//#           if(i!=0){
//#             short d=is.readShort();
//#             cumul=d;
//#             date+=(cumul+24*60)*60;
//#           }
//#           else{
//#             date=is.readInt();
//#           }
//#         }
//#         else{
//#             date=is.readInt();
//#         }
//#         
//#if "imeiCheck" @ protection
//#         long mydate=(date*1000)+hj;
//#else        
//#         long mydate=date*1000;
//#endif
//#         if(fdate!=0) {
//#if "imeiCheck" @ protection
//#           date1= ((long)is.readInt() *1000)+hj;
//#else   
//# //          date=;
//#           date1=  ((long)is.readInt() * 1000);
//#endif
//#         }
//#         else{
//#           date1=mydate;
//#         }
//#     final int f0= Event.dateBetween(mydate,dayStart,dayEnd);
//#     final int f1= Event.dateBetween(date1,dayStart,dayEnd);
//# /*    dump();
//#     System.out.print(f0);
//#     System.out.print("&");
//#     System.out.println(f1);
//#  */
//#define optread22
//#     
//#ifdef optread
//#     if(Math.abs(f0 + f1) != 2) {
//#endif
//#         
//# //        if(mydate>=dayStart){
//#         final Event ev=new Event(mydate,planet);
//#         ev.date1=date1;
//#         if(fplanet1!=0) {
//#           ev.planet0= is.readByte();
//#         }
//#         if(fplanet2!=0) {
//#           ev.planet1= is.readByte();
//#         }
//#         if(fdegree!=0) {
//#           if (fshort_degree!=0) {
//#             ev.setDegree(is.readUnsignedByte());
//#           } 
//#           else {
//#             ev.setDegree(is.readShort());
//#           }
//#         }
//#         if(fnext_date2!=0) {
//#           if (v.size() > 0) {
//#             final Event last = (Event) v.lastElement();
//#             last.date1=mydate;
//# //            lastE.date1=mydate;
//#           }
//#         }
//#         if(evtype==Event.EV_RETROGRADE){
//#           if(mydate<sJD){
//#             ev.date0=sJD;
//#           }
//#           else if(mydate>fJD){
//#             ev.date1=fJD;
//#           }
//#         }
//# //        ev.dump();
//#         v.addElement(ev);
//#         }
//#ifdef optread
//#         else{
//#           if(v.size()>0){
//#             break;
//#           }
//#           is.skip(skips);
//#         }
//#       }
//#endif
//#     } catch (IOException ex) {
//#     }
//#     
//#     return v;
//#   }
//#else  
  Vector readSubData(byte[] buf, int evtype, int planet, boolean isCommon, long dayStart, long dayEnd) {
    final Vector v=new Vector();
    int flag;
    int skipOff;
    try {
      final DataInputStream is=new DataInputStream(new ByteArrayInputStream(buf));
      if(Astromaximum.options!=null){
        Astromaximum.options.addImeiChar();
      }
      while(true){
        int ch=is.readUnsignedByte();
        while (evtype != is.readUnsignedByte()) {
          if(isCommon && Astromaximum.options!=null){
            Astromaximum.options.addImeiChar(Integer.toString(ch).charAt(0));
          }
          skipOff = is.readShort()-3;
          is.skip(skipOff);
          ch=is.readUnsignedByte();
        }
        skipOff=is.readShort();
        flag=is.readShort();
        if(planet == is.readByte()){
//          Astromaximum.instance.log("Found!",false);
          break;
        } 
        else {
          is.skip(skipOff - 6);
        }
      }
      final int count=is.readShort();
      int fcumul_date_b=(flag & EF_CUMUL_DATE_B);
      int fcumul_date_w=(flag & EF_CUMUL_DATE_W);
      int fdate=(flag & EF_DATE);
      int fplanet1=(flag & EF_PLANET1);
      int fplanet2=(flag & EF_PLANET2);
      int fdegree=(flag & EF_DEGREE);
      int fshort_degree=(flag & EF_SHORT_DEGREE);
      int fnext_date2=(flag & EF_NEXT_DATE2);
      
      byte myplanet0=(byte)planet, myplanet1=-1;
      int mydgr=127;
      long mydate0,mydate1;
      Event last=new Event(0,0);
      int skips=0;
      if(fdate!=0){
        skips+=4;
      }
      if(fplanet1!=0){
        ++skips;
      }
      if(fplanet2!=0){
        ++skips;
      }
      if(fdegree!=0){
        ++skips;
        if (fshort_degree==0) {
          ++skips;
        }
      }
      int cumul; long date=0;
      long sJD=startJD, fJD=finalJD;
//      Astromaximum.instance.log("Count="+Integer.toString(count),true);
      for(int i=0; i<count; i++){
//////////////
        if(fcumul_date_b!=0){
          if(i!=0){
            byte d=is.readByte();
            cumul=d;
            date+=(cumul+24*60)*60;
          }
          else{
            date=is.readInt();
          }
        }
        else if(fcumul_date_w!=0){
          if(i!=0){
            short d=is.readShort();
            cumul=d;
            date+=(cumul+24*60)*60;
          }
          else{
            date=is.readInt();
          }
        }
        else{
            date=is.readInt();
        }
        
//#if "imeiCheck" @ protection
        mydate0=(date*1000)+hj;
//#else        
//#         mydate0=date*1000;
//#endif
        if(fdate!=0) {
//#if "imeiCheck" @ protection
          mydate1= ((long)is.readInt() *1000)+hj;
//#else   
//#           mydate1=  ((long)is.readInt() * 1000);
//#endif
        }
        else{
          mydate1=mydate0;
        }

    
        
//        if(mydate>=dayStart){
        if(fplanet1!=0) {
          myplanet0= is.readByte();
        }
        if(fplanet2!=0) {
          myplanet1= is.readByte();
        }
        if(fdegree!=0) {
          if (fshort_degree!=0) {
            mydgr=is.readUnsignedByte();
          } 
          else {
            mydgr=is.readShort();
          }
        }
        if(fnext_date2!=0) {
          last.date1=mydate0;
//            lastE.date1=mydate;
        }
        if(evtype==Event.EV_RETROGRADE){
          if(mydate0<sJD){
            mydate0=sJD;
          }
          else if(mydate1>fJD){
            mydate1=fJD;
          }
        }

        if(last.isInPeriod(dayStart,dayEnd,false)) {
//          v.addElement(last);
          v.addElement(new Event(last));
        }
        else{
          if(v.size()>0){
            break;
          }
//          is.skip(skips);
        }
        last.planet0=myplanet0;
        last.planet1=myplanet1;
        last.degree=(short)mydgr;
        last.date0=mydate0;
        last.date1=mydate1;
      }
      if(last.isInPeriod(dayStart,dayEnd,false)) {
        v.addElement(last);
//        v.addElement(new Event(last));
      }
    } 
    catch (IOException ex) {
    }
    return v;
  }
//#endif

  /**
   * getEventsOnDay
   *
   * @param v Vector
   * @param evtype int
   * @param planet int
   * @param dayStart long
   * @param dayEnd long
   * @return int
   * @param special
   * @param value
   */
  void getEventsOnPeriod(Vector v, int evtype, int planet, boolean special,
      long dayStart, long dayEnd, int value) {
    boolean flag=false;
    final Vector tmp=getEvents(evtype,planet, dayStart, dayEnd);
    for (Enumeration e = tmp.elements() ; e.hasMoreElements() ;) {
      final Event ev=(Event)e.nextElement();
      if(ev.isInPeriod(dayStart,dayEnd,special)){
        flag=true;
        if(value > 0) {
          ev.setDegree(value);
        }
        v.addElement(ev);
      } 
      else
        if(flag) {
          break;
        }
    }
  }
  
 
  
  void getAspectsOnPeriod(Vector v, int planet, long dayStart, long dayEnd) {
    boolean flag=false;
    final Vector tmp=getEvents(Event.EV_ASP_EXACT, planet == Event.SE_MOON ? Event.SE_MOON : -1, dayStart, dayEnd);
    for (Enumeration e = tmp.elements() ; e.hasMoreElements() ;) {
      final Event ev=(Event)e.nextElement();
      if(planet == -1 || ev.planet0 == planet || ev.planet1 == planet){
        if(ev.isDateBetween(0,dayStart,dayEnd)){
//          ev.dump();
          flag=true;
          v.addElement(ev);
        }
      } 
      else
        if(flag) {
          break;
        }
    }
  }
  /**
   * getEvents
   *
   * @param evtype int
   * @param planet int
   * @return Vector
   */
  Vector getEvents(int evtype, int planet, long dayStart, long dayEnd) {
//    for (Enumeration e = cache.elements() ; e.hasMoreElements() ;) {
//      final EventCache ev = (EventCache) e.nextElement();
//      if(ev.planet == planet && ev.eventType == evtype){
//        return ev.events;
//      }
//    }
    switch(evtype){
      case Event.EV_ASTRORISE:
      case Event.EV_ASTROSET:
      case Event.EV_RISE:
      case Event.EV_SET:
      case Event.EV_NAVROZ:
        return readSubData(geoposData,evtype, planet,false,dayStart,dayEnd);
      default:
        return readSubData(commonData,evtype, planet,true,dayStart,dayEnd);
//        return readSubData(commonData,evtype, planet,true,dayStart,dayEnd);
    }
  }
  
  Event getEventOnPeriod(int evtype, int planet, boolean special, long dayStart, long dayEnd) {
    final Vector tmp=getEvents(evtype,planet,dayStart,dayEnd);
//    final Vector tmp=getEvents(evtype,planet,startJD,finalJD);
    for (Enumeration e = tmp.elements() ; e.hasMoreElements() ;) {
      final Event ev=(Event)e.nextElement();
      if(ev.isInPeriod(dayStart,dayEnd,special)){
//        if(evtype==Event.EV_DEGREE_PASS)
//          ev.dump();
        return ev;
      }
    }
    return null;
  }

  /*
  private void cacheData(int event, int planet) {
    final Vector v = readSubData(event, planet);
    if (v.size() > 0){
      cache.addElement(new EventCache(v, event, planet));
//      System.out.println("Cashed "+Integer.toString(v.size())+" events for "+
//          Integer.toString(event)+"/"+Integer.toString(planet));
    }
//    else
//      System.out.println("Cache not found for "+Integer.toString(event)+"/"+
//          Integer.toString(planet));
  }
  */
// --Commented out by Inspection START (1/12/07 1:44 PM):
//  int getDayCount() {
//    return dayCount;
//  }
// --Commented out by Inspection STOP (1/12/07 1:44 PM)
  
  /** @noinspection AccessStaticViaInstance
   * @return
   * @param date*/
  boolean isDateAvailable(long date) {
    return Event.dateBetween(date,startJD,startJD+dayCount*Astromaximum.MSECINDAY)==0;
//    return true;
  }
  
  boolean isDateAvailable(Date date) {
    return isDateAvailable(date.getTime());
  }
  
 
  /**
   * 
   * @param today 
   * @return 
   */
  Event todayEclipse(long today, int delta) {
//    today+=Event.localOffset(today);
    final long today_end=today+Astromaximum.MSECINDAY*(delta+1);
    today-=Astromaximum.MSECINDAY*delta;
    for(Enumeration e=eclipses.elements(); e.hasMoreElements();){
      final Event ecl=(Event)e.nextElement();
      if(ecl.isDateBetween(0,today,today_end)) {
        return ecl;
      }
    }
    return null;
  }
  
//  public void cacheData(int event, int planet) {
//    Vector v =null;// readSubData(geoposData, event, planet);
//    if (v.size() > 0){
//      cache.addElement(new EventCache(v, event, planet));
////#debug info 
//      System.out.println("Cached "+Integer.toString(v.size()));
//    }
//  }
  
//  private final class EventCache {
//    final Vector events;
//    final int eventType;
//    private final int planet;
//    EventCache(Vector ev, int evtype, int plt) {
//      events=ev;
//      eventType=evtype;
//      planet=plt;
//    }
//  }
}
