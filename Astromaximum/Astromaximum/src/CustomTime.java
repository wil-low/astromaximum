//#define timeHistory

//import java.io.ByteArrayOutputStream;
//import java.io.DataInputStream;
//import java.io.DataOutputStream;
import java.util.Calendar;
import java.util.Date;
import javax.microedition.lcdui.*;
/*
 * CustomTime.java
 *
 * Created on 29 ������ 2006, 18:42
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

/**
 * CustomTime form allows to select any hour and minute to be highlighted
 * every day with "blue mark".
 * <p>Also used for {@link Summary#PAGE_DECUMB decumbiture page}.</p>
 *
 * @author Administrator
 */
final class CustomTime extends Form implements CommandListener, ItemStateListener {
//#if "timeBomb" @ protection
    /**
     * Secret variable for timeBomb protection (end of time range)
     */
    static final int hj = 0x89abcdef;
//#endif
    private int invoker = Event.EV_ASP_EXACT_MOON;
    final DateField timeField;
    final DateField dateField;
//#if microemu
//#     static final String commons[] = {
//#         "COMMON_DAT0",
//#         "COMMON_DAT1",
//#         "COMMON_DAT2",
//#     };
//#endif
    /**
     * Decumbiture date storage variable
     */
    long decumbDate;
    /**
     * Bitmask of locking status of items in recent time list
     */
    int lockFlags;
    /**
     * Recently entered times list
     */
    final ChoiceGroup cg;
    private static final int HIST_COUNT = 8;
    /**
     * Array holding entered times
     */
    static final long[] history = new long[HIST_COUNT];
    /**
     * History item count
     */
    static int histCount = 0;
    private boolean showHistory;
    private final Command[] cmds;

    /**
     * Creates a new instance of CustomTime
     */
    CustomTime() {
        super("");
        timeField = new DateField(null, DateField.TIME,
                Astromaximum.calendar.getTimeZone());

        dateField = new DateField(null, DateField.DATE, Astromaximum.calendar.getTimeZone());
        decumbDate = System.currentTimeMillis();
        dateField.setDate(new Date(decumbDate + Event.localOffset(decumbDate)));
//    System.out.print("<> ");
//    System.out.println(dateField.getDate());
        timeField.setDate(new Date((decumbDate + Event.localOffset(decumbDate)) % Astromaximum.MSECINDAY));
        cg = new ChoiceGroup(null/*LocalizationSupport.getMessage("History")*/, Choice.EXCLUSIVE);
        for (int i = 0; i < histCount; i++) {
            cg.append(Event.long2String(history[i], 0, false), null);
        }
        cmds = new Command[]{
                    new Command("OK", Command.OK, 1),
                    new Command(Astromaximum.getstr(117), Command.ITEM, 2),
                    new Command(Astromaximum.getstr(100), Command.ITEM, 3),
                    new Command(Astromaximum.getstr(97), Command.CANCEL, 4),
                };
        setCommandListener(this);
        setItemStateListener(this);
//#if logger
      Astromaximum.instance.logger("inside CustomTime");
      Astromaximum.instance.logger(timeField.getDate().toString());
//#endif
    }

    /**
     * Update DateFields when some history item is selected
     * @param item - should always be history list
     */
    public void itemStateChanged(Item item) {
        if (item == cg) {
//      System.out.println("hkjh");
            int sel = cg.getSelectedIndex();
            if (sel >= 0) {
                long tm = history[sel];
                tm += Event.localOffset(tm);
                dateField.setDate(new Date(tm));
//        System.out.print("isc ");
//        System.out.println(dateField.getDate());
                timeField.setDate(new Date(tm % Astromaximum.MSECINDAY));
            }
        }
    }

    private void deleteHistItem(int sel) {
        if (sel >= 0 && sel < histCount) {
            cg.delete(sel);
            int newLock = 0;
            for (int i = histCount - 1; i >= 0; i--) {
                if (i != sel) {
                    newLock <<= 1;
                    if ((lockFlags & (1 << i)) != 0) {
                        ++newLock;
                    }
                }
            }
            lockFlags = newLock;
            for (int i = sel + 1; i < histCount; i++) {
//#debug debug
                System.out.println(i);
                history[i - 1] = history[i];
            }
            --histCount;
        }
    }

