/**
 * <p>Title: Nomad</p>
 *
 * <p>Description: </p>
 *
 * <p>Copyright: Copyright (c) 2006</p>
 *
 * <p>Company: Wiland Inc.</p>
 *
 * @author Andrei Ivushkin
 * @version 1.0
 */
import java.util.*;

final class Event {
    static final byte SE_SUN = 0;
    static final byte SE_MOON = 1;
    static final byte SE_MERCURY = 2;
    static final byte SE_VENUS = 3;
    static final byte SE_MARS = 4;
    static final byte SE_JUPITER = 5;
    static final byte SE_SATURN = 6;
    static final byte SE_URANUS = 7;
    static final byte SE_NEPTUNE = 8;
    static final byte SE_PLUTO = 9;
    static final byte SE_TRUE_NODE = 10;
    static final byte SE_MEAN_APOG = 11;
    static final byte SE_WHITE_MOON = 12;
    static final int EV_VOC = 0; // void of course
    static final int EV_SIGN_ENTER = 1; // enter into sign
    static final int EV_ASP_EXACT = 2; // exact aspect
    static final int EV_RISE = 3;  // rising & setting
    static final int EV_DEGREE_PASS = 4;  // entering degree
    static final int EV_VIA_COMBUSTA = 5;  // good & bad degrees
    static final int EV_RETROGRADE = 6;
    static final int EV_ECLIPSE = 7;
    static final int EV_TITHI = 8;
    static final int EV_NAKSHATRA = 9;
    static final int EV_SET = 10;  // rising & setting
    static final int EV_DECL_EXACT = 11;  // declination
    static final int EV_NAVROZ = 12;  // Navroz
    static final int EV_TOP_DAY = 13;  // week days
    static final int EV_PLANET_HOUR = 14;  // planetary hours
    static final int EV_STATUS = 15;
    static final int EV_SUN_RISE = 16;
    static final int EV_MOON_RISE = 17;
    static final int EV_MOON_MOVE = 18;
    static final int EV_SEL_DEGREES = 19;
    static final int EV_DAY_HOURS = 20;
    static final int EV_NIGHT_HOURS = 21;
    static final int EV_SUN_DAY = 22;
    static final int EV_MOON_DAY = 23;
    static final int EV_TOP_MONTH = 24;
    static final int EV_MOON_PHASE = 25;
    static final int EV_ZODIAC_SIGN = 26;
    static final int EV_PANEL = 27;
    static final int EV_TOPIC_BUTTON = 28;
    static final int EV_DEG_2ND = 29; // degrees on second page
    static final int EV_WEEK_GRID = 30;
    static final int EV_MONTH_GRID = 31;
    static final int EV_DECUMBITURE = 32;
    static final int EV_DECUMB_ASPECT = 33;
    static final int EV_DECUMB_BEGIN = 34;
    static final int EV_SUN_DEGREE_LARGE = 35;
    static final int EV_MOON_SIGN_LARGE = 36;
    static final int EV_HELP = 37;
    static final int EV_ASP_EXACT_MOON = 38;
    static final int EV_DEGPASS0 = 39;
    static final int EV_DEGPASS1 = 40;
    static final int EV_DEGPASS2 = 41;
    static final int EV_DEGPASS3 = 42;
    static final int EV_HELP0 = 43;
    static final int EV_HELP1 = 44;
    static final int EV_ASTRORISE = 45;
    static final int EV_ASTROSET = 46;
    static final int EV_APHETICS = 47;
    static final int EV_FAST = 48;
    static final int EV_ASCAPHETICS = 49;
    static final int EV_MSG = 50;
    static final int EV_BACK = 51;
    static final int EV_LAST = 52;  // last - do not use

    // Any changes above must be synched with %eventType in tools.pm
    // and EventType in mutter2/events.h !!!

    //#if "imeiCheck" @ protection
    static int hj;
//#endif
    byte planet0, planet1 = -1;
    long date0, date1;
    short degree = 127;
//#if microemu
//#     static final String locations[] = {
//#         "LOCATIONS_DAT0",
//#         "LOCATIONS_DAT1",
//#         "LOCATIONS_DAT2",
//#         "LOCATIONS_DAT3",
//#         "LOCATIONS_DAT4",
//#     };
//#endif

    /**
     * @param dat
     * @param planet
     * @noinspection NestedAssignment
     */
    Event(long dat, int planet) {
        planet0 = (byte) planet;
        date0 = date1 = dat;
    }

    /**
     * getDateString
     *
     * @param index     byte[]
     * @param hoursOnly boolean
     * @return String
     */
    String getDateString(int index, int hoursOnly) {
        return long2String((index > 0) ? date1 : date0, hoursOnly, hoursOnly > 0 && index == 1);
    }

    /**
     * to2String
     *
     * @param value int
     * @return String
     */
    static String to2String(int value) {
        String str = Integer.toString(value);
        if (str.length() == 1) {
            str = "0" + str;
        }
        return str;
    }

