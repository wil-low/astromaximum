/*
 * GeoInstaller.java
 *
 * Created on 08 February 2007, 18:03
 */

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.RecordStoreException;
import java.io.IOException;

/**
 *
 * @author  Administrator
 * @version
 */
public class GeoInstaller extends MIDlet implements CommandListener {

    private GeoList gl;
    private boolean interrupt = false;

    public void startApp() {
        gl = new GeoList(this, Choice.MULTIPLE, "locations.dat");
        gl.setCommandListener(this);
        gl.addCommand(new Command("Cancel", Command.CANCEL, 1));
        Command cmd = new Command(gl.getMessage("Install"), Command.OK, 1);
        gl.addCommand(cmd);
        try {
            gl.initDB(false);
            gl.init();
            for (int i = 0; i < gl.total; i++) {
                if (interrupt) {
                    throw new IllegalArgumentException();
                }
                gl.cityList.append(gl.extractCityName(gl.extractLocation(i)), null);
            }
            System.out.println(gl.cityList.size());
            Display.getDisplay(this).setCurrent(gl);
            if (gl.cityList.size() == 1) {
                gl.cityList.setSelectedIndex(0, true);
                commandAction(cmd, gl);
            }
        }
        catch (RecordStoreException ex) {
            reportError("Astromaximum cities database is not found. Please install Astromaximum " +
                    Integer.toString(gl.year) + ": <" + ex.getMessage() + ">", Command.ITEM);
        }
        catch (IOException ex) {
            reportError("Cannot read Astromaximum cities database. Please reinstall Astromaximum " +
                    Integer.toString(gl.year) + ": <" + ex.getMessage() + ">", Command.ITEM);
        }
    }

    public void pauseApp() {
    }

    public void destroyApp(boolean unconditional) {
    }

    public void commandAction(Command c, Displayable d) {
        switch (c.getCommandType()) {
            case Command.OK:
                String msg = "Cities installed: ";
                AlertType at = AlertType.INFO;
                boolean[] selArray = new boolean[gl.cityList.size()];
                if (gl.cityList.getSelectedFlags(selArray) > 0) {
                    for (int i = 0; i < selArray.length; i++) {
                        if (selArray[i]) {
                            try {
                                byte[] cn = gl.extractLocation(i);
                                msg += gl.extractCityName(cn) + "; ";
                                gl.rs.addRecord(cn, 0, cn.length);
                            } catch (RecordStoreException ex) {
                                msg = "An error occured when installing cities!  " + ex.toString() + "  ";
                                at = AlertType.ERROR;
                            }
                        }
                    }
                    Alert alert = new Alert("GeoInstaller", msg.substring(0, msg.length()-2), null, at);
                    alert.addCommand(new Command("Close", Command.CANCEL, 1));
                    alert.setCommandListener(this);
                    alert.setTimeout(Alert.FOREVER);
                    Display.getDisplay(this).setCurrent(alert);
                }
                break;
            case Command.ITEM:
            case Command.CANCEL:
                quit();
        }
    }

    private void quit() {
        interrupt = true;
        Display.getDisplay(this).setCurrent(null);
        destroyApp(true);
        notifyDestroyed();
    }

    private void reportError(String str, int commandType) {
        Alert alert = new Alert("Error", str, null, AlertType.ERROR);
        alert.addCommand(new Command("OK", commandType, 1));
        alert.setTimeout(Alert.FOREVER);
        alert.setCommandListener(this);
        Display.getDisplay(this).setCurrent(alert);
    }
}

// # vi:et:ts=4:sw=4
