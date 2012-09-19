/*
 * GeoList.java
 *
 * Created on 08 February 2007, 18:09
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

/**
 *
 * @author Administrator
 */
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
//import java.util.Date;
//import java.util.TimeZone;
import java.util.Calendar;
import java.util.Date;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;
import javax.microedition.rms.*;

class GeoList extends Form implements RecordComparator, RecordFilter, CommandListener {

    protected RecordStore rs;
    protected String curCity = null;
    int total;
    int year;
    protected final String STORE_NAME = "Astromaximum";
    protected DataInputStream locStream = null;
    private final MIDlet main;
    static byte[] customData;
    ChoiceGroup cityList;
    static private byte transitionCount;
    static private long[] transitionTimes;
    static private long[] transitionOffsets;
    static private String[] transitionNames;
    static int coords[] = {0, 0, 0};

    GeoList(MIDlet midlet, int type, InputStream loc) {
        super("");
        main = midlet;
        try {
            byte[] buf = new byte[loc.available()];
            loc.read(buf);
            loc.close();
            locStream = new DataInputStream(new ByteArrayInputStream(buf));
        }
        catch (IOException e) {}

        locStream.mark(10000000);
//    addCommand(new Command(LocalizationSupport.getMessage("Back"),
//        Command.BACK, 1));
        try {
            year = locStream.readShort();
//            System.out.println("Year=" + year);
            total = locStream.readShort();
        } catch (IOException e) {
        }
        cityList = new ChoiceGroup(null, type);
        append(cityList);
    }

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
//    setTitle("IMEI: "+imei.toString());
            /*String[] cities = */            getAvailableCities();
        } catch (Exception ex) {
        }
        setTitle("Check cities to install:");
//#endif
    }

    public void commandAction(Command c, Displayable d) {
    }

    byte[] initDB(boolean canCreate) throws RecordStoreException, IOException {
        int errCode = 0;
        byte[] data = null;
        try {
//    System.out.println(STORE_NAME);
//        String platform = System.getProperty("microedition.platform");
        String storeName = getStoreName();
        try {
            rs = RecordStore.openRecordStore(storeName, main.getAppProperty("MIDlet-Vendor"),
                    storeName);
        } catch (RecordStoreNotFoundException ex) {
            rs = RecordStore.openRecordStore(storeName, false);
        }
        errCode = 1;
        curCity = new String(rs.getRecord(1));
        errCode = 2;
        RecordEnumeration rece = rs.enumerateRecords(this, null, false);
//#mdebug info
//#         System.out.println("enumerate " + curCity + ": " + rece.numRecords());
//#enddebug
        byte[] nextR;
        errCode = 3;
        nextR = rece.nextRecord();
        errCode = 4;
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(nextR));
        errCode = 5;
        dis.skip(4); // signature
        byte version = dis.readByte();
        if (version == 2) {
            dis.skip(6); // ymd, day count
            dis.readInt(); // city id
            coords[0] = dis.readShort(); // latitude
            coords[1] = dis.readShort(); // longitude
            coords[2] = dis.readShort(); // altitude
            customData = null;
            dis.readUTF(); // city
            dis.readUTF(); // state
            dis.readUTF(); // country
            dis.readUTF(); // timezone
            dis.readUTF(); // custom data
            transitionCount = dis.readByte();
            transitionTimes = new long[transitionCount];
            transitionOffsets = new long[transitionCount];
            transitionNames = new String[transitionCount];
            for (int i = 0; i < transitionCount; ++i) {
                transitionTimes[i] = dis.readInt(); // start_date
                transitionTimes[i] *= 1000;
                transitionOffsets[i] = dis.readShort(); // gmt_ofs_min
                transitionOffsets[i] *= 60000;
                transitionNames[i] = dis.readUTF(); // name
                System.out.println(transitionTimes[i] + ", " + new Date(transitionTimes[i]) + " > " + transitionOffsets[i] + " " + transitionNames[i]);
            }
        }
        else {
            System.out.println("Unknown version " + version);
        }

//        System.out.print("customData=");
//        System.out.println(customData);

//#mdebug info
//#         System.out.print("TZ offset=");
//#         System.out.println(tzOffset);
//#enddebug
        data = new byte[dis.available()];
        errCode = 6;
        dis.read(data);
        dis.close();
//    for(int i=0; i<20; i++){
//      System.out.print(Integer.toHexString(geoposData[i])+" ");
//    }
        }
        catch (NullPointerException e) {
            data[0] = (byte)255;
            data[1] = (byte)errCode;
        }
        return data;
    }

    String getMessage(String string) {
        return string;
    }

    String getStoreName() {
        return Integer.toString(year).substring(2) + STORE_NAME;
    }

    String[] getAvailableCities() throws Exception {
        String[] cities;
        long ET = System.currentTimeMillis();
        RecordEnumeration re = rs.enumerateRecords(null, this, false);
        cities = new String[re.numRecords()];
        for (int i = 0; re.hasNextElement(); i++) {
            cities[i] = extractCityName(re.nextRecord());
        }
        System.out.println("getAvailableCities=" + Long.toString(System.currentTimeMillis() - ET));
        return cities;
    }

    public int compare(byte[] b0, byte[] b1) {
        String cn0 = extractCityName(b0);
        String cn1 = extractCityName(b1);
        try {
            int cmp = cn0.compareTo(cn1);
            if (cmp < 0) {
                return RecordComparator.PRECEDES;
            }
            if (cmp > 0) {
                return RecordComparator.FOLLOWS;
            }
        } catch (Exception e) {
            if (cn0 != null) {
                return RecordComparator.PRECEDES;
            }
            if (cn1 != null) {
                return RecordComparator.FOLLOWS;
            }
        }
        return RecordComparator.EQUIVALENT;
    }

    String extractCityName(byte[] b) {
        if (b.length < 1024) {
            return null;
        }
        String name;
        try {
            DataInputStream inputStream = new DataInputStream(new ByteArrayInputStream(b));
            inputStream.skip(0x15);
            name = inputStream.readUTF();
            inputStream.close();
        } catch (Exception ex) {
            return null;
        }
        return name;
    }

    public boolean matches(byte[] b) {
        if (b == null) {
            return false;
        }
        String s = extractCityName(b);
        return s != null && curCity.equals(s);
        }

