import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;
import java.io.*;
import java.util.*;

//#define perftest="0"
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreNotOpenException;

/**
 * Main class of application. Provides useful routines and runs all other
 * objects.
 *
 * @author willow
 */
public class Astromaximum extends MIDlet implements CommandListener {

    /**
     * Background color (gray) of {@link Summary}
     */
    static final int BACK_COLOR = 0xb0b0b0;
    /**
     * Color of selection box
     */
    static final int SELECTION_COLOR = 0xffffff;
    /**
     * Color of selected topic
     */
    static final int SEA_COLOR = 0x009bd5;
    /**
     * Color of topic box
     */
    static final int BLUE_COLOR = 0x006ff4;
    /**
     * Dimmed color for {@link Event#EV_MOON_MOVE moon move} boundaries,
     * {@link Event#EV_NIGHT_HOURS night hours}, days outside current week
     * and month
     */
    static final int DIMMED_COLOR = 0x909090;
    /**
     * Dark red color for "red mark" - current time selector
     */
    static final int RUBY_COLOR = 0xb00000;
    /**
     * Bright red color for "red mark" - current time selector
     */
    static final int RED_COLOR = 0xf00000;
    /**
     * Light background for month page
     */
    static final int CURRENT_MONTH_COLOR = 0xc0c0c0;
    /**
     * {@link SummItem} border color
     */
    static final int BORDER_COLOR = 0xa0a0a0;
    /**
     * Highlighted {@link SummItem} that has interpretation on selected topic
     */
    static final int TOPIC_COLOR = 0xd0d0d0;
    /**
     * {@link Event#EV_MONTH_GRID} color
     */
    static final int GRAY_COLOR = 0xe0e0e0;
    /**
     * Suggested URL for main web site
     */
    static final String URL = "mobi.astromaximum.com";
    /**
     * Delay (ms) between showing log texts, if logging is on
     * and user pressed "Enter" to bypass it quickly
     */
    static int LOGGER_SLEEP = 2000;
    /**
     * Persistent variable for use of rendering classes
     */
    static Display disp;    //#if "imeiCheck" @ protection
    /**
     * Secret variable for IMEI protection. Available with "imeiCheck" ability
     */
    static int hj;
    /**
     * Attempt to figure out possible bugs in live system; holds constant values
     * in order to find the place where an Exception occured; errCode is shown
     * then in {@link LogBox} window
     */
    static int errCode = 0;
    /**
     * Application-wide current year of calendar
     */
    static int startYear = 0;
    /**
     * GMT calendar instance for Date/time calculations
     */
    static Calendar calendar;
    /**
     * Flag indicating that midlet is started for the first time
     */
    private static boolean firstRun = true;
    /**
     * Flag indicating that # key is pressed
     */
    static boolean poundPressed = false;
    static private final Hashtable locHash = new Hashtable();
    /**
     * Midlet instance reference
     */
    static Astromaximum instance;
    /**
     * Milliseconds in day - constant
     */
    static final long MSECINDAY = 86400 * 1000;
    /**
     * Month names (localized)
     */
    static final String[] months = new String[12];
    private static final String[] shortMonths = new String[12];
    /**
     * Zodiac constellations abbreviated
     */
    static final String[] CONSTELL = {
        "Ari", "Tau", "Gem", "Cnc", "Leo", "Vir", "Lib", "Sco", "Sgr", "Cap", "Aqu", "Psc"
    };
    /**
     * Planet names abbreviated
     */
    static final String[] PLANETS = {
        "SO", "MO", "ME", "VE", "MA", "JU", "SA", "UR", "NE", "PL", "KN", "BM", "WM"
    };
    /**
     * Blue color for "blue mark" - custom time selector
     */
    static final int CUST_COLOR = 0xf0;

    /**
     * Get midnight time (00:00) for the day specified
     *
     * @param time any date
     * @return midnight date
     */
    static long getMidnight(long time) {
        calendar.setTime(new Date(time + Event.localOffset(time)));
        calendar.set(Calendar.HOUR_OF_DAY, 0);
        calendar.set(Calendar.MINUTE, 0);
        calendar.set(Calendar.SECOND, 0);
        return calendar.getTime().getTime();
    }
    static Options options;
    static Summary summary;
    static DataFile dataFile;
    static LogBox logBox;
    static Interpreter interpreter;
    static CustomTime customTime;
    static List menu;
    
    private static long start;
    /**
     * Not exactly locale, just language indentifier
     */
    static String locale = null;

