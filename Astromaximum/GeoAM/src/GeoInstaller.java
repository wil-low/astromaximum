/*
 * GeoInstaller.java
 *
 * Created on 08 February 2007, 18:03
 */

import java.io.DataInputStream;
import java.io.IOException;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreNotOpenException;

/**
 *
 * @author  Administrator
 * @version
 */
public class GeoInstaller extends MIDlet implements CommandListener{
  private GeoList gl;
  private boolean interrupt=false;
  
  public void startApp() {
    Alert alert=new Alert("Installer",
       "Loading list of cities, please wait...",null,AlertType.INFO);
    alert.setTimeout(Alert.FOREVER);
    alert.addCommand(new Command("Cancel",Command.OK,1));
    alert.setCommandListener(this);
    Display.getDisplay(this).setCurrent(alert);
    gl=new GeoList(this,Choice.MULTIPLE);
    gl.setCommandListener(this);
    gl.addCommand(new Command(gl.getMessage("Install"),
        Command.OK, 1));
    try {
      gl.initDB(false);
      gl.init();
      for(int i=0; i<gl.total; i++){
        if(interrupt)
          throw new IllegalArgumentException();
        gl.append(gl.extractCityName(gl.extractLocation(i)),null);
      }
      Display.getDisplay(this).setCurrent(gl);
    } 
    catch (Exception ex) {
       ex.printStackTrace();
       alert=new Alert("Error",
           "Astromaximum cities database is not found. Please install Astromaximum first.",
           null,AlertType.ERROR);
       alert.addCommand(new Command("OK",Command.ITEM,1));
       alert.setTimeout(Alert.FOREVER);
       alert.setCommandListener(this);
       Display.getDisplay(this).setCurrent(alert);
    }
  }
  
  public void pauseApp() {
  }
  
  public void destroyApp(boolean unconditional) {
  }

  public void commandAction(Command c, Displayable d) {
    switch(c.getCommandType()){
      case Command.OK:
        boolean[] selArray=new boolean[gl.size()];
        if(gl.getSelectedFlags(selArray)>0){
          for(int i=0; i<selArray.length; i++){
            if(selArray[i]){
              byte[] cn=gl.extractLocation(i);
              try {
                gl.rs.addRecord(cn,0,cn.length);
              }
              catch (RecordStoreException ex) {
              }
            }
          }
        }
      case Command.ITEM:
      case Command.CANCEL:
        quit();
    }
  }

  private void quit(){
    interrupt=true;
    Display.getDisplay(this).setCurrent(null);
    destroyApp(true);
    notifyDestroyed();
  }
  
  
}
