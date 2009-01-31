
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
import javax.microedition.lcdui.*;

class LogBox extends List implements CommandListener {

    static String EMPTY;//Empty
    private Displayable invoker;
    static private final byte[] buf = new byte[8];

    LogBox() {
        super("", Choice.IMPLICIT);//Log
        setCommandListener(this);
    }

    void askResetDB() {
        invoker = Astromaximum.disp.getCurrent();
        Alert al = new Alert(Astromaximum.getstr(158),
                Astromaximum.getstr(159), null, AlertType.ERROR);
        al.addCommand(new Command(Astromaximum.getstr(157), Command.EXIT, 0));
        al.addCommand(new Command("OK", Command.OK, 10));
        al.setCommandListener(this);
        Astromaximum.disp.setCurrent(al);
    }

    void init() {
        setTitle(Astromaximum.getstr(95));
        EMPTY = Astromaximum.getstr(119);
        if (size() == 0) {
            append(EMPTY, null);
        }
        addCommand(new Command(Astromaximum.getstr(94), Command.BACK, 1));//Back
        addCommand(new Command(Astromaximum.getstr(96), Command.STOP, 1));//Clear
    }

    /**
     * showLog
     *
     * @param displayable Displayable
     */
    void showLog(Displayable displayable) {
        invoker = displayable;
        setSelectedIndex(size() - 1, true);
        Astromaximum.disp.setCurrent(this);
    }

    /**
     * @noinspection InfiniteLoopStatement
     */
    public void commandAction(Command c, Displayable d) {
        switch (c.getCommandType()) {
            case Command.BACK:
                Astromaximum.disp.setCurrent(invoker);
                break;
            case Command.STOP:
                if (c.getPriority() == 10) { // from ShowAbout
                    try {
                        Astromaximum.instance.platformRequest("http://" +
                                Astromaximum.URL);
                    } catch (Exception ex) {
                        ex.printStackTrace();
                    }
                } else { // clear logs
                    deleteAll();
                    append(EMPTY, null);
                }
                break;
            case Command.EXIT: // from Astromaximum()
                Astromaximum.quit();
                break;
            case Command.OK: // from Astromaximum()
                try {
                    Astromaximum.options.resetStorage();
                    Astromaximum.options.initDB(true);

                } catch (Exception ex) {
                }
                ;
                Astromaximum.instance.init2();
                break;
        }
    }

    static String decipherPngCodeSection(String str, int param) {
        String ss = "";
        int idx = str.indexOf('.');
        while (idx >= 0) {
            long res = Long.parseLong(str.substring(0, idx), param + 15);
            str = str.substring(idx + 1);
            int i = 8;
            while (res > 0) {
                buf[--i] = (byte) res;
                res >>= 8;
            }
            ss += new String(buf, i, 8 - i);
            idx = str.indexOf('.');
        }

        return ss;
    }
}

// # vi:et:ts=4:sw=4
