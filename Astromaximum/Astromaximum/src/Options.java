
/**
 * <p>Title: </p>
 *
 * <p>Description: </p>
 *
 * <p>Copyright: Copyright (c) 2003</p>
 *
 * <p>Company: </p>
 *
 * @author not attributable
 * @version 1.0
 */
import java.io.*;
import java.util.Random;
import java.util.Vector;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;

class Options extends GeoList implements CommandListener {

    private static ChoiceGroup optList;
    private static ChoiceGroup timeGap;
    private static ChoiceGroup layout;
    private static ChoiceGroup fontSize;
    private static ChoiceGroup fontFace;
//#if localtime
//#   static byte OPT_FLAGS=1;
    //#else
    private static final byte OPT_FLAGS = 1;
    //#endif
    static byte optFlags;
    private static long localOffset;
    static boolean isRealtimeOff = false;
    private String oldc;
    static final int FLG_ALLTEXT = 1;
    private static final int FLG_LOCALTIME = 2;
    static byte optLayout;
    static byte optFontFace;
    static byte optFontSize;
    static byte optTimeGap = 2;
    final private int[] FONT_SIZE = {Font.SIZE_SMALL, Font.SIZE_MEDIUM, Font.SIZE_LARGE};
    final private int[] FONT_FACE = {Font.FACE_PROPORTIONAL, Font.FACE_MONOSPACE};

    Options() {
        super(Astromaximum.instance, Choice.EXCLUSIVE, Astromaximum.dataFile.getAmaxStream(1));
        String[] sTimeGap = {"-2", "-1", "0", "1", "2"};
        timeGap = new ChoiceGroup(Astromaximum.getstr(118), // Correction_hr
                Choice.POPUP, sTimeGap, null);

        String[] sLayout = new String[Summary.MAX_LAYOUT_NUM + 1];
        sLayout[0] = Astromaximum.getstr(107); // "Auto"
        for (int i = 1; i <= Summary.MAX_LAYOUT_NUM; i++)
            sLayout[i] = Integer.toString(i);
        
        layout = new ChoiceGroup(Astromaximum.getstr(106), // Screen
                Choice.POPUP, sLayout, null);

        optFlags = OPT_FLAGS;
        setTitle(Astromaximum.getstr(92));//Options
        setCommandListener(this);
        addCommand(new Command(Astromaximum.getstr(94), Command.CANCEL, 1));
        Command cmd = new Command("OK", Command.OK, 1);
        addCommand(cmd);
        addCommand(new Command(Astromaximum.getstr(108), Command.ITEM, 2)); // Delete city
        
        String[] sOpt = {
            Astromaximum.getstr(104), // Use all texts
            Astromaximum.getstr(103), // Local time
        };

        optList = new ChoiceGroup(null, Choice.MULTIPLE,
                sOpt, null);
        append(optList);

        append(timeGap);
        append(layout);

        int propW = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN,
                Font.SIZE_MEDIUM).stringWidth(Astromaximum.MainURL);
        int monoW = Font.getFont(Font.FACE_MONOSPACE, Font.STYLE_PLAIN,
                Font.SIZE_MEDIUM).stringWidth(Astromaximum.MainURL);
        if (propW != monoW) {
            sOpt = new String[2];
            for (int i = 0; i < 2; ++i) {
                sOpt[i] = Astromaximum.getstr(170 + i);
            }
            fontFace = new ChoiceGroup(Astromaximum.getstr(167), Choice.POPUP, sOpt, null);
            append(fontFace);
        }
        else {
            // create empty, do not append
            sOpt = new String[1];
            sOpt[0] = "";
            fontFace = new ChoiceGroup(null, Choice.POPUP, sOpt, null);
        }
        sOpt = new String[3];
        for (int i = 0; i < 3; ++i) {
            Font font = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_PLAIN, FONT_SIZE[i]);
            sOpt[i] = Integer.toString(font.getHeight());
        }
        fontSize = new ChoiceGroup(Astromaximum.getstr(168), Choice.POPUP, sOpt, null);
        append(fontSize);