    /**
     * Start application, initialize all subsystems
     */
    public void startApp() {
        logBox = new LogBox();
        errCode = 1; // XXX
        calendar = Calendar.getInstance(TimeZone.getTimeZone("GMT"));//TimeZone.getDefault());
//#ifdef freetest
//# 		System.out.println(System.getProperty("microedition.io.file.FileConnection.version"));
//#endif
        disp = Display.getDisplay(this);
//#debug
        start = System.currentTimeMillis();
        try {
            instance = this;
//      InputStream iis=getClass().getResourceAsStream("/Amaxdata.dat");
//      try {
//        System.out.println(iis.available());
//      } catch (IOException ex) {
//        ex.printStackTrace();
//      }

//#if "timeBomb" @ protection
//#mdebug debug
     System.out.println(Integer.toHexString(Interpreter.hj));
      System.out.println(Integer.toHexString(CustomTime.hj));
//#enddebug
//#endif
//#if logger
      long beforeLS=Runtime.getRuntime().freeMemory();
//#endif
//        LocalizationSupport.initLocalizationSupport(locale);
//#if logger
      long afterLS=Runtime.getRuntime().freeMemory();
//#endif
            interpreter = new Interpreter();
            interpreter.recreateCommands();
            locale = getstr(255);
            errCode = 101; // XXX

            logBox.init();
//#if logger
      interpreter.isLogged=true;
      disp.setCurrent(interpreter);
      logger("Total heap="+Long.toString(Runtime.getRuntime().totalMemory())+
          "|before LocSupport="+Long.toString(beforeLS)+
          "|after LocSupport="+Long.toString(afterLS));
//#endif
            summary = new Summary();
//#if logger
      logger(summary.toString());
//#endif
//#if perftest=="0"
            errCode = 2; // XXX
            summary.setMoonXY(summary.getWidth() >> 1, summary.getHeight() >> 1,
                    Graphics.HCENTER | Graphics.VCENTER);
//#if logger
      logger("before summary run");
//#endif
            errCode = 3; // XXX
            summary.run();
            //#ifndef logger
//#             disp.setCurrent(summary);
            //#endif
//#endif
//#if logger
      logger("after summary run");
//#endif
            for (int i = 0; i < 12; i++) {
                months[i] = getstr(7 + i);
                shortMonths[i] = months[i].substring(0, 3);
            }

            errCode = 4; // XXX
//#if logger
      logger(logBox.toString());
//#endif
            //    sizer.setSize(logBox.getWidth(), logBox.getHeight());
            log("TZ id=" + TimeZone.getDefault().getID());
            options = new Options();
            errCode = 41; // XXX
            dataFile = new DataFile();
//#if logger
      logger("dataFile");
//#endif
            errCode = 5; // XXX
            try{
                options.initDB(true);
                init2();
            }
            catch(Exception ex){
                ex.printStackTrace();
                logBox.askResetDB();
            }
        } catch (Exception oome) {
///#mdebug debug
            Astromaximum.log("errCode=" + Integer.toString(errCode));
            Astromaximum.log(oome.toString());
            logBox.showLog(null);
            oome.printStackTrace();
//        quit();
///#enddebug
        }
    }
    
