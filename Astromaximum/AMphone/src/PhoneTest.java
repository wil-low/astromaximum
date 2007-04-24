/*
 * PhoneTest.java
 *
 * Created on April 24, 2007, 1:50 PM
 */

import java.util.Calendar;
import java.util.TimeZone;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

/**
 *
 * @author aivushkin
 */
public class PhoneTest extends MIDlet implements CommandListener {
  private final String[] imeiCodes={
    "com.sonyericsson.IMEI",
    "com.samsung.IMEI",
    "com.samsung.imei",
    "com.samsungmobile.IMEI",
    "com.samsungmobile.imei",
    "com.siemens.mp.imei",
    "phone.imei",
    "phone.IMEI",
    "com.nokia.mid.imei",
    "com.nokia.IMEI",
    "device.imei",
    "device.IMEI",
    "imei",
    "IMEI"
  };
  private final String[] props={
    "microedition.commports",
    "microedition.configuration",
    "microedition.encoding",
    "microedition.hostname",
    "microedition.io.file.FileConnection.version",
    "microedition.jtwi.version",
    "microedition.locale",
    "microedition.m3g.version",
    "microedition.media.version",
    "microedition.pim.version",
    "microedition.platform",
    "microedition.profiles",
    "microedition.smartcardslots",
    "microedition.timezone",
    "CellID",
    "GPRSState",
    "IMSI",
    "LocAreaCode",
    "MAType",
    "MSISDN",
    "audio.encodings",
    "batterylevel",
    "com.mot.carrier.URL",
    "commports.maxbaudrate",
    "default.timezone",
    "device.model",
    "device.software.version",
    "file.separator",
    "funlight.product",
    "language.direction",
    "location_category_max_length",
    "location_landmark_max_length",
    "midp_alert_done",
    "midp_command_menu",
    "midp_enhance_create_cropped_image",
    "midp_enhance_flush_game_graphics",
    "midp_game_flush_compatible_wt_wtk",
    "midp_screen_background",
    "midp_screen_foreground",
    "midp_screen_linked_color",
    "midp_scrollbar_width",
    "midp_selector_exit",
    "midp_selector_launch_failed",
    "midp_vertical_scroll",
    "supports.audio.capture",
    "supports.mixing",
    "supports.recording",
    "supports.video.capture",
    "video.encodings",
    "video.snapshot.encodings",
    "wireless.messaging.mms.mmsc",
    "wireless.messaging.sms.smsc"
  };
   
  int screenWidth, screenHeight;
  /** Creates a new instance of PhoneTest */
  public PhoneTest() {
    initialize();
  }
  
  private List imeiList;//GEN-BEGIN:MVDFields
  private org.netbeans.microedition.util.SimpleCancellableTask speedTask;
  private org.netbeans.microedition.lcdui.WaitScreen waitScreen;
  private List propList;
  private Command exitCommand;
  private Command nextCommand;
  private Command backCommand;//GEN-END:MVDFields
  
//GEN-LINE:MVDMethods

  /** Called by the system to indicate that a command has been invoked on a particular displayable.//GEN-BEGIN:MVDCABegin
   * @param command the Command that ws invoked
   * @param displayable the Displayable on which the command was invoked
   */
  public void commandAction(Command command, Displayable displayable) {//GEN-END:MVDCABegin
  // Insert global pre-action code here
    if (displayable == waitScreen) {//GEN-BEGIN:MVDCABody
      if (command == waitScreen.FAILURE_COMMAND) {//GEN-END:MVDCABody
	// Insert pre-action code here
	getDisplay().setCurrent(get_imeiList());//GEN-LINE:MVDCAAction14
	// Insert post-action code here
      } else if (command == waitScreen.SUCCESS_COMMAND) {//GEN-LINE:MVDCACase14
	// Insert pre-action code here
	getDisplay().setCurrent(get_imeiList());//GEN-LINE:MVDCAAction13
	// Insert post-action code here
      }//GEN-BEGIN:MVDCACase13
    } else if (displayable == imeiList) {
      if (command == exitCommand) {//GEN-END:MVDCACase13
	// Insert pre-action code here
	exitMIDlet();//GEN-LINE:MVDCAAction23
	// Insert post-action code here
      } else if (command == nextCommand) {//GEN-LINE:MVDCACase23
	// Insert pre-action code here
	getDisplay().setCurrent(get_propList());//GEN-LINE:MVDCAAction25
	// Insert post-action code here
      }//GEN-BEGIN:MVDCACase25
    } else if (displayable == propList) {
      if (command == exitCommand) {//GEN-END:MVDCACase25
	// Insert pre-action code here
	exitMIDlet();//GEN-LINE:MVDCAAction26
	// Insert post-action code here
      } else if (command == backCommand) {//GEN-LINE:MVDCACase26
	// Insert pre-action code here
	getDisplay().setCurrent(get_imeiList());//GEN-LINE:MVDCAAction28
	// Insert post-action code here
      }//GEN-BEGIN:MVDCACase28
    }//GEN-END:MVDCACase28
  // Insert global post-action code here
}//GEN-LINE:MVDCAEnd