/*
        OK button is disabled (relevant for PocketPC only)
        StringItem strOK = new StringItem("", "OK", Item.BUTTON);
        strOK.setDefaultCommand(cmd);
        strOK.setItemCommandListener(this);
        append(strOK);
*/
        cityList.setLabel(Astromaximum.getstr(105));//Cities
    }    

    //#if "imeiCheck" @ protection
    static int hj;
    //#endif
    private final int IMEI_LEN = 15;

    /**
     * @noinspection InfiniteLoopStatement
     */
    void init() {
        cityList.deleteAll();

        try {
//#if Demo
//#     stringItem1.setText(System.getProperty("com.sun.imei"));
//#     stringItem1.setPreferredSize(112, 24);
//#     this.append(stringItem1);
//#     this.append(dateField1);
//#     this.addCommand(new Command(LocalizationSupport.getMessage("Calendar"),
//#         Command.BACK, 1));
//#     dateField1.setLabel("dateField1");
//#     dateField1.setInputMode(DateField.DATE);
//#     dateField1.setPreferredSize(117, 50);
//#else
            String[] cities = getAvailableCities();
            String cur;
//#if "imeiCheck" @ protection
            hj *= new Random().nextInt() * 348;
//#endif
            cur = new String(rs.getRecord(1));
//#debug error
            Astromaximum.log(cur);
            for (int i = 0; i < cities.length; i++) {
                if (cities[i] != null) {
                    cityList.append(cities[i], null);
                    if (cities[i].equals(cur)) {
                        cityList.setSelectedIndex(cityList.size() - 1, true);
                    }
                }
            }
        } catch (Exception ex) {
            Astromaximum.log(ex.toString());
        }
//#endif
    }

    public void commandAction(Command c, Displayable d) {
        if (d != this) {
            if (c.getCommandType() == Command.OK) { // delete city entrance from Alert
                try {
                    RecordEnumeration rece = rs.enumerateRecords(this, null, false);
                    int nextID = rece.nextRecordId();
                    rs.deleteRecord(nextID);
                    //            rs.closeRecordStore();
                    curCity = oldc;
                    //Astromaximum.dataFile.geoposData=initDB(false);
                    init();
                } catch (Exception ex) {
                }
            }
            Display.getDisplay(Astromaximum.instance).setCurrent(this);
            return;
        }
        if (c.getCommandType() == Command.CANCEL) {
            updateChoices();
            Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.summary);
        } 
        else { // save and return to summary screen
            switch (c.getPriority()) {
                case 1:
//#debug debug
                    System.out.println("OK");
                    optFlags = 0;
                    for (int i = 0; i < optList.size(); i++) {
                        if (optList.isSelected(i)) {
                            optFlags += (1 << i);
                        }
                    }
//#debug debug
                    System.out.println(optFlags);
                    optLayout = (byte)layout.getSelectedIndex();
                    saveHistory();
                    loadCity (cityList.getSelectedIndex());
                    if (Summary.size != optLayout) {
                        Astromaximum.summary.setLayout(optLayout);
                    }
                    Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.summary);
                    break;
//      case 3:
//        resetStorage();
//        break;
                case 2: // delete city command
                    String sel = cityList.getString(cityList.getSelectedIndex());
                    System.out.println(sel);
                    if (!sel.equals(curCity) && cityList.size() > 1) {
                        oldc = curCity;
                        curCity = sel;
                        Alert alert = new Alert(Astromaximum.getstr(148),//Confirm
                                Astromaximum.getstr(108) + " " + sel + "?", null,//Delete_city
                                AlertType.CONFIRMATION);
                        alert.addCommand(new Command("OK", Command.OK, 1));
                        alert.addCommand(new Command(Astromaximum.getstr(97),//Cancel
                                Command.CANCEL, 1));
                        alert.setCommandListener(this);
                        Display.getDisplay(Astromaximum.instance).setCurrent(alert);
                    }
            }
        }
    }

    void loadCity (int index) {
        cityList.setSelectedIndex(index, true);
        String newCity = cityList.getString(index);
        if (!newCity.equals(curCity)) {
            try {
                curCity = newCity;
                rs.setRecord(1, curCity.getBytes(), 0, curCity.length());
    //          rs.closeRecordStore();
                initDB(false);
                System.out.println("LoadCity " + curCity + "!");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        Astromaximum.summary.changeDay(0);
    }
    
    /**
     * @param obj
     * @noinspection UnusedParameters
     */
    void addImeiChar(Object obj) {
        String res = "";
        if (DataFile.ids == null) {
            DataFile.ids = new Vector();
        }
        String id = "";
        for (int i = 0; i < DataFile.ids.size(); i++) {
            String ss = (String) DataFile.ids.elementAt(i);
            res = System.getProperty(ss);
            if (res != null) {
                id = ss;
                final StringBuffer sb = new StringBuffer(res);
                for (int j = 0; j < sb.length(); j++) {
                    final char c = sb.charAt(j);
                    if (c >= '0' && c <= '9') {
                        addImeiChar(c);
                    }
                }
                res = sb.toString();
                break;
            }
        }
//#if "imeiCheck" @ protection
       Astromaximum.log("App IMEI=" + imei);
        if (res == null) {
    //#if "useMF" @ protection
//#       res = Astromaximum.instance.getAppProperty("MIDlet-Description");
    //#else
            res = "";
        }
    //#endif
        try {
            while (res.length() < IMEI_LEN) {
                res += "0";
            }
            hj = res.compareTo(imei.toString());
        }
        catch (NullPointerException npe) {
            hj = getHeight();
        }
//    Astromaximum.log("hj="+Integer.toString(hj));
//  //#mdebug debug
        Astromaximum.log(id + ": ");
        Astromaximum.log(res);
        Astromaximum.log(imei.toString());
        Astromaximum.log(Long.toString(hj));
//  //#enddebug
//       final Alert alert=new Alert("Error",WARNING,null,AlertType.ERROR);
//       alert.addCommand(new Command("OK",Command.OK,1));
//       alert.setTimeout(8000);
//       alert.setCommandListener(Astromaximum.instance);
//       Display.getDisplay(Astromaximum.instance).setCurrent(alert);
//     }
//#endif
    }
    private StringBuffer imei;

    void addImeiChar(char c) {
        if (imei == null) {
            imei = new StringBuffer();
        }
        if (imei.length() < IMEI_LEN) {
            imei.append(c);
//      System.out.print("imeibuf=");
//      System.out.println(imei);
        }
    }

    /**
     * @noinspection EmptyCatchBlock,AssignmentToNull,ProhibitedExceptionCaught
     */
    void addImeiChar() {
        try {
            if (imei.length() != IMEI_LEN) {
                imei = null;
            }
        } catch (NullPointerException npe) {
        }
    }

//  protected String getMessage(String string) {
//    return LocalizationSupport.getMessage(string);
//  }
    void resetStorage() {
        try {
//            int count = rs.getNumRecords();
            RecordEnumeration re = rs.enumerateRecords(null, null, false);
            while(re.hasNextElement()){
                int id = re.nextRecordId();
                if(id>2) // skip first 2 records
                    rs.deleteRecord(id);
            }
//            count = rs.getNumRecords();
            addLocations();
            Astromaximum.summary.changeDay(0);
            init();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    public byte[] initDB(boolean canCreate) throws RecordStoreException, IOException {
        String place = "opt";
        if (canCreate) {
            try {
                String storeName = getStoreName();
                Astromaximum.errCode = 51;
                rs = RecordStore.openRecordStore(storeName,
                        true, RecordStore.AUTHMODE_ANY, true);
                Astromaximum.errCode = 52;
                if (rs.getNumRecords() == 0) {

                    byte[] cn;
//#if "timeBomb" @ protection
                    cn = Astromaximum.getArray();
//#else
//#                     cn = new byte[2];
//#endif
                    rs.addRecord(cn, 0, 1);
                    rs.addRecord(cn, 0, 1);
                    Astromaximum.errCode = 53;
                    addLocations();
//#debug info
                    Astromaximum.log("rs created " + storeName);
                }
            } catch (RecordStoreException ex) {
                Astromaximum.log(place + ex.getMessage());
            } catch (IOException ex) {
                Astromaximum.log(place + ex.getMessage());
            }
        }
                Astromaximum.errCode = 54;
        Astromaximum.dataFile.geoposData = super.initDB(false);
        if (Astromaximum.dataFile.geoposData[0] == 255) {
            Astromaximum.errCode = 540 + Astromaximum.dataFile.geoposData[1];
            throw new NullPointerException();
        }
                Astromaximum.errCode = 80;
        return null;
    }

    void saveHistory() {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        try {
            optTimeGap = (byte)timeGap.getSelectedIndex();
            optFontFace = (byte)fontFace.getSelectedIndex();
            optFontSize = (byte)fontSize.getSelectedIndex();
            setLocalOffset();
            dos.writeByte(optFlags);
            dos.writeByte(optTimeGap);
            dos.writeByte(optLayout);
            dos.writeByte(optFontFace);
            dos.writeByte(optFontSize);
            dos.writeByte(Astromaximum.interpreter.fontSize);
            dos.writeShort(CustomTime.histCount);
            dos.writeInt(Astromaximum.customTime.lockFlags);
            for (int i = 0; i < CustomTime.histCount; i++) {
                dos.writeLong(CustomTime.history[i]);
            }
            rs.setRecord(2, baos.toByteArray(), 0, baos.size());
//#mdebug info
            System.out.println("history lock");
            System.out.println(Integer.toBinaryString(Astromaximum.customTime.lockFlags));
//#enddebug
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        updateChoices();
    }

    void loadHistory() {
//#debug info
        System.out.println("Load history");
        try {
            ByteArrayInputStream bais = new ByteArrayInputStream(rs.getRecord(2));
            DataInputStream dis = new DataInputStream(bais);
            optFlags = dis.readByte();
            optTimeGap = dis.readByte();
            optLayout = dis.readByte();
            optFontFace = dis.readByte();
            optFontSize = dis.readByte();
            Astromaximum.interpreter.fontSize = dis.readByte();
            CustomTime.histCount = dis.readUnsignedShort();
            Astromaximum.customTime.lockFlags = dis.readInt();
            for (int i = 0; i < CustomTime.histCount; i++) {
                long tt = dis.readLong();
                CustomTime.history[i] = tt;
                String str = Event.long2String(tt, 0, false);
                if ((Astromaximum.customTime.lockFlags & (1 << i)) != 0) {
                    str += "*";
                }
                Astromaximum.customTime.cg.append(str, null);
            }
        } catch (Exception ex) {
            CustomTime.histCount = Astromaximum.customTime.lockFlags = 0;
            Astromaximum.customTime.cg.deleteAll();
            optTimeGap = 2;
            optLayout = 0;
            optFontSize = 0;
            optFontFace = 0;
            optFlags = OPT_FLAGS;
            Astromaximum.interpreter.fontSize = Font.SIZE_SMALL;
        }
        updateChoices();
//#mdebug info
        System.out.println(Integer.toBinaryString(Astromaximum.customTime.lockFlags));
//#enddebug
    }

    static long currentTime() {
//#ifdef freetest
//#         if (isRealtimeOff)
//#             return Astromaximum.dataFile.startJD - Astromaximum.MSECINDAY * 40;
//#endif

        long now = System.currentTimeMillis();
        if ((optFlags & FLG_LOCALTIME) != 0) {
            now -= Event.localOffset(now);
            return now;
        }
        return now + localOffset;
    }

    private void setLocalOffset() {
        localOffset = (long)Integer.parseInt(timeGap.getString(optTimeGap)) * 3600000;
    }
    
    private void addLocations() throws RecordStoreException, IOException {
        locStream.reset();
                Astromaximum.errCode = 55;
        locStream.skip(2);
                Astromaximum.errCode = 56;
        int numRec = locStream.readUnsignedShort();
        byte[] cn;
        for (int i = 0; i < numRec; i++) {
            cn = extractLocation(i);
            try {
                rs.addRecord(cn, 0, cn.length);
            } catch (RecordStoreException ex) {
                Astromaximum.errCode = 57;
                ex.printStackTrace();
            }
//          System.out.print(rid);
//          System.out.println(extractCityName(cn));
        }
        byte[] geo = extractLocation(0);
                Astromaximum.errCode = 58;
        geo = extractCityName(geo).getBytes();
                Astromaximum.errCode = 59;
        rs.setRecord(1, geo, 0, geo.length);
    }

    private void updateChoices() {
        setLocalOffset();
        try {
            for (int i = 0; i < optList.size(); i++) {
                optList.setSelectedIndex(i, (optFlags & (1 << i)) != 0);
            }
            timeGap.setSelectedIndex(optTimeGap, true);
            layout.setSelectedIndex(optLayout, true);
            fontSize.setSelectedIndex(optFontSize, true);
            fontFace.setSelectedIndex(optFontFace, true);
        }
        catch (IndexOutOfBoundsException ex){}
    }

    String getCurrentCity(boolean isTrimComma) {
        String s = curCity;
        if (isTrimComma) {
            int pos = s.indexOf(',');
            if (pos > 0)
                s = s.substring(0, pos);
        }
        return s;
    }

    int getFontFace() {
        try {
            return FONT_FACE[optFontFace];
        }
        catch (IndexOutOfBoundsException ex) {
            return Font.FACE_PROPORTIONAL;
        }
    }

    int getFontSize(int idx) {
        try {
            return FONT_SIZE[idx];
        }
        catch (IndexOutOfBoundsException ex) {
            return Font.SIZE_SMALL;
        }
    }

    int getFontSize() {
        return getFontSize(optFontSize);
    }
}

// # vi:et:ts=4:sw=4
