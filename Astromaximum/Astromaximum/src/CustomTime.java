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
//#if "timeBomb" @ protection
//#   static int hj=0x89abcdef;
//#endif
  int invoker=Event.EV_ASP_EXACT_MOON;
  final DateField timeField;
  final DateField dateField;
  long decumbDate;
  int lockFlags;
  final ChoiceGroup cg;
  static final int HIST_COUNT=8;
  static long[] history=new long[HIST_COUNT];
  static int histCount=0;
  private boolean showHistory;
  private Command[] cmds;
  /** Creates a new instance of CustomTime */
  CustomTime() {
    super("");
    timeField=new DateField(null,DateField.TIME,
        Astromaximum.calendar.getTimeZone());
    
    dateField=new DateField(null,DateField.DATE,Astromaximum.calendar.getTimeZone());
    decumbDate=System.currentTimeMillis();
    dateField.setDate(new Date(decumbDate+Event.localOffset(decumbDate)));
//    System.out.print("<> ");
//    System.out.println(dateField.getDate());
    timeField.setDate(new Date((decumbDate+Event.localOffset(decumbDate))%Astromaximum.MSECINDAY));
    cg=new ChoiceGroup(null/*LocalizationSupport.getMessage("History")*/, Choice.EXCLUSIVE);
    for(int i=0; i<histCount; i++){
      cg.append(Event.long2String(history[i],0,false),null);
    }
    cmds=new Command[]{
      new Command("OK",Command.OK,1),
      new Command(LocalizationSupport.getMessage("Lock/Unlock"),Command.ITEM,2),
      new Command(LocalizationSupport.getMessage("Delete"),Command.ITEM,3),
      new Command(LocalizationSupport.getMessage("Cancel"),Command.CANCEL,4),
    };
    setCommandListener(this);
    setItemStateListener(this);
//#if logger
//#       Astromaximum.instance.logger("inside CustomTime");
//#       Astromaximum.instance.logger(timeField.getDate().toString());
//#endif      
  }

  public void itemStateChanged(Item item) {
    if(item==cg){
//      System.out.println("hkjh");
      int sel=cg.getSelectedIndex();
      if(sel>=0){
        long tm=history[sel];
        tm+=Event.localOffset(tm);
        dateField.setDate(new Date(tm));
//        System.out.print("isc ");
//        System.out.println(dateField.getDate());
        timeField.setDate(new Date(tm%Astromaximum.MSECINDAY));
      }
    }
  }
  
  private void deleteHistItem(int sel){
    if(sel>=0 && sel<histCount){
      cg.delete(sel);
      int newLock=0;
      for(int i=histCount-1; i>=0; i--){
        if(i!=sel){
          newLock<<=1;
          if((lockFlags & (1<<i))!=0){
            ++newLock;
          }
        }
      }
      lockFlags=newLock;
      for(int i=sel+1; i<histCount; i++){
        System.out.println(i);
        history[i-1]=history[i];
      }
      --histCount;
    }
  }
  
  public void commandAction(Command c, Displayable d)  {
    if (c.getPriority() == 4){
      Astromaximum.summary.dontRender();
      return;
    }
    if (c.getPriority() == 3){
      int sel=cg.getSelectedIndex();
      if(sel>=0 && !cg.getString(sel).endsWith("*")){
        deleteHistItem(sel);
        itemStateChanged(cg);
      }
      return;
    }
    if(c.getPriority() == 2){
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
    if (c.getCommandType() == Command.OK){
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
      Astromaximum.options.saveHistory();
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
//#if logger
//#       Astromaximum.instance.logger(timeField.getDate().toString());
//#endif      
    long tmp=timeField.getDate().getTime();
//#if logger
//#       Astromaximum.instance.logger(Event.long2String(tmp,0,false));
//#endif      
    if(addHistory){
      tmp+=dateField.getDate().getTime();
    }
    Astromaximum.calendar.setTime(new Date(tmp));
//#if logger
//#       Astromaximum.instance.logger("before setCustomTime");
//#endif      
    Astromaximum.summary.setCustomTime(
        Astromaximum.calendar.get(Calendar.HOUR_OF_DAY),Astromaximum.calendar.get(Calendar.MINUTE));
    if(addHistory){
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
      if(histCount>=HIST_COUNT){
        int i=histCount;
        do{
          --i;
        }while(i>=0 && (lockFlags & (1<<i))!=0);
        if(i<0){
          return false;
        }
        deleteHistItem(i);
      }
      lockFlags<<=1;
      for(int i=histCount-1; i>=0; i--){
        history[i+1]=history[i];
      }
      ++histCount;
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
    for(int i=0; i<4; i++){
      removeCommand(cmds[i]);
    }
    showHistory= pn==Summary.PAGE_PANEL || pn==Summary.PAGE_DECUMB;
    if(showHistory){
      timeField.setLabel(null);
      append(dateField);
//      System.out.print("init ");
//      System.out.println(dateField.getDate());
      append(timeField);
      append(cg);
      for(int i=0; i<4; i++){
        addCommand(cmds[i]);
      }
    }
    else{
      timeField.setLabel(LocalizationSupport.getMessage("Enter_time:"));
      append(timeField);
      addCommand(cmds[0]);
      addCommand(cmds[3]);
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
