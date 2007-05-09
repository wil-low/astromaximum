//#ifdef build.desktop
//# package com.sw_axis;
//# import java.awt.Frame;
//# import java.awt.event.WindowAdapter;
//# import java.awt.event.WindowEvent;
//#else
import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;
//#endif
import java.io.*;
import java.util.*;
//#define perftest="0"
/** @noinspection CastToConcreteClass*/
//#ifdef build.desktop
//# public class Astromaximum extends Frame{
//#   public Astromaximum() {
//#     addWindowListener(new WindowAdapter() {
//#       public void windowClosing(WindowEvent evt) {
//#         exitForm(evt);
//#       }
//#     });
//#     setBounds(0,0,200,200);
//#     setTitle("Astromaximum Desktop");
//#   }
//#   private void exitForm(WindowEvent evt) {                          
//#     System.exit(0);
//#   }                         
//#     
//#else
public class Astromaximum extends MIDlet implements CommandListener{
  static final int BACK_COLOR = 0xb0b0b0;
  static final int SELECTION_COLOR = 0xffffff;
  static final int SEA_COLOR = 0x009bd5;
  static final int BLUE_COLOR = 0x006ff4;
  static final int DIMMED_COLOR = 0x909090;
  static final int RUBY_COLOR = 0xb00000;
  static final int RED_COLOR = 0xf00000;
  static final int CURRENT_MONTH_COLOR = 0xc0c0c0;
  static final int BORDER_COLOR = 0xa0a0a0;
  static final int TOPIC_COLOR = 0xd0d0d0;
  static final int GRAY_COLOR = 0xe0e0e0;
  static final String URL="www.astromaximum.com";
  static int LOGGER_SLEEP = 2000;

//#if "imeiCheck" @ protection
  static int hj;
//#endif
  
  static int startYear=0;
  static final Calendar calendar = Calendar.getInstance(TimeZone.getTimeZone("GMT"));//TimeZone.getDefault());
  static boolean firstRun=true;
  
  // midlet instance reference
  static Astromaximum instance;
  static final long MSECINDAY=86400*1000;
  private final String[] monthKeys={"January","February","March","April","May","June",
  "July","August","September","October","November","December"
  };
  static final String[] months=new String[12];
  static final String[] CONSTELL={
    "Ari","Tau","Gem","Cnc","Leo","Vir","Lib","Sco","Sgr","Cap","Aqu","Psc"
  };
  static final String[] PLANETS={
    "SO","MO","ME","VE","MA","JU","SA","UR","NE","PL","KN","BM","WM"
  };
  static final int CUST_COLOR=0xf0;
  