  /** This method initializes UI of the application.//GEN-BEGIN:MVDInitBegin
   */
  private void initialize() {//GEN-END:MVDInitBegin
    // Insert pre-init code here
    getDisplay().setCurrent(get_waitScreen());//GEN-LINE:MVDInitInit
    // Insert post-init code here
  }//GEN-LINE:MVDInitEnd
  
  /**
   * This method should return an instance of the display.
   */
  public Display getDisplay() {//GEN-FIRST:MVDGetDisplay
    return Display.getDisplay(this);
  }//GEN-LAST:MVDGetDisplay
  
  /**
   * This method should exit the midlet.
   */
  public void exitMIDlet() {//GEN-FIRST:MVDExitMidlet
    getDisplay().setCurrent(null);
    destroyApp(true);
    notifyDestroyed();
  }//GEN-LAST:MVDExitMidlet

  /** This method returns instance for speedTask component and should be called instead of accessing speedTask field directly.//GEN-BEGIN:MVDGetBegin11
   * @return Instance for speedTask component
   */
  public org.netbeans.microedition.util.SimpleCancellableTask get_speedTask() {
    if (speedTask == null) {//GEN-END:MVDGetBegin11
      // Insert pre-init code here
      speedTask = new org.netbeans.microedition.util.SimpleCancellableTask();//GEN-BEGIN:MVDGetInit11
      speedTask.setExecutable(new org.netbeans.microedition.util.Executable() {
	public void execute() throws Exception {
	  
	}
      });//GEN-END:MVDGetInit11
      // Insert post-init code here
      
    }//GEN-BEGIN:MVDGetEnd11
    return speedTask;
  }//GEN-END:MVDGetEnd11

  /** This method returns instance for waitScreen component and should be called instead of accessing waitScreen field directly.//GEN-BEGIN:MVDGetBegin12
   * @return Instance for waitScreen component
   */
  public org.netbeans.microedition.lcdui.WaitScreen get_waitScreen() {
    if (waitScreen == null) {//GEN-END:MVDGetBegin12
      // Insert pre-init code here
      waitScreen = new org.netbeans.microedition.lcdui.WaitScreen(getDisplay());//GEN-BEGIN:MVDGetInit12
      waitScreen.setCommandListener(this);
      waitScreen.setFullScreenMode(true);
      waitScreen.setText("Calculating speed...");//GEN-END:MVDGetInit12
      // Insert post-init code here
      screenWidth=waitScreen.getWidth();
      screenHeight=waitScreen.getHeight();
    }//GEN-BEGIN:MVDGetEnd12
    return waitScreen;
  }//GEN-END:MVDGetEnd12

