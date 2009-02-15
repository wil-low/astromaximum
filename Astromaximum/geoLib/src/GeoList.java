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
import java.util.Date;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;
import javax.microedition.rms.*;

class GeoList extends Form implements RecordComparator, RecordFilter, CommandListener {

    protected RecordStore rs;
    protected byte[] curCity = null;
    int total;
    int year;
    protected final String STORE_NAME = "Astromaximum";
    protected final String LOC;
    private final MIDlet main;
    static long dstStart;
    static long dstEnd;
    static boolean dstExists;
    static byte[] customData;
    static long tzOffset;
    static boolean isSouthern = false;
    ChoiceGroup cityList;

    GeoList(MIDlet midlet, int type, String loc) {
        super("");
        main = midlet;
        LOC = loc;
//    addCommand(new Command(LocalizationSupport.getMessage("Back"),
//        Command.BACK, 1));
        try {
            DataInputStream dis = new DataInputStream(getClass().getResourceAsStream(LOC));
            year = dis.readShort();
            System.out.println("Year=" + year);
            total = dis.readShort();
            dis.close();
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
//    System.out.println(STORE_NAME);
//        String platform = System.getProperty("microedition.platform");
        String storeName = getStoreName();
        try {
            rs = RecordStore.openRecordStore(storeName, main.getAppProperty("MIDlet-Vendor"),
                    storeName);
        } catch (RecordStoreNotFoundException ex) {
            rs = RecordStore.openRecordStore(storeName, false);
        }
        curCity = rs.getRecord(1);
        RecordEnumeration rece = rs.enumerateRecords(this, null, false);
//#mdebug info
        System.out.println(new String(curCity));
        System.out.println(rece.numRecords());
//#enddebug
        byte[] nextR;
        nextR = rece.nextRecord();
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(nextR));
        dis.skip(4);
        customData = null;
        int count = dis.readUnsignedShort();
        dis.skip(2);
        dis.readUTF();
        isSouthern = false;
        tzOffset = dis.readUnsignedShort();
        dstExists = (tzOffset & (1 << 15)) == 0;
        tzOffset &= (1 << 15) - 1;
        tzOffset -= 16 * 60;
        tzOffset *= 60000L;
        long d_1, d_2;
        if (dstExists) {
            d_1 = dis.readInt() * 60000L - tzOffset;
            d_2 = dis.readInt() * 60000L - tzOffset - 3600000L;
            if (d_1 < d_2) { // N hemisphere
                dstStart = d_1;
                dstEnd = d_2;
            } else {
                dstStart = d_2;
                dstEnd = d_1;
                isSouthern = true;
            }
        if (count > 0) {
           customData = new byte[count];
           dis.read(customData);
        }
//#mdebug info
            System.out.println(dstStart);
            System.out.println(new Date(dstStart).toString());
            System.out.println(dstEnd);
            System.out.println(new Date(dstEnd).toString());
//#enddebug
        }
//        System.out.print("customData=");
//        System.out.println(customData);

//#mdebug info
        System.out.print("TZ offset=");
        System.out.println(tzOffset);
//#enddebug
        byte[] data = new byte[dis.available()];
        dis.read(data);
        dis.close();
//    for(int i=0; i<20; i++){
//      System.out.print(Integer.toHexString(geoposData[i])+" ");
//    }
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
            inputStream.skip(8);
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
        if (s == null) {
            return false;
        }
        return new String(curCity).equals(s);
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
            final DataInputStream dis = new DataInputStream(
                    getClass().getResourceAsStream(LOC));
            dis.skip(4);
            int off = 0;
            for (int i = 0; i < index; i++) {
                off += dis.readShort();
                System.out.println(off);
            }
            final int len = dis.readShort();
            dis.skip(2 * (total - index - 1) + off);
            res = new byte[len + 1];
            dis.read(res);
            dis.close();
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
}

// # vi:et:ts=4:sw=4