    void init2(){            
        try {
            int len = dataFile.geoposData.length;
            Astromaximum.log("geopos len=" + new Integer(len).toString());
//#if logger
      logger("initDB");
//#endif
            errCode = 7; // XXX
            customTime = new CustomTime();
//#if logger
      logger("customTime");
//#endif
//        System.out.println("Modem="+customTime.askModem());
//#debug error
            errCode = 8; // XXX
            Astromaximum.log("Total memory = " + Long.toString(Runtime.getRuntime().totalMemory()));
            options.loadHistory();
            options.init();
            errCode = 9; // XXX
//      System.gc();
//        dataFile.fillCache();
//        log("Options");
//          System.out.println(Runtime.getRuntime().freeMemory());
//#debug error
            System.out.println("Interpreter");
//        log("customTime");
//          System.out.println(Runtime.getRuntime().freeMemory());
//#if perftest == "2"
//#         calendar.set(Calendar.YEAR,2007);
//#         calendar.set(Calendar.MONTH,Calendar.FEBRUARY);
//#         calendar.set(Calendar.DAY_OF_MONTH,1);
//#         calendar.set(Calendar.HOUR_OF_DAY,0);
//#         calendar.set(Calendar.MINUTE,0);
//#         calendar.set(Calendar.SECOND,0);
//#         long tm=calendar.getTime().getTime();
//#         SummItem si = new SummItem(Event.EV_MOON_SIGN_LARGE);
//#         si.events=new Event[1];
//#         si.events[0]=new Event(tm,Event.SE_MOON);
//#         si.events[0].date1=tm+3600*1000;
//#         Interpreter instance = new Interpreter();
//#         Alert alert=new Alert("Test","",null, AlertType.INFO);
//#         alert.setTimeout(Alert.FOREVER);
//#         disp.setCurrent(alert);
//#         long tick=System.currentTimeMillis();
//#         for(int h=0; h<11; h++){
//#           alert.setString(Integer.toString(h));
//#           Astromaximum.log(Integer.toString(h));
//#           instance.topic=h;
//#           for(int i=0; i<12; i++){
//#             si.events[0].degree=(short)i;
//#             boolean result = instance.findText(si);
//#           }
//#         }
//#         Astromaximum.log("Total time="+Long.toString(System.currentTimeMillis()-tick));
//#         logBox.showLog(null);
//#endif
//#if perftest == "1"
//#         calendar.set(Calendar.YEAR,2007);
//#         calendar.set(Calendar.MONTH,Calendar.FEBRUARY);
//#         calendar.set(Calendar.DAY_OF_MONTH,1);
//#         calendar.set(Calendar.HOUR_OF_DAY,0);
//#         calendar.set(Calendar.MINUTE,0);
//#         calendar.set(Calendar.SECOND,0);
//#         summary.moonPhase= Astromaximum.dataFile.getEvents(Event.EV_MOON_PHASE,Event.SE_MOON,0,-1);
//#         dataFile.getEvents(Event.EV_NAVROZ,Event.SE_SUN, 0, dataFile.finalJD).copyInto(summary.aNavroz);
//# //        Vector v= Astromaximum.dataFile.getEvents(Event.EV_TITHI,1,calendar.getTime().getTime(),0);
//# //        System.out.println("Mercury");
//# //        evDump(v);
//#         summary.changeSize();
//# //        summary.setCell(getMidnight(),true);
//# //        summary.showDaySummary();
//#         summary.setCell(calendar.getTime().getTime(),true);
//#         long tm[]=new long[31];
//#         Alert alert=new Alert("Test","",null, AlertType.INFO);
//#         alert.setTimeout(Alert.FOREVER);
//#         disp.setCurrent(alert);
//#
//#         for(int i=0; i<31; i++){
//#           tm[i]=summary.changeDay(1);
//#           alert.setString(Integer.toString(i));
//#         }
//#         long min=tm[0], sum=0, max=tm[0];
//#         for(int i=0; i<31; i++){
//#           long tt=tm[i];
//#           if(tt<min){
//#             min=tt;
//#           }
//#           if(tt>max){
//#             max=tt;
//#           }
//#         }
//#         for(int i=0; i<31; i++){
//#           long tt=tm[i];
//#           if(tt!=min || tt!=max){
//#             sum+=tt;
//#           }
//#         }
//#         Astromaximum.log("Min="+Long.toString(min));
//#         Astromaximum.log("Avg="+Long.toString(sum/31));
//#         Astromaximum.log("Max="+Long.toString(max));
//#         logBox.showLog(null);
//#endif
//#if perftest=="0"

            errCode = 10; // XXX
            int cnt = Astromaximum.dataFile.getEvents(Event.EV_MOON_PHASE, Event.SE_MOON,
                    dataFile.startJD, dataFile.finalJD);
            Summary.aMoonPhase = new Event[cnt];
            System.arraycopy(DataFile.events, 0, Summary.aMoonPhase, 0, cnt);
            Summary.moonPhaseCount = cnt;
//#if logger
      logger("moonPhase");
//#endif
            errCode = 11; // XXX
            cnt = dataFile.getEvents(Event.EV_NAVROZ, Event.SE_SUN, 0, dataFile.finalJD);
//        evDump(nav);
            if (cnt != 2) {
                errCode = 12; // XXX
                throw new Exception("Navroz event count != 2");
            }
            System.arraycopy(DataFile.events, 0, summary.aNavroz, 0, cnt);
//#if logger
      logger("Navroz");
//#endif
            errCode = 13; // XXX
            Summary.size = Options.optLayout;
            summary.changeSize();
//#if logger
      logger("changeSize");
//#endif
            errCode = 14; // XXX
            summary.setCell(getMidnight(summary.selDate.getTime()), true);
//#if logger
      logger("setCell");
//#endif
//      evDump(dataFile.getEvents(Event.EV_RISE, Event.SE_MOON, 0, dataFile.finalJD));
            errCode = 15; // XXX
            summary.setToday();
//#if logger
      logger("showDaySummary");
//#endif
            errCode = 16; // XXX
            summary.stop();
//#if logger
      if(interpreter.isLogged){
        Thread.sleep(3000);
        interpreter.isLogged=false;
      }
      disp.setCurrent(summary);
//#endif
            errCode = 17; // XXX
            firstRun = false;
//#endif
        } catch (Exception oome) {
///#mdebug debug
            Astromaximum.log("errCode=" + Integer.toString(errCode));
            Astromaximum.log(oome.toString());
            logBox.showLog(null);
            oome.printStackTrace();
//        quit();
///#enddebug
        }
        summary.repaint();
//#mdebug
        System.out.print("Initialization took ");
        System.out.print(System.currentTimeMillis() - start);
        System.out.println(" msec.");
//#enddebug

         summary.startRealtime();
    }

