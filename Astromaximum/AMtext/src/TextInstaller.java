/*
 * TextInstaller.java
 *
 * Created on 08 February 2007, 18:03
 */

import java.io.DataInputStream;
import java.io.IOException;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;

//#define use_gauge1
/**
 *
 * @author  Administrator
 * @version
 */
public class TextInstaller extends MIDlet implements CommandListener {

    private boolean interrupt = false;
    private final String storeName = "AMtext";
    private Alert alert;
    private Command cmd;
    public void startApp() {
        alert = new Alert("Texts", "This will install interpretation texts for Astromaximum", null, AlertType.INFO);
        cmd=new Command("OK", Command.OK, 1);
        alert.addCommand(cmd);
        alert.setTimeout(Alert.FOREVER);
        alert.setCommandListener(this);
        Display.getDisplay(this).setCurrent(alert);
    }

    public void pauseApp() {
    }

    public void destroyApp(boolean unconditional) {
    }

    public void commandAction(Command c, Displayable d) {
        switch (c.getCommandType()) {
            case Command.OK:
                if(c.getLabel().equals("OK")){
                    String msg = "Texts installed\n";
                    String was="";
                    boolean success=false;
    //#ifdef use_gauge
    //#                 Form frm = new Form("Installing texts");
    //#                 frm.addCommand(new Command("Cancel", Command.CANCEL, 1));
    //#                 frm.setCommandListener(this);
    //#endif
                    try {
                        DataInputStream dis = new DataInputStream(getClass().getResourceAsStream("index"));
                        int count = dis.available();

    //#ifdef use_gauge
    //#                     Gauge progress=new Gauge("", false, count, 0);
    //#                     frm.append(progress);
    //#                     Display.getDisplay(this).setCurrent(frm);
    //#endif

                        byte[] fnames = new byte[count];
                        dis.read(fnames, 0, count);
                        dis.close();
                        try {
                            RecordStore.deleteRecordStore(storeName);
                        } catch (RecordStoreNotFoundException ex) {}

                        RecordStore rs = RecordStore.openRecordStore(storeName, true,
                                RecordStore.AUTHMODE_ANY, false);
                        was=" storage was "+new Integer(rs.getSizeAvailable()).toString()+" bytes";
                        rs.addRecord(fnames, 0, count); // here will be index
                        byte[] text = null;
                        for (int i = 0; i < count; i++) {
                            dis = new DataInputStream(getClass().getResourceAsStream(new Byte(fnames[i]).toString()));
                            int len = dis.available();
                            text = new byte[len];
                            dis.read(text, 0, len);
                            rs.addRecord(text, 0, len);
    //#ifdef use_gauge
    //#                         progress.setValue(i+1);
    //#endif
                            dis.close();
                        }
                        rs.closeRecordStore();
                        success=true;
                    } catch (RecordStoreFullException ex) {
                        msg="Error when installing texts:\n No space -"+was;
                    } catch (RecordStoreNotFoundException ex) {
                        msg="Error when installing texts:\n Store not found";
                    } catch (RecordStoreException ex) {
                        msg="Error when installing texts:\n General error";
                    } catch (IOException ex) {
                        msg="Error: bad midlet data: "+ex.getMessage();
                    }
                    if(!success){
                        try {
                            RecordStore.deleteRecordStore(storeName);
                        } catch (RecordStoreException ex) {}
                    }
                    else{
                        msg+=was;
                    }
                    alert.setString(msg);
                    alert.removeCommand(cmd);
                    alert.addCommand(new Command("Close", Command.OK, 1));
//                    Display.getDisplay(this).setCurrent(alert);
                }
                else{
                    destroyApp(true);
                    notifyDestroyed();
                }
                break;
//            case Command.ITEM:
//            case Command.CANCEL:
        }
    }
}