    /**
     * Process important messages and transitions
     * @param c
     * @param d
     */
    public void commandAction(Command c, Displayable d) {
        if (c.getPriority() == 4) {
            Astromaximum.summary.dontRender();
            return;
        }
        if (c.getPriority() == 3) {
            int sel = cg.getSelectedIndex();
            if (sel >= 0 && !cg.getString(sel).endsWith("*")) {
                deleteHistItem(sel);
                itemStateChanged(cg);
            }
            return;
        }
        if (c.getPriority() == 2) {
            int sel = cg.getSelectedIndex();
            String str = cg.getString(sel);
            if (str.endsWith("*")) {
                str = str.substring(0, str.length() - 1);
            } else {
                str += "*";
            }
            lockFlags ^= (1 << sel);
//#debug debug
            System.out.println("Lock=" + Integer.toBinaryString(lockFlags));
            cg.set(sel, str, null);
            return;
        }
        if (c.getCommandType() == Command.OK) {
            Summary.isShowCustom = true;
//#if timeHistory
            if (!setTime(showHistory)) {
                return;
            }
//#else
//#       setTime(false);
//#endif
            switch (invoker) {
                case Summary.PAGE_PANEL:
                case Summary.PAGE_DECUMB:
                    Astromaximum.summary.calcDecumbiture();
            }
            Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.summary);
            arrangeHistory();
            Astromaximum.options.saveHistory();
        }
    }

    /**
     * Initialize form title and {@link #decumbDate} according to invoker page
     * @param pn invoker's page index
     */
    void setTimePrompt(int pn) {
        invoker = pn;
        String tit, sDate;
        switch (pn) {
            case Summary.PAGE_PANEL:
            case Summary.PAGE_DECUMB:
                tit = Astromaximum.getstr(142);
                sDate = "";//Event.long2String((Astromaximum.summary.period0+
                //Astromaximum.summary.period0)>>1,0,false).substring(0,5);
                decumbDate = dateField.getDate().getTime();
                decumbDate -= Event.localOffset(decumbDate);
                break;
            default:
                tit = Astromaximum.getstr(141);

                sDate = Event.long2String((Summary.period0 +
                        Summary.period0) >> 1, 0, false).substring(0, 5);
//        sDate=Event.long2String(tm,0,false);
        }
        setTitle(tit + " " + sDate);
    }

    /**
     * Set custom time marker
     * @param addHistory boolean
     * @return true if date is added to history, otherwise show alert message
     */
    boolean setTime(boolean addHistory) {
//#if logger
      Astromaximum.instance.logger(timeField.getDate().toString());
//#endif
        long tmp = timeField.getDate().getTime();
//#if logger
      Astromaximum.instance.logger(Event.long2String(tmp,0,false));
//#endif
        if (addHistory) {
            tmp += dateField.getDate().getTime();
        }
        Astromaximum.calendar.setTime(new Date(tmp));
//#if logger
      Astromaximum.instance.logger("before setCustomTime");
//#endif
        Astromaximum.summary.setCustomTime(
                Astromaximum.calendar.get(Calendar.HOUR_OF_DAY), Astromaximum.calendar.get(Calendar.MINUTE));
        if (addHistory) {
            tmp -= Event.localOffset(tmp);
            if (!Astromaximum.dataFile.isDateAvailable(tmp)) {
                Alert alert = new Alert(Astromaximum.getstr(109),
                        Astromaximum.getstr(116), null, AlertType.ERROR);
                Display.getDisplay(Astromaximum.instance).setCurrent(alert, this);
                return false;
            }
            decumbDate = tmp;
        }
        return true;
    }

    /**
     * Recreate form controls according to invoker
     * @param pn invoker page index
     */
    void init(int pn) {
        setTimePrompt(Astromaximum.summary.pageNum);
//#if timeHistory
        deleteAll();
        for (int i = 0; i < 4; i++) {
            removeCommand(cmds[i]);
        }
        showHistory = pn == Summary.PAGE_PANEL || pn == Summary.PAGE_DECUMB;
        if (showHistory) {
            timeField.setLabel(null);
            append(dateField);
//      System.out.print("init ");
//      System.out.println(dateField.getDate());
            append(timeField);
            append(cg);
            for (int i = 0; i < 4; i++) {
                addCommand(cmds[i]);
            }
        } else {
            timeField.setLabel(Astromaximum.getstr(139));
            append(timeField);
            addCommand(cmds[0]);
            addCommand(cmds[3]);
        }
//#endif
        Display.getDisplay(Astromaximum.instance).setCurrent(this);
    }

    /*
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
    //#debug debug
          System.out.print("Available=");
    //#debug debug
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
     */
    /**
     * Reconstruct history list from {@link #history} after modifications
     * @return boolean
     */
    boolean arrangeHistory() {
        for (int i = 0; i < histCount; i++) {
            if (history[i] == decumbDate) {
                cg.setSelectedIndex(i, true);
                return true;
            }
        }
        if (histCount >= HIST_COUNT) {
            int i = histCount;
            do {
                --i;
            } while (i >= 0 && (lockFlags & (1 << i)) != 0);
            if (i < 0) {
                return false;
            }
            deleteHistItem(i);
        }
        lockFlags <<= 1;
        for (int i = histCount - 1; i >= 0; i--) {
            history[i + 1] = history[i];
        }
        ++histCount;
        history[0] = decumbDate;
        cg.insert(0, Event.long2String(history[0], 0, false), null);
        cg.setSelectedIndex(0, true);
        return true;
    }
}

// # vi:et:ts=4:sw=4