    public void pauseApp() {
//        summary.stopRealtime();
    }

    /**
     * Clean up any resources
     *
     * @param unconditional API standard
     */
    public void destroyApp(boolean unconditional) {
        options.shutdown();
        firstRun = true;
        dataFile = null;
        summary = null;
        interpreter = null;
        customTime = null;
        logBox = null;
        options = null;
    }

    /**
     * Stop the MIDlet, saving {@link Options}
     */
    static void quit() {
        String isApplet = System.getProperty("microemu.applet");
        if (isApplet != null && isApplet.equals("true"))
            return;
        Display.getDisplay(instance).setCurrent(null);
//#ifdef use_amtext
//#         try {
//#             interpreter.rs.closeRecordStore();
//#         } catch (RecordStoreException ex) {}
//#endif
        instance.destroyApp(true);
        instance.notifyDestroyed();
    }

    /**
     * Send any string into {@link LogBox}, remove extra lines (>30)
     *
     * @param string String
     */
    static void log(String string) {
///#mdebug debug
        System.out.println(string);
        if (logBox.size() > 0 && logBox.getString(0).equals(LogBox.EMPTY)) {
            logBox.delete(0);
        }
        logBox.append(string, null);
        while (logBox.size() > 30) {
            logBox.delete(0);
        }
///#enddebug
    }

    /**
     * Convert from absolute degree to its zodiac sign
     * @param absDegree int
     * @return sign number
     */
    static final int getSignDegree(int absDegree) {
        return absDegree % 30 + 1;
    }

    /**
     * Get element from {@link Event} vector (shortcut)
     * @param v vector
     * @param idx index
     * @return Event
     */
    static final Event evAt(Vector v, int idx) {
        return (Event) v.elementAt(idx);
    }

    /**
     * Change date by specified amount of days
     * @param date1 Date
     * @param delta amount of days
     * @return New time or 0 unless date is available
     */
    long changeDate(Date date1, int delta) {
        long tmp = date1.getTime();
        tmp += MSECINDAY * delta;
        return (dataFile.isDateAvailable(tmp)) ? tmp : 0;
    }

    /**
     * Extract PNG from res/ pack
     * @param index image index
     * @param string pack filename
     * @return created PNG image
     */
    static Image extractImg(int index, String string) {
        Image res = null;
        try {
            final DataInputStream dis = new DataInputStream(
                    instance.getClass().getResourceAsStream(string));
            int off = 0;
            final int all = dis.readUnsignedShort();
            for (int i = 0; i < index; i++) {
                off += dis.readShort();
            }
            final int len = dis.readShort();
            dis.skip(2 * (all - index - 1) + off);
            byte[] pngdata = new byte[len + 1];
            dis.read(pngdata);
            dis.close();
            res = Image.createImage(pngdata, 0, len);
        } catch (IOException ex) {
//      ex.printStackTrace();
        }
        return res;
    }
    //#mdebug info
    /**
     * Show events in array to STDOUT (utility)
     * @param events array
     */
    static void evDump(Event[] events) {
        System.out.println("**Array dump**");
        for (int i = 0; i < events.length; i++) {
            if (events[i] != null) {
                events[i].dump();
            } else {
                System.out.println("null");
            }
        }
    }

    /**
     * Show events in vector to STDOUT (utility)
     * @param events vector
     */
    static void evDump(Vector events) {
        System.out.print("Dump events= ");
        System.out.println(events.size());
        for (Enumeration e = events.elements(); e.hasMoreElements();) {
            ((Event) e.nextElement()).dump();
        }
    }
//#enddebug
    /**
     * Process switching to {@link Summary} and exiting
     * @param c
     * @param d
     */
    public void commandAction(Command c, Displayable d) {
        switch (c.getCommandType()) {
            case Command.STOP:
                disp.setCurrent(summary);
                break;
            case Command.OK:
                quit();
            case Command.BACK:
                disp.setCurrent(options);
        }
    }

