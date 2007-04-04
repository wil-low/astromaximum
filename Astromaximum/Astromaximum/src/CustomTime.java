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
//#define timeHistory
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
  final DateField timeField;
  final DateField dateField;
  long decumbDate;
  int lockFlags;
  final ChoiceGroup cg;
  static final int HIST_COUNT=12;
  static long[] history=new long[HIST_COUNT];
  static int histCount=0;
  private boolean showHistory;
  /** Creates a new instance of CustomTime */
  CustomTime() {
    super("");
    timeField=new DateField(null/*LocalizationSupport.getMessage("Enter_time:")*/,DateField.TIME,
        Astromaximum.calendar.getTimeZone());
    dateField=new DateField(null,DateField.DATE,Astromaximum.calendar.getTimeZone());
    decumbDate=System.currentTimeMillis();
    dateField.setDate(new Date(decumbDate+Event.localOffset(decumbDate)));
    timeField.setDate(new Date(decumbDate+Event.localOffset(decumbDate)));
    cg=new ChoiceGroup(null/*LocalizationSupport.getMessage("History")*/, Choice.EXCLUSIVE);
    for(int i=0; i<histCount; i++){
      cg.append(Event.long2String(history[i],0,false),null);
    }
    addCommand(new Command("OK",Command.ITEM,2));
    addCommand(new Command(LocalizationSupport.getMessage("Lock/Unlock"),Command.ITEM,3));
    addCommand(new Command(LocalizationSupport.getMessage("Cancel"),Command.CANCEL,4));
    setCommandListener(this);
    setItemStateListener(this);
  }

  public void itemStateChanged(Item item) {
    if(item==cg){
//      System.out.println("hkjh");
      long tm=history[cg.getSelectedIndex()];
      dateField.setDate(new Date(tm+Event.localOffset(tm)));
//      setTimePrompt(invoker,0);
    }
  }
  
  public void commandAction(Command c, Displayable d)  {
    if(c.getPriority() == 3){
      int sel=cg.getSelectedIndex();
      String str=cg.getString(sel);
      if(str.endsWith("*")){
        str=str.substring(0,str.length()-1);
      }
      else{
        str+="*";
      }
      lockFlags^=(1<<sel);
//#debug debug      
      System.out.println("Lock="+Integer.toBinaryString(lockFlags));
      cg.set(sel,str,null);
      return;
    }
    Astromaximum.options.saveHistory();
    if (c.getPriority() == 4){
      Astromaximum.summary.dontRender();
    }
    if (c.getPriority() == 2){
      Astromaximum.summary.isShowCustom=true;
//#if timeHistory
      if(!setTime(showHistory)){
        return;
      }
//#else
//#       setTime(false);
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
        sDate="";//Event.long2String((Astromaximum.summary.period0+
          //Astromaximum.summary.period0)>>1,0,false).substring(0,5);
        decumbDate=dateField.getDate().getTime();
        decumbDate-=Event.localOffset(decumbDate);
        break;
      default:  
        tit=LocalizationSupport.getMessage("Date");

        sDate=Event.long2String((Astromaximum.summary.period0+
          Astromaximum.summary.period0)>>1,0,false).substring(0,5);
//        sDate=Event.long2String(tm,0,false);
    }
    setTitle(tit+" "+sDate);
  }

  boolean setTime(boolean addHistory) {
    Date dt=(addHistory? dateField:timeField).getDate();
    Astromaximum.calendar.setTime(dt);
    Astromaximum.summary.setCustomTime(
        Astromaximum.calendar.get(Calendar.HOUR_OF_DAY),Astromaximum.calendar.get(Calendar.MINUTE));
    if(addHistory){
      long tmp=dt.getTime();
      tmp-=Event.localOffset(tmp);
      if(!Astromaximum.dataFile.isDateAvailable(tmp)){
        Alert alert=new Alert(LocalizationSupport.getMessage("Error"),
            LocalizationSupport.getMessage("Date_unavail"), null, AlertType.ERROR);
        Display.getDisplay(Astromaximum.instance).setCurrent(alert,this);
        return false;
      }
      decumbDate=tmp;
      for(int i=0; i<histCount; i++){
        if(history[i]==decumbDate){
          cg.setSelectedIndex(i,true);
          return true;
        }
      }
      if(histCount<HIST_COUNT){
        for(int i=histCount-1; i>=0; i--){
          history[i+1]=history[i];
        }
        ++histCount;
      }
      else{
        int newLock=0; 
        for(int i=histCount-1; i>=0; i--){
          newLock<<=1;
          if((lockFlags & (1<<i))!=0){ //locked
            ++newLock;
          }
          else{
            if(addHistory){
              cg.delete(i);
              addHistory=false;
            }
          }
        }
        lockFlags=newLock;
      }
      history[0]=decumbDate;
      cg.insert(0,Event.long2String(history[0],0,false),null);
      cg.setSelectedIndex(0,true);
    }
    return true;
  }

  void init(int pn) {
    setTimePrompt(Astromaximum.summary.pageNum,timeField.getDate().getTime());
//#if timeHistory
    deleteAll();
    showHistory= pn==Summary.PAGE_PANEL || pn==Summary.PAGE_DECUMB;
    if(showHistory){
      append(timeField);
      append(dateField);
      append(cg);
    }
    else{
      append(timeField);
    }
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