    /**
     * getDegree
     *
     * @return int
     */
    int getDegree() {
        return degree & 0x3ff;
    }

    int getDegType() {
        return (degree >>> 14) & 0x3;
    }

    /**
     * getJD
     *
     * @return long
     */
    /*  long getJD() {
    return julianDay;
    }*/
    /**
     * Event
     *
     * @param event Event
     */
    Event(Event event) {
        planet0 = event.planet0;
        planet1 = event.planet1;
        date0 = event.date0;
        date1 = event.date1;
        degree = event.degree;
//    julianDay=event.julianDay;
    }

    /**
     * isInPeriod
     *
     * @param start   byte[]
     * @param end     byte[]
     * @param special
     * @return boolean
     */
    boolean isInPeriod(long start, long end, boolean special) {
        if (date0 == 0) {
            return false;
        }
        final int f = dateBetween(date0, start, end) + dateBetween(date1, start, end);
        /*    dump();
        System.out.print(f0);
        System.out.print("&");
        System.out.println(f1);
         */
        if (f == 2 || f == -2) {
            return false;
        }
        if (special) {
            if (f == -1) {
                return false;
            }
//      if(f0*f1==-1)
//        tmp.setDate(0,0);
        }
        return true;
    }

    //#mdebug info
    /**
     * dump
     *
     * @noinspection UseOfSystemOutOrSystemErr
     */
    void dump() {
//        System.out.print(new Date(date0) + " - " + new Date(date1));
        System.out.print (getDateString(0, 0) + " - " + getDateString(1, 0));
        final int dgr = getDegree();
        final int goodbad = getDegType();
        System.out.print("  degree=" + Integer.toString(dgr));
        if (goodbad != 0) {
            System.out.print(" " + (goodbad == 2 ? "good" : "bad"));
        }
        System.out.print("  planets " + Integer.toString(planet0) + " - " +
                Integer.toString(planet1) + "\n");
    }
//#enddebug
    /**
     * @param date0
     * @param start
     * @param end
     * @return
     */
    static int dateBetween(long date0, long start, long end) {
        if (date0 < start) {
            return -1;
        }
        if (date0 >= end) {
            return 1;
        }
        return 0;
    }

    boolean isDateBetween(int index, long start, long end) {
        long dat = (index > 0) ? date1 : date0;
        return start <= dat && dat < end;
    }

    /**
     * @param date0
     * @param hoursOnly
     * @param h24
     * @return
     * @noinspection AssignmentToMethodParameter,HardcodedFileSeparator
     */
    static String long2String(long date0, int hoursOnly, boolean h24) {
        /* @todo TZ code! */
        if (hoursOnly == 1) {
            if (date0 < Summary.period0) {
                date0 = Summary.period0;//-localOffset(Summary.period0);
            }
            if (date0 > Summary.period1) {
                date0 = Summary.period1;//-localOffset(Summary.period1);
            }
        }
        date0 += localOffset(date0);
        Astromaximum.calendar.setTime(new Date(date0));
        final StringBuffer s = new StringBuffer();
        if (hoursOnly == 0) {
            if (!Astromaximum.locale.equals("Ru")) {
                s.append(to2String(Astromaximum.calendar.get(Calendar.MONTH) + 1)).
                        append("/").
                        append(to2String(Astromaximum.calendar.get(Calendar.DAY_OF_MONTH)));
            } else {
                s.append(to2String(Astromaximum.calendar.get(Calendar.DAY_OF_MONTH))).
                        append(".").
                        append(to2String(Astromaximum.calendar.get(Calendar.MONTH) + 1));
            }
            s.append(" ");
        }
        int hh = 0, mm = 0;
        try {
            hh = Astromaximum.calendar.get(Calendar.HOUR_OF_DAY);
            mm = Astromaximum.calendar.get(Calendar.MINUTE);
        }
        catch (Exception e) {
            System.out.println("Ex: long2String(" + Long.toString(date0) + ", "
                + Integer.toString(hoursOnly) +", " + (h24 ? "true" : "false"));
        }
        if (h24 && hh + mm == 0) {
            hh = 24;
        }
        s.append(to2String(hh)).
                append(":").append(to2String(mm));
//    if(!hoursOnly)
//      s.append("/");

//    s+=to2String(date0[index])+":"+to2String(date0[index]);
        return s.toString();//s;
    }

    static long localOffset(long date0) { // date0 is in UTC always ?
        long ofs = GeoList.tzOffset;//-GeoList.localOffset;
        if (Options.dstExists) {
            /*
            System.out.println(date0);
            System.out.println(GeoList.dstStart);
            System.out.println(GeoList.dstEnd);
            System.out.println(GeoList.isSouthern);
             */
            int inn = dateBetween(date0, GeoList.dstStart, GeoList.dstEnd);
            if ((inn == 0) ^ GeoList.isSouthern) {
                ofs += Astromaximum.MSECINDAY / 24;
            }
        }
        return ofs;
    }
}

// # vi:et:ts=4:sw=4
