import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Calendar;
import java.util.Date;
import javax.microedition.io.CommConnection;
import javax.microedition.io.Connector;
import javax.microedition.lcdui.*;
/*
 * CustomTime.java
 *
 * Created on 29 грудня 2006, 18:42
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
//#define timeHistory1
/**
 *
 * @author Administrator
 */
final class CustomTime extends Form implements CommandListener,ItemStateListener{
//  private final TextField hours;
//#if "timeBomb" @ protection
//#   static int hj=0x89abcdef;
//#endif
  int invoker=Event.EV_ASP_EXACT_MOON;
  final DateField dateField;

  final ChoiceGroup cg;
  static final int HIST_COUNT=5;
  static long[] history=new long[HIST_COUNT];
  static int histCount=0;
  private boolean showHistory;
  /** Creates a new instance of CustomTime */
  CustomTime() {
    super("");
    dateField=new DateField(LocalizationSupport.getMessage("Enter time:"),DateField.TIME,
        Astromaximum.calendar.getTimeZone());
    dateField.setDate(new Date());
    append(dateField);
    cg=new ChoiceGroup(LocalizationSupport.getMessage("History"), Choice.EXCLUSIVE);
    for(int i=0; i<histCount; i++){
      cg.append(Event.long2String(history[i],0,false),null);
    }
    addCommand(new Command("OK",Command.OK,1));
    addCommand(new Command(LocalizationSupport.getMessage("Cancel"),Command.CANCEL,1));
    setCommandListener(this);
    setItemStateListener(this);
  }

  public void itemStateChanged(Item item) {
//    if(item==cg){
//      System.out.println("hkjh");
//      long tm=history[cg.getSelectedIndex()];
//      tm-=Event.localOffset(tm);
//      dateField.setDate(new Date(tm));
//      setTimePrompt(invoker,0);
//    }
  }
  
  public void commandAction(Command c, Displayable d)  {
    if (c.getCommandType() == Command.CANCEL){
      Astromaximum.summary.dontRender();
    }
    else{
      Astromaximum.summary.isShowCustom=true;
//#if timeHistory
//#       setTime(showHistory);
//#else
      setTime(false);
//#endif
      switch(invoker){
        case Summary.PAGE_PANEL:
        case Summary.PAGE_DECUMB:
          Astromaximum.summary.calcDecumbiture();
          Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.summary);
          break;
        default:
          Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.summary);
//          Astromaximum.summary.dontRender();
      }
    }
  }
  
  void setTimePrompt(int pn, long tm) {
    invoker=pn;
    String tit, sDate;
    switch(pn){
      case Summary.PAGE_PANEL:
      case Summary.PAGE_DECUMB:
        tit=LocalizationSupport.getMessage("Disease_date");
        sDate=Event.long2String((Astromaximum.summary.period0+
          Astromaximum.summary.period0)>>1,0,false).substring(0,5);
        break;
      default:  
        tit=LocalizationSupport.getMessage("Date");

        sDate=Event.long2String((Astromaximum.summary.period0+
          Astromaximum.summary.period0)>>1,0,false).substring(0,5);
//        sDate=Event.long2String(tm,0,false);
    }
    setTitle(tit+" "+sDate);
  }

  void setTime(boolean addHistory) {
    Date dt=dateField.getDate();
    Astromaximum.calendar.setTime(dt);
    Astromaximum.summary.setCustomTime(
        Astromaximum.calendar.get(Calendar.HOUR_OF_DAY),Astromaximum.calendar.get(Calendar.MINUTE));
//#if timeHistory
//#     if(addHistory){
//#       for(int i=0; i<histCount; i++){
//#         if(history[i]==Astromaximum.summary.cusTime){
//#           cg.setSelectedIndex(i,true);
//#           return;
//#         }
//#       }
//#       for(int i=histCount-1; i>0; i--){
//#         history[i]=history[i-1];
//#       }
//#       history[0]=Astromaximum.summary.cusTime;
//#       if(histCount<HIST_COUNT){
//#         ++histCount;
//#       }
//#       cg.insert(0,Event.long2String(history[0],0,false),null);
//#       cg.setSelectedIndex(0,true);
//#       while(cg.size()>HIST_COUNT){
//#         cg.delete(cg.size()-1);
//#       }
//#       Astromaximum.options.saveHistory();
//#     }
//#endif
  }

  void init(int pn) {
    setTimePrompt(Astromaximum.summary.pageNum,dateField.getDate().getTime());
//#if timeHistory
//#     if(get(size()-1)==cg){
//#       delete(size()-1);
//#     }
//#     showHistory= pn==Summary.PAGE_PANEL || pn==Summary.PAGE_DECUMB;
//#     if(showHistory){
//#       append(cg);
//#     }
//#endif
    Display.getDisplay(Astromaximum.instance).setCurrent(this);
  }
  
  String askModem(){
     String port1;
     String ports = System.getProperty("microedition.commports");
     int comma = ports.indexOf(',');
     if (comma > 0) {
         // Parse the first port from the available ports list.
         port1 = ports.substring(0, comma);
     } else {
         // Only one serial port available.
         port1 =ports;
     }
    try {
      CommConnection cc = (CommConnection)
                 Connector.open("comm:COM3;baudrate=19200");
      int baudrate = cc.getBaudRate();
      InputStream is  = cc.openInputStream();
      OutputStream os = cc.openOutputStream();
      byte[] cmd=new String("AT+GMM").getBytes();
      os.write(cmd);
      os.write(0xa);
      os.write(0xd);
      int ch = 0;
      System.out.print("Available=");
      System.out.println(is.available());
//      while(true) {
//        ch = is.read();
//        System.out.println(ch);
//      }
      is.close();
      os.close();
      cc.close();
    } 
    catch (IOException ex) {
      ex.printStackTrace();
    }
     return port1;
  }
}