// --Commented out by Inspection START (25.01.09 13:16):
//    byte[] extractCityNameBytes(byte[] geo) {
//        String s = extractCityName(geo);
//        if (s == null) {
//            return null;
//        } else {
//            return s.getBytes();
//        }
//    }
// --Commented out by Inspection STOP (25.01.09 13:16)

    byte[] extractLocation(int index) {
        byte[] res = null;
        try {
            locStream.reset();
            locStream.skip(4);
            int off = 0;
            for (int i = 0; i < index; i++) {
                off += locStream.readShort();
                System.out.println(off);
            }
            final int len = locStream.readShort();
            locStream.skip(2 * (total - index - 1) + off);
            res = new byte[len + 1];
            locStream.read(res);
        } catch (IOException ex) {
//      ex.printStackTrace();
        }
        return res;
    }

    void shutdown() {
        try {
            rs.closeRecordStore();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    static String to2String(long value) {
        String str = Long.toString(value);
        if (str.length() == 1) {
            str = "0" + str;
        }
        return str;
    }

    static String tzOffset2String() {
        StringBuffer result = new StringBuffer();
        for (int i = 0; i < transitionCount; ++i) {
            result.append(new Date(transitionTimes[i]))
                    .append(" > ")
                    .append(transitionNames[i])
                    .append(" ");
            long tzOffset = transitionOffsets[i];
            long absOffsetInMins = Math.abs(tzOffset) / 60000;
            result.append(tzOffset > 0 ? "+" : "-");
            result.append(absOffsetInMins / 60);
            absOffsetInMins %= 60;
            result.append(":").append(to2String(absOffsetInMins)).append("|");
        }
        return result.toString();
    }

    static long getTZoffset(long date0) {
        long offset = 0;
        for (int i = 1; i < transitionCount; ++i) {
            if (transitionTimes[i] >= date0) {
                offset = transitionOffsets[i - 1];
                break;
            }
        }
        return offset;
    }
}

// # vi:et:ts=4:sw=4