  /** This method returns instance for imeiList component and should be called instead of accessing imeiList field directly.//GEN-BEGIN:MVDGetBegin4
   * @return Instance for imeiList component
   */
  public List get_imeiList() {
    if (imeiList == null) {//GEN-END:MVDGetBegin4
      // Insert pre-init code here
      imeiList = new List("Results", Choice.IMPLICIT, new String[0], new Image[0]);//GEN-BEGIN:MVDGetInit4
      imeiList.addCommand(get_exitCommand());
      imeiList.addCommand(get_nextCommand());
      imeiList.setCommandListener(this);
      imeiList.setSelectedFlags(new boolean[0]);//GEN-END:MVDGetInit4
      // Insert post-init code here
      imeiList.setTitle(Integer.toString(screenWidth)+"x"+
	  Integer.toString(screenHeight));
      
      Calendar cal=Calendar.getInstance(TimeZone.getTimeZone("GMT"));
      String strDate="GMT "+cal.get(Calendar.DAY_OF_MONTH)+"."+
	  cal.get(Calendar.MONTH)+"."+
	  cal.get(Calendar.YEAR)+" "+
	  cal.get(Calendar.HOUR_OF_DAY)+"."+
	  cal.get(Calendar.MINUTE);
      
      imeiList.append(strDate, null);
      
      Runtime runtime=Runtime.getRuntime();
      imeiList.append("Free "+Long.toString(runtime.freeMemory()/1024)+" kb from "+
	  Long.toString(runtime.totalMemory()/1024)+" kb" , null);

      for(int i=0; i<imeiCodes.length; i++){
	String response=System.getProperty(imeiCodes[i]);
	if(response!=null){
	  imeiList.append(imeiCodes[i]+"="+response, null);
	}
      }
      
    }//GEN-BEGIN:MVDGetEnd4
    return imeiList;
  }//GEN-END:MVDGetEnd4

  /** This method returns instance for propList component and should be called instead of accessing propList field directly.//GEN-BEGIN:MVDGetBegin16
   * @return Instance for propList component
   */
  public List get_propList() {
    if (propList == null) {//GEN-END:MVDGetBegin16
      // Insert pre-init code here
      propList = new List("", Choice.IMPLICIT, new String[0], new Image[0]);//GEN-BEGIN:MVDGetInit16
      propList.addCommand(get_exitCommand());
      propList.addCommand(get_backCommand());
      propList.setCommandListener(this);
      propList.setSelectedFlags(new boolean[0]);//GEN-END:MVDGetInit16
      // Insert post-init code here
      for(int i=0; i<props.length; i++){
	String response=System.getProperty(props[i]);
	if(response!=null){
	  propList.append(props[i]+":", null);
	  propList.append("  "+response, null);
	}
      }
      propList.setTitle("Properties: "+Integer.toString(propList.size()/2));
      
    }//GEN-BEGIN:MVDGetEnd16
    return propList;
  }//GEN-END:MVDGetEnd16

  /** This method returns instance for exitCommand component and should be called instead of accessing exitCommand field directly.//GEN-BEGIN:MVDGetBegin21
   * @return Instance for exitCommand component
   */
  public Command get_exitCommand() {
    if (exitCommand == null) {//GEN-END:MVDGetBegin21
      // Insert pre-init code here
      exitCommand = new Command("Exit", Command.EXIT, 1);//GEN-LINE:MVDGetInit21
      // Insert post-init code here
    }//GEN-BEGIN:MVDGetEnd21
    return exitCommand;
  }//GEN-END:MVDGetEnd21

  /** This method returns instance for nextCommand component and should be called instead of accessing nextCommand field directly.//GEN-BEGIN:MVDGetBegin24
   * @return Instance for nextCommand component
   */
  public Command get_nextCommand() {
    if (nextCommand == null) {//GEN-END:MVDGetBegin24
      // Insert pre-init code here
      nextCommand = new Command("Next", Command.SCREEN, 1);//GEN-LINE:MVDGetInit24
      // Insert post-init code here
    }//GEN-BEGIN:MVDGetEnd24
    return nextCommand;
  }//GEN-END:MVDGetEnd24

  /** This method returns instance for backCommand component and should be called instead of accessing backCommand field directly.//GEN-BEGIN:MVDGetBegin27
   * @return Instance for backCommand component
   */
  public Command get_backCommand() {
    if (backCommand == null) {//GEN-END:MVDGetBegin27
      // Insert pre-init code here
      backCommand = new Command("Back", Command.BACK, 1);//GEN-LINE:MVDGetInit27
      // Insert post-init code here
    }//GEN-BEGIN:MVDGetEnd27
    return backCommand;
  }//GEN-END:MVDGetEnd27
  
  public void startApp() {
  }
  
  public void pauseApp() {
  }
  
  public void destroyApp(boolean unconditional) {
  }
  
}