  /**
   * getToday
   *
   * @return Date
   */
  long getToday() {
    final Date dat=new Date();
    calendar.setTime(dat);
    calendar.set(Calendar.HOUR_OF_DAY,0);
    calendar.set(Calendar.MINUTE,0);
    calendar.set(Calendar.SECOND,0);
    return calendar.getTime().getTime();
  }
  private final String[] dowKeys={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
  static final String[] dow=new String[7];
  // displayable object
  static Options options;
  static Summary summary;
  static DataFile dataFile;
  static LogBox logBox;
  static Interpreter interpreter;
  static CustomTime customTime;
  
  /**
   * Start this MIDlet
   */
  public void startApp(){
    if(firstRun){
//        System.gc();
      try{
        instance = this;
//      InputStream iis=getClass().getResourceAsStream("/Amaxdata.dat");
//      try {
//        System.out.println(iis.available());
//      } catch (IOException ex) {
//        ex.printStackTrace();
//      }
      
//#if "timeBomb" @ protection
//#mdebug debug
//#      System.out.println(Integer.toHexString(Interpreter.hj));
//#       System.out.println(Integer.toHexString(CustomTime.hj));
//#enddebug
//#endif
//#if logger
//#       long beforeLS=Runtime.getRuntime().freeMemory();
//#endif      
        LocalizationSupport.initLocalizationSupport("ru_RU");
//#if logger
//#       long afterLS=Runtime.getRuntime().freeMemory();
//#endif      
        interpreter =new Interpreter();
//#if logger
//#       interpreter.isLogged=true;
//#       Display.getDisplay(this).setCurrent(interpreter);
//#       logger("Total heap="+Long.toString(Runtime.getRuntime().totalMemory())+
//#           "|before LocSupport="+Long.toString(beforeLS)+
//#           "|after LocSupport="+Long.toString(afterLS));
//#endif      
        summary =new Summary();
//#if logger
//#       logger(summary.toString());
//#endif        
//#if perftest=="0"
        summary.setMoonXY(summary.getWidth()>>1,summary.getHeight()>>1,
            Graphics.HCENTER|Graphics.VCENTER);
//#if logger
//#       logger("before summary run");
//#endif        
        summary.run();
  //#ifndef logger
        Display.getDisplay(this).setCurrent(summary);
  //#endif      
//#endif
//#if logger
//#       logger("after summary run");
//#endif        
        for(int i=0; i < 12; i++) {
          months[i] = LocalizationSupport.getMessage(monthKeys[i]);
        }
        for(int i=0; i < 7; i++) {
          dow[i] = LocalizationSupport.getMessage(dowKeys[i]);
        }
        
        logBox =new LogBox();
//#if logger
//#       logger(logBox.toString());
//#endif        
        //    sizer.setSize(logBox.getWidth(), logBox.getHeight());
        log("TZ id="+TimeZone.getDefault().getID());
        options = new Options();
        dataFile = new DataFile();
//#if logger
//#       logger("dataFile");
//#endif      
        try {
          options.initDB(true);
        } 
        catch (Exception ex) {
          options.resetStorage();
        }
//#if logger
//#       logger("initDB");
//#endif      
      customTime =new CustomTime();
//#if logger
//#       logger("customTime");
//#endif      
//        System.out.println("Modem="+customTime.askModem());
      Astromaximum.log("****Total memory = "+Long.toString(Runtime.getRuntime().totalMemory()));
      options.loadHistory();
      System.gc();
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
//#         Display.getDisplay(this).setCurrent(alert);
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
//# //        summary.setCell(getToday(),true);
//# //        summary.showDaySummary();
//#         summary.setCell(calendar.getTime().getTime(),true);
//#         long tm[]=new long[31];
//#         Alert alert=new Alert("Test","",null, AlertType.INFO);
//#         alert.setTimeout(Alert.FOREVER);
//#         Display.getDisplay(this).setCurrent(alert);
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
      if(true/*dataFile.isDateAvailable(summary.selDate)*/){
//        log("SDS before");
        summary.moonPhase= Astromaximum.dataFile.getEvents(Event.EV_MOON_PHASE,Event.SE_MOON,
            dataFile.startJD,dataFile.finalJD);
//#if logger
//#       logger("moonPhase");
//#endif      
//        Vector nav=dataFile.getEvents(Event.EV_NAVROZ,Event.SE_SUN, 1, dataFile.finalJD);
//        evDump(nav);
//        nav.copyInto(summary.aNavroz);
        dataFile.getEvents(Event.EV_NAVROZ,Event.SE_SUN, 1, dataFile.finalJD).copyInto(summary.aNavroz);
//#if logger
//#       logger("Navroz");
//#endif      
      summary.changeSize();
//#if logger
//#       logger("changeSize");
//#endif      
        summary.setCell(getToday(),true);
//#if logger
//#       logger("setCell");
//#endif      
        summary.showDaySummary();
//#if logger
//#       logger("showDaySummary");
//#endif      
      summary.stop();
//#if logger
//#       if(interpreter.isLogged){
//#         Thread.currentThread().sleep(3000);
//#         interpreter.isLogged=false;
//#       }
//#       Display.getDisplay(this).setCurrent(summary);
//#endif      
      } 
      else{
        //#if Demo
        //#       calendar.set(Calendar.YEAR,2006);
        //#       calendar.set(Calendar.MONTH,Calendar.DECEMBER);
        //#       calendar.set(Calendar.DAY_OF_MONTH,14);
        //#       grid.selDate=calendar.getTime();
        //#       showDaySummary(grid.selDate);
        //#else
        final Alert noDate=new Alert("Today: "+ summary.selDate.toString(),
            "Year containing current date is not present in database.\nYou can install it"+
            " or select another time range.",null,AlertType.WARNING);
        noDate.addCommand(new Command("Install",Command.SCREEN,1));
        noDate.addCommand(new Command("Avail.",Command.BACK,1));
        noDate.setCommandListener(this);
        Display.getDisplay(this).setCurrent(noDate);
        //#endif
      }
      firstRun =false;
//#endif
    
    } 
    catch(Exception oome){
//#mdebug debug
       Astromaximum.log("****Total memory = "+Long.toString(Runtime.getRuntime().totalMemory()));
       Astromaximum.log(oome.toString());
       logBox.showLog(null);
       oome.printStackTrace();
//        quit();
//#enddebug       
    }
    }  
    summary.repaint();
    summary.startRealtime();
//    log("SDS after");
//      Display.getDisplay(this).setCurrent(summary);
  }
  
  public void pauseApp() {
    /**@todo Implement pauseApp behavior here*/
  }
  
  /**
   * Clean up any resources
   *
   * @param unconditional boolean
   */
  public void destroyApp(boolean unconditional) {
    dataFile=null;
    summary=null;
    interpreter=null;
    customTime=null;
    logBox=null;
    options=null;
  }
  
  /**
   * Stop the MIDlet
   */
  static void quit() {
    Display.getDisplay(instance).setCurrent(null);
    options.shutdown();
    instance.destroyApp(true);
    instance.notifyDestroyed();
  }
  
  
  /**
   * log
   *
   * @param string String
   */
  static void log(String string) {
///#mdebug debug    
     if(Astromaximum.logBox.getString(0).equals(LogBox.EMPTY)) {
       Astromaximum.logBox.delete(0);
     }
     Astromaximum.logBox.append(string,null);
     while(Astromaximum.logBox.size() > 30) {
       Astromaximum.logBox.delete(0);
     }
     System.out.println(string);
///#enddebug
  }
  
  
  static int getSignDegree(int absDegree) {
    return absDegree%30+1;
  }
  
  
  static Event evAt(Vector v, int idx) {
    return (Event) v.elementAt(idx);
  }
  
  /** @noinspection BooleanMethodNameMustStartWithQuestion*/
  long changeDate(Date date1, int delta) {
    long tmp=date1.getTime();
    tmp += MSECINDAY *delta;
    return (dataFile.isDateAvailable(tmp))? tmp: 0;
  }
  
  static Image extractImg(int index, String string) {
    Image res=null;
    try {
      final DataInputStream dis=new DataInputStream(
          instance.getClass().getResourceAsStream(string));
      int off=0;
      final int all=dis.readUnsignedShort();
      for(int i=0; i<index; i++) {
        off += dis.readShort();
      }
      final int len=dis.readShort();
      dis.skip(2*(all-index-1)+off);
      byte[] pngdata=new byte[len+1];
      dis.read(pngdata);
      dis.close();
      res=Image.createImage(pngdata,0,len);
      pngdata=null;
    } catch (IOException ex) {
//      ex.printStackTrace();
    }
    return res;
  }
  

  
//#mdebug info
  /** @noinspection StaticMethodOnlyUsedInOneClass*/
  static void evDump(Event[] events) {
    System.out.println("**Array dump**");
    for(int i=0; i<events.length; i++) {
      if (events[i] != null) {
        events[i].dump();
      } else {
        System.out.println("null");
      }
    }
  }
  
  static void evDump(Vector events) {
    System.out.print("Dump events= ");
    System.out.println(events.size());
    for(Enumeration e=events.elements(); e.hasMoreElements();)
      ((Event)e.nextElement()).dump();
  }
//#enddebug

  public void commandAction(Command c, Displayable d) {
    switch(c.getCommandType()){
      case Command.SCREEN:
        break;
      case Command.OK:
        quit();
      case Command.BACK:
        Display.getDisplay(instance).setCurrent(options);
    }
  }

//#if "timeBomb" @ protection
//#   static byte[] getArray() {
//#debug
//#         System.out.println(Options.optFlags);
//#     int now=(int)(Options.currentTime()/4096);
//#      Astromaximum.log("Timebomb:");
//#      Astromaximum.log(Long.toString(GeoList.tzOffset));
//#  //    Astromaximum.log(Long.toString(GeoList.localOffset));
//#      Astromaximum.log(new Date((long)Interpreter.hj*4096).toString());
//#      Astromaximum.log(new Date((long)now*4096).toString());
//#      Astromaximum.log(new Date((long)CustomTime.hj*4096).toString());
//#     int mul=(CustomTime.hj-now)*(now-Interpreter.hj);
//# //    System.out.println(mul);
//#     if(mul==0){
//#       mul=1;
//#     }
//#     mul/=Math.abs(mul);
//# //    System.out.println(Integer.toHexString(now));
//#     return new byte[mul];
//#   }
//#endif  

//#if logger
//#   void logger(String s){
//#     if(interpreter.isLogged){
//#       interpreter.txt+=(s+" ("+Long.toString(Runtime.getRuntime().freeMemory())+")|");
//#       interpreter.repaint();
//#       interpreter.serviceRepaints();
//#       try {
//#         Thread.currentThread().sleep(LOGGER_SLEEP);
//#       } 
//#       catch (InterruptedException ex) {
//#debug debug
//#         ex.printStackTrace();
//#       }
//#     }
//#   }
//#endif
//#endif
}

