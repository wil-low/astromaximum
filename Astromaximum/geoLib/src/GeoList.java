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
import java.util.Date;
import java.util.TimeZone;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;
import javax.microedition.rms.*;

public class GeoList extends Form implements RecordComparator, RecordFilter, CommandListener{
  protected RecordStore rs;
  protected byte[] curCity=null;
  int total;
  protected final String STORE_NAME="Astromaximum";
  protected final String LOC="locations.dat";
  private MIDlet main;
  static long dstStart;
  static long dstEnd;
  static boolean dstExists;
  static long tzOffset;
  ChoiceGroup cityList;
  
  public GeoList(MIDlet midlet, int type){
    super("");
    main=midlet;
//    addCommand(new Command(LocalizationSupport.getMessage("Back"),
//        Command.BACK, 1));
    addCommand(new Command(getMessage("Cancel"),Command.CANCEL, 1));
    try {
      DataInputStream dis=new DataInputStream(getClass().getResourceAsStream(LOC));
      total=dis.readShort();
      dis.close();
    } 
    catch(IOException e) {
    }
    cityList=new ChoiceGroup(null,type);
    append(cityList);
  }
  
  void init()  {
//#if MIDP == "2.0"
    cityList.deleteAll();
    try {
//#else
//#    try {
//#      while(true)
//#        cityList.delete(0);
//#    } catch (IndexOutOfBoundsException iob) {}
//#endif
      
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
      String[] cities=getAvailableCities();
    } 
    catch (Exception ex) {
    }
    setTitle("Check cities to install:");
//#endif
  }
  
  public void commandAction(Command c, Displayable d){
  }

  public byte[] initDB(boolean canCreate) throws Exception{
    if(rs==null){
      rs=RecordStore.openRecordStore(STORE_NAME, main.getAppProperty("MIDlet-Vendor"), "Astromaximum2007");
    }
    curCity=rs.getRecord(1);
    RecordEnumeration rece=rs.enumerateRecords(this,null,false);
//#mdebug info 
    System.out.println(new String(curCity));
    System.out.println(rece.numRecords());
//#enddebug    
    byte[] nextR;
    nextR = rece.nextRecord();
    DataInputStream dis=new DataInputStream(new ByteArrayInputStream(nextR));
    dis.skip(8);
    dis.readUTF();
    tzOffset=dis.readUnsignedShort();
    dstExists=(tzOffset & (1<<15))==0;
    tzOffset &= (1<<15)-1;
    tzOffset-=16*60;
    tzOffset*=60000L;
    if(dstExists){
      dstStart=dis.readInt()*60000L-tzOffset;//
      dstEnd=dis.readInt()*60000L-tzOffset-3600000L;
//#mdebug info
      System.out.println(dstStart);
      System.out.println(new Date(dstStart).toString());
      System.out.println(dstEnd);
      System.out.println(new Date(dstEnd).toString());
//#enddebug      
    }

//#mdebug info
    System.out.print("TZ offset=");
    System.out.println(tzOffset);
//#enddebug    
    byte[] data=new byte[dis.available()];
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

  
  String[] getAvailableCities() throws Exception{
    String[] cities=null;
    long ET=System.currentTimeMillis();
    RecordEnumeration re = rs.enumerateRecords(null, this, false);
    cities=new String[re.numRecords()];
    for(int i=0; re.hasNextElement(); i++){
      cities[i]=extractCityName(re.nextRecord());
    }
    System.out.println("getAvailableCities="+Long.toString(System.currentTimeMillis()-ET));
    return cities;
  }


  public int compare(byte[] b0, byte[] b1) {
    String cn0=extractCityName(b0);
    String cn1=extractCityName(b1);
    try {
      int cmp=cn0.compareTo(cn1);
      if(cmp<0)
        return RecordComparator.PRECEDES;
      if(cmp>0)
        return RecordComparator.FOLLOWS;
    } 
    catch(Exception e) {
      if(cn0!=null)
        return RecordComparator.PRECEDES;
      if(cn1!=null)
        return RecordComparator.FOLLOWS;
    }
    return RecordComparator.EQUIVALENT;
  }

  String extractCityName(byte[] b) {
    if(b.length<1024){
      return null;
    }
    String name=null;
    try {
      DataInputStream inputStream = new DataInputStream(new ByteArrayInputStream(b));
      inputStream.skip(8);
      name = inputStream.readUTF();
      inputStream.close();
    } 
    catch (Exception ex) {
      return null;
    }
    return name;
  }

  public boolean matches(byte[] b) {
    String s=extractCityName(b);
    if(b==null || s==null){
      return false;
    }
    boolean bb=new String(curCity).equals(s);
    return bb;
  }

  byte[] extractCityNameBytes(byte[] geo) {
    String s=extractCityName(geo);
    if(s==null)
      return null;
    else
      return s.getBytes();
  }

  byte[] extractLocation(int index) {
    byte[] res=null;
    try {
      final DataInputStream dis=new DataInputStream(
          getClass().getResourceAsStream(LOC));
      dis.skip(2);
      int off=0;
      for(int i=0; i<index; i++) {
        off += dis.readShort();
      }
      final int len=dis.readShort();
      dis.skip(2*(total-index-1)+off);
      res=new byte[len+1];
      dis.read(res);
      dis.close();
    } catch (IOException ex) {
//      ex.printStackTrace();
    }
    return res;
  }

  void shutdown(){
    try {
      rs.closeRecordStore();
    } 
    catch (Exception ex) {
    }
    rs=null;
  }


}
