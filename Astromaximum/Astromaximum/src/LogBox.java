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
 */
//#ifdef build.desktop
//# package com.sw_axis;
//# import java.awt.Frame;
//# 
//# class LogBox extends Frame{
//#else
import javax.microedition.lcdui.*;

class LogBox extends List implements CommandListener{
  static final String EMPTY=LocalizationSupport.getMessage("<Empty>");
  private Displayable invoker;
  static private byte[] buf=new byte[8];
  LogBox() {
    super(LocalizationSupport.getMessage("Log"), Choice.IMPLICIT);
    append(EMPTY,null);
    addCommand(new Command(LocalizationSupport.getMessage("Back"), Command.BACK, 1));
    addCommand(new Command(LocalizationSupport.getMessage("Clear"), Command.STOP, 1));
    setCommandListener(this);
  }
  
  
  /**
   * showLog
   *
   * @param displayable Displayable
   */
  void showLog(Displayable displayable) {
    invoker=displayable;
    setSelectedIndex(size()-1,true);
    Display.getDisplay(Astromaximum.instance).setCurrent(this);
  }
  
  /** @noinspection InfiniteLoopStatement*/
  public void commandAction(Command c, Displayable d)  {
    if (c.getCommandType() == Command.BACK) {
      Display.getDisplay(Astromaximum.instance).setCurrent(invoker);
    }
    if (c.getCommandType() == Command.STOP) {
//#if MIDP == "2.0"
      deleteAll();
//#else
//#    try {
//#      while(true)
//#        delete(0);
//#    } catch (IndexOutOfBoundsException iob) {}
//#endif
      append(EMPTY,null);
    }
  }
  
  static String access(String str, int param) {
    String ss="";
    int idx=str.indexOf('.');
    while(idx>=0){
      long res=Long.parseLong(str.substring(0,idx),param+15);
      str=str.substring(idx+1);
      int i=8;
      while(res>0){
	buf[--i]=(byte)res;
	res>>=8;
      }
      ss+=new String(buf,i,8-i);
      idx=str.indexOf('.');
    }
    
    return ss;
  }
//#endif  
}