    /**
     * Show alert inside Interpreter
     */
    public void alert(String str) {
        interpreter.prepareText();
        interpreter.txt = str;
        disp.setCurrent(interpreter);
    }

    /**
     * Get localized string from messages.txt
     * @param id string index
     * @return String
     */
    public static String getstr(int id) {
        Integer key = new Integer(id);
        if (locHash.containsKey(key)) {
            return (String) locHash.get(key);
        }
        String str = interpreter.extractArticle(new long[]{Event.EV_MSG, 0, id});
        if (str == null) {
            System.out.println("?? " + Integer.toString(id) + " ??");
            System.exit(1);
        } else {
            locHash.put(key, str);
        }
        return str;
    }

    static String localizedDateString(Date date) {
        calendar.setTime(date);
        final int weekDay = calendar.get(Calendar.DAY_OF_WEEK);
        StringBuffer sb=new StringBuffer(Astromaximum.getstr(weekDay - 1 + 20) + " ");
        if (!locale.equals("Ru")) {
            sb.append(Astromaximum.shortMonths[calendar.get(Calendar.MONTH)]).
                    append(" ").append(calendar.get(Calendar.DAY_OF_MONTH)).
                    append(" '").append(Integer.toString(calendar.get(Calendar.YEAR)).substring(2, 4));
        } else {
            sb.append(calendar.get(Calendar.DAY_OF_MONTH)).append(" ").
                    append(Astromaximum.shortMonths[calendar.get(Calendar.MONTH)]).append(" '").
                    append(Integer.toString(calendar.get(Calendar.YEAR)).substring(2, 4));
        }
        return sb.toString();
    }

    void showMenu(CommandListener listener, Command[] cmds) {
        menu=new List(Astromaximum.getstr(161), List.IMPLICIT);
        int cmdCount = cmds.length;
        for (int i = 0; i < cmdCount; i++) {
            if (cmds[i] != null)
                menu.append(cmds[i].getLabel(), null);
        }
        Command ok = new Command(Astromaximum.getstr(98), Command.OK, 0);
        menu.addCommand(ok);
        menu.addCommand(new Command(Astromaximum.getstr(97), Command.CANCEL, 4));
        menu.setSelectCommand(ok);
        menu.setCommandListener(listener);
        Astromaximum.disp.setCurrent(menu);
    }

    void showAbout() {
        String msg = "Astromaximum " + Integer.toString(startYear) + " " +
                getstr(162) + "||Web: http://" + URL;
        String tick = getAppProperty("X-Midlet-Version");
        if (tick != null) {
            tick = getstr(163) + " " + tick;
        }
        tick += " " + getstr(255);
        String rev = getAppProperty("X-Hg-Revision");
        if (rev != null) {
            tick += ", r" + rev;
        }
        msg += "||" + tick;
//#ifdef demo
//#         msg += " " + getstr(154);
//#endif
        msg += "||Copyright 2007, S&W Axis|" + getstr(153);
        msg += "||" + getstr(156);
        alert(msg);
    }

//#if "timeBomb" @ protection
    /**
     * Secret routine to check time bomb range
     * @return garbage array
     */
    static byte[] getArray() {
//        System.out.println(Options.optFlags);
        int now = (int) (Options.currentTime() / 4096);
//#mdebug
     Astromaximum.log("Timebomb:");
     Astromaximum.log(Long.toString(GeoList.tzOffset));
     Astromaximum.log(new Date((long)Interpreter.hj*4096).toString());
     Astromaximum.log(new Date((long)now*4096).toString());
     Astromaximum.log(new Date((long)CustomTime.hj*4096).toString());
//#enddebug
        int mul = (CustomTime.hj - now) * (now - Interpreter.hj);
//    System.out.println(mul);
        if (mul == 0) {
            mul = 1;
        }
        mul /= Math.abs(mul);
//    System.out.println(Integer.toHexString(now));
        return new byte[mul];
    }
//#endif

//#if logger
    /**
     * Show strings and free memory when logging is enabled
     * @param s string to show
     */
    void logger(String s){
    if(interpreter.isLogged){
      interpreter.txt+=(s+" ("+Long.toString(Runtime.getRuntime().freeMemory())+")|");
      interpreter.repaint();
      interpreter.serviceRepaints();
      try {
        Thread.sleep(LOGGER_SLEEP);
      }
      catch (InterruptedException ex) {
//#debug debug
        ex.printStackTrace();
      }
    }
  }
//#endif
}// # vi:et:ts=4:sw=4

