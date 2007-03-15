import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Calendar;
import java.util.Date;
import javax.microedition.lcdui.*;
/*
 * CustomTime.java
 *
 * Created on 29 грудня 2006, 18:42
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

/**
 *
 * @author Administrator
 */
final class CustomTime extends Form implements CommandListener,ItemStateListener{
//  private final TextField hours;
//#if "timeBomb" @ protection
//#   static int hj=0x89abcdef;
//#endif
  private int invoker;
  final DateField dateField;
  final ChoiceGroup cg;
  static final int HIST_COUNT=5;
  static long[] history=new long[HIST_COUNT];
  static int histCount=0;
  private boolean showHistory;
  /** Creates a new instance of CustomTime */
  CustomTime() {
    super("");
/*
    hours=new TextField(LocalizationSupport.getMessage("Hours"),"",2,
//#if MIDP=="2.0"
        TextField.DECIMAL
//#else
//#         TextField.ANY
//#endif
        );
//    hours.setPreferredSize(getWidth()*45/100,-1);
    minutes=new TextField(LocalizationSupport.getMessage("Minutes"),"",2,
//#if MIDP=="2.0"
        TextField.DECIMAL
//#else
//#         TextField.ANY
//#endif
        );
//    minutes.setPreferredSize(getWidth()*45/100,-1);
//#if MIDP=="2.0"
    final int lay=Item.LAYOUT_2|Item.LAYOUT_SHRINK;
    hours.setLayout(lay);
    hours.setInitialInputMode("IS_LATIN_DIGITS");
    minutes.setLayout(lay);
    minutes.setInitialInputMode("IS_LATIN_DIGITS");
    final Spacer spc=new Spacer(getWidth()/3,1);
    spc.setLayout(Item.LAYOUT_2|Item.LAYOUT_EXPAND);
//#endif
    append(hours);
    append(minutes);
//#if MIDP=="2.0"
    append(spc);
//#endif
 **/
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
    if(item==cg){
      System.out.println("hkjh");
//      long tm=history[cg.getSelectedIndex()];
//      tm-=Event.localOffset(tm);
//      dateField.setDate(new Date(tm));
//      setTimePrompt(invoker,0);
    }
  }
  
  public void commandAction(Command c, Displayable d)  {
    if (c.getCommandType() == Command.CANCEL){
      Astromaximum.summary.dontRender();
    }
    else{
      Astromaximum.summary.isShowCustom=true;
      setTime(showHistory);
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
    if(addHistory){
      for(int i=0; i<histCount; i++){
        if(history[i]==Astromaximum.summary.cusTime){
          cg.setSelectedIndex(i,true);
          return;
        }
      }
      for(int i=histCount-1; i>0; i--){
        history[i]=history[i-1];
      }
      history[0]=Astromaximum.summary.cusTime;
      if(histCount<HIST_COUNT){
        ++histCount;
      }
      cg.insert(0,Event.long2String(history[0],0,false),null);
      cg.setSelectedIndex(0,true);
      while(cg.size()>HIST_COUNT){
        cg.delete(cg.size()-1);
      }
      Astromaximum.options.saveHistory();
    }
  }

  void init(int pn) {
    setTimePrompt(Astromaximum.summary.pageNum,dateField.getDate().getTime());
    if(get(size()-1)==cg){
      delete(size()-1);
    }
    showHistory= pn==Summary.PAGE_PANEL || pn==Summary.PAGE_DECUMB;
    if(showHistory){
//      append(cg);
    }
    Display.getDisplay(Astromaximum.instance).setCurrent(this);
  }
  
}
