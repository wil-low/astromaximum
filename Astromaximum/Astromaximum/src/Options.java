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

//#ifdef build.desktop
//# package com.sw_axis;
//# import java.awt.Frame;
//# 
//# class Options extends Frame{
//#else
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Random;
import java.util.Vector;
import javax.microedition.lcdui.*;
import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
class Options extends GeoList{
  static ChoiceGroup optList;
  static ChoiceGroup timeGap;
  static ChoiceGroup layout;
//#if localtime  
//#   static byte OPT_FLAGS=1;
//#else
      static byte OPT_FLAGS=0;
//#endif  
  static byte optFlags;
  private static long localOffset;
  private String oldc, initCity;
  static final int FLG_ALLTEXT=1;
  static final int FLG_LOCALTIME=2;
  
  Options(){
    //#if "test" @ protection
//#     super(Astromaximum.instance,Choice.EXCLUSIVE,"l.dat");
    //#else
    super(Astromaximum.instance,Choice.EXCLUSIVE,"locations.dat");
    //#endif
    String[] sTimeGap={"-2","-1","0","1","2"};
    timeGap=new ChoiceGroup(LocalizationSupport.getMessage("Correction_hr"),
        Choice.POPUP,sTimeGap,null);
    timeGap.setSelectedIndex(2,true);
    
    String[] sLayout={LocalizationSupport.getMessage("Auto"),"1","2","3"};
    layout=new ChoiceGroup(LocalizationSupport.getMessage("Screen"),
        Choice.POPUP,sLayout,null);

    String[] sOpt={
      LocalizationSupport.getMessage("uat"),
      LocalizationSupport.getMessage("Local_time"),
    };
    optFlags=OPT_FLAGS;
    setTitle(LocalizationSupport.getMessage("Options"));
    setCommandListener(this);
    addCommand(new Command("OK",Command.OK, 1));
    addCommand(new Command(LocalizationSupport.getMessage("Delete_city"),Command.ITEM, 2));
//    addCommand(new Command("Reset storage",Command.ITEM, 3));
    optList=new ChoiceGroup(null,Choice.MULTIPLE,
        sOpt,null);
    insert(0,layout);
    insert(0,timeGap);
    insert(0,optList);
    cityList.setLabel(LocalizationSupport.getMessage("Cities"));
  }
//#if "imeiCheck" @ protection
  static int hj;
//#endif
  private final int IMEI_LEN=15;
//  private final String WARNING="Sorry, your device doesn't match minimal requirements for this application.\n"+
//      "Please check screen dimensions, storage size, memory available or application legalness.";
  /** @noinspection InfiniteLoopStatement*/
  void init() {
//#if MIDP == "2.0"
    cityList.deleteAll();
//#else
//#    try {
//#      while(true)
//#        cityList.delete(0);
//#    } catch (IndexOutOfBoundsException iob) {}
//#endif
    
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
    String[] cities=getAvailableCities();
    String cur="";
//#if "imeiCheck" @ protection
    hj*=new Random().nextInt()*348;
//#endif
      cur=new String(rs.getRecord(1));
//#debug error
      Astromaximum.log(cur);
      for(int i=0; i<cities.length; i++){
        if(cities[i]!=null){
          cityList.append(cities[i],null);
          if(cities[i].equals(cur)){
            cityList.setSelectedIndex(cityList.size()-1,true);
          }
        }
      }
    } 
    catch (Exception ex) {
      Astromaximum.log(ex.toString());
    } 
//    setTitle("IMEI: "+imei.toString());
//    String res="";
//    final String[] ids={
//      "CellID",
//      "phone.mcc",
//      "phone.mnc",
//      "phone.lai",
//      "phone.cid",
//    };
//    String id=null;
//    for(int i=0; i<ids.length; i++){
//      res=System.getProperty(ids[i]);
//      if(res!=null){
//        id=ids[i];
//        break;
//      }
//    }
//    setTitle(id+": "+res);
    

//#endif
  }
  
  public void commandAction(Command c, Displayable d)  {
    if(d!=this){
      if(c.getCommandType()==Command.OK){
        try {
          RecordEnumeration rece=rs.enumerateRecords(this,null,false);
          int nextID=rece.nextRecordId();
          rs.deleteRecord(nextID);
  //            rs.closeRecordStore();
          curCity=oldc.getBytes();
  //            Astromaximum.dataFile.geoposData=initDB(true);
          init();
        } 
        catch (Exception ex) {
        }
      }
      Display.getDisplay(Astromaximum.instance).setCurrent(this);
      return;
    }
    if(c.getCommandType()==Command.CANCEL){
      Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.summary);
    }
    else
    switch(c.getPriority()){
      case 1:
//#debug debug 
        System.out.println("OK");
        optFlags=0;
        for(int i=0; i<optList.size(); i++){
          if(optList.isSelected(i)){
            optFlags+=(1<<i);
          }
        }
//#debug debug 
        System.out.println(optFlags);
        saveHistory();
        curCity=cityList.getString(cityList.getSelectedIndex()).getBytes();
        localOffset=getLocalOffset();
        try{
          rs.setRecord(1,curCity, 0, curCity.length);
//          rs.closeRecordStore();
          initDB(false);
          Astromaximum.summary.changeDay(0);
        }
        catch(Exception e){
          e.printStackTrace();
        }
        if(Astromaximum.summary.size!=layout.getSelectedIndex()){
          Astromaximum.summary.items=null;
          Astromaximum.summary.pageNum=Summary.PAGE_SUMMARY;
          Astromaximum.summary.selItem=1;
          Astromaximum.summary.changeSize();
          Astromaximum.summary.gatherSummary(Astromaximum.summary.date.getTime());
          Astromaximum.summary.setCurPage(Summary.PAGE_SUMMARY);
//          Astromaximum.summary.repaint();
        }
        Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.summary);
        break;
//      case 3:
//        resetStorage();
//        break;
      case 2:
        String sel=cityList.getString(cityList.getSelectedIndex());
        System.out.println(sel);
        if(!sel.equals(curCity) && cityList.size()>1){
          oldc=new String(curCity);
          curCity=sel.getBytes();
          Alert alert=new Alert(LocalizationSupport.getMessage("Confirm"),
              LocalizationSupport.getMessage("Delete_city")+" "+sel+"?",null,
              AlertType.CONFIRMATION);
          alert.addCommand(new Command("OK",Command.OK,1));
          alert.addCommand(new Command(LocalizationSupport.getMessage("Cancel"),
              Command.CANCEL,1));
          alert.setCommandListener(this);
          Display.getDisplay(Astromaximum.instance).setCurrent(alert);
        }
      }
  }
  
  /** @noinspection UnusedParameters*/
  void addImeiChar(Object obj) {
//#debug debug
    Astromaximum.log("App IMEI="+imei);
    String res="";
    if(DataFile.ids==null){
      DataFile.ids=new Vector();
//#if 1==2      
//#       final String[] ids={
//#         "com.sonyericsson.IMEI",
//#         "com.samsung.IMEI",
//#         "com.samsung.imei",
//#         "com.samsungmobile.IMEI",
//#         "com.samsungmobile.imei",
//#         "com.siemens.mp.imei",
//#         "phone.imei",
//#         "phone.IMEI",
//#         "com.nokia.mid.imei",
//#         "device.imei",
//#         "device.IMEI",
//#         "imei",
//#         "IMEI"
//#       };
//#       for(int i=0; i<ids.length; i++){
//#         DataFile.ids.addElement(ids[i]);
//#       }
//#endif      
    }    
    String id="";
    for(int i=0; i<DataFile.ids.size(); i++){
      String ss=(String)DataFile.ids.elementAt(i);
      res=System.getProperty(ss);
      if(res!=null){
        id=ss;
        final StringBuffer sb=new StringBuffer(res);
        for(int j=0; j<sb.length(); j++){
          final char c=sb.charAt(j);
          if(c >= '0' && c <= '9') {
            addImeiChar(c);
          }
        }
        res=sb.toString();
        break;
      }
    }
//#if "imeiCheck" @ protection
    if(res==null) {
  //#if "useMF" @ protection
//#       res = Astromaximum.instance.getAppProperty("MIDlet-Description");
  //#else
          res="";
    }
  //#endif
    try {
	  while(res.length()<IMEI_LEN){
	    res+="0";
	  }
      hj=res.compareTo(imei.toString());
    } 
    catch (NullPointerException npe){
      hj=getHeight();
    }
//    Astromaximum.log("hj="+Integer.toString(hj));
//  //#mdebug debug
     Astromaximum.log(id+": ");
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
  StringBuffer imei;
  
  void addImeiChar(char c){
    if(imei==null) {
      imei = new StringBuffer();
    }
    if(imei.length() < IMEI_LEN) {
      imei.append(c);
//      System.out.print("imeibuf=");
//      System.out.println(imei);
    }
  }
  
  /** @noinspection EmptyCatchBlock,AssignmentToNull,ProhibitedExceptionCaught */
  void addImeiChar(){
    try{
      if(imei.length() != IMEI_LEN) {
        imei = null;
      }
    } catch(NullPointerException npe){}
  }

  protected String getMessage(String string) {
    return LocalizationSupport.getMessage(string);
  }

  void resetStorage() {
    try {
      rs.closeRecordStore();
      rs=null;
      RecordStore.deleteRecordStore(STORE_NAME);
      initDB(true);
      Astromaximum.summary.changeDay(0);
      init();
    } 
    catch (Exception ex) {
      ex.printStackTrace();
    }
  }

  public byte[] initDB(boolean canCreate) throws Exception{
    if(canCreate){
      rs=RecordStore.openRecordStore(STORE_NAME, true, RecordStore.AUTHMODE_ANY, true);
//      rs=RecordStore.openRecordStore(STORE_NAME, "Wiland", "Astromaximum2007");
      if(rs.getNumRecords()==0){ // fill initial city
        byte[] cn;
//#if "timeBomb" @ protection
//#         cn=Astromaximum.getArray();
//#else
        cn=new byte[2];
//#endif
        DataInputStream istr = new DataInputStream(getClass().getResourceAsStream(LOC));
        rs.addRecord(cn,0,1);
        rs.addRecord(cn,0,1);
	istr.skip(2);
        int numRec=istr.readUnsignedShort();
        int rid=-1;
        for(int i=0; i<numRec; i++){
          cn=extractLocation(i);
          try {
            rid=rs.addRecord(cn,0,cn.length);
          }
          catch (RecordStoreException ex) {
            ex.printStackTrace();
          }
//          System.out.print(rid);
//          System.out.println(extractCityName(cn));
        }
        byte[] geo=extractLocation(0);
//        rs.addRecord(geo, 0, geo.length);
        geo=extractCityName(geo).getBytes();
        rs.setRecord(1,geo, 0, geo.length);
//#debug info
        System.out.println("RecStore created");
      }
    }
    Astromaximum.dataFile.geoposData=super.initDB(false);
    return null;
  }

  void saveHistory(){
    ByteArrayOutputStream baos=new ByteArrayOutputStream();
    DataOutputStream dos=new DataOutputStream(baos);
    try {
      dos.writeByte(optFlags);
      dos.writeByte(timeGap.getSelectedIndex());
      dos.writeByte(layout.getSelectedIndex());
      dos.writeShort(Astromaximum.customTime.histCount);
      dos.writeInt(Astromaximum.customTime.lockFlags);
      for(int i=0; i<Astromaximum.customTime.histCount; i++){
        dos.writeLong(Astromaximum.customTime.history[i]);
      }
      dos=null;
      rs.setRecord(2,baos.toByteArray(), 0, baos.size());
//#mdebug info
      System.out.println("history lock");
      System.out.println(Integer.toBinaryString(Astromaximum.customTime.lockFlags));
//#enddebug      
      baos=null;
    } 
    catch (Exception ex) {
      ex.printStackTrace();
    }
  }

  void loadHistory() {
//#debug info 
      System.out.println("Load history");
    try {
      ByteArrayInputStream baos=new ByteArrayInputStream(rs.getRecord(2));
      DataInputStream dis=new DataInputStream(baos);
      optFlags=dis.readByte();
      timeGap.setSelectedIndex(dis.readByte(),true);
      layout.setSelectedIndex(dis.readByte(),true);
      Astromaximum.customTime.histCount=dis.readUnsignedShort();
      Astromaximum.customTime.lockFlags=dis.readInt();
      for(int i=0; i<Astromaximum.customTime.histCount; i++){
        long tt=dis.readLong();
        Astromaximum.customTime.history[i]=tt;
        String str=Event.long2String(tt,0,false);
        if((Astromaximum.customTime.lockFlags&(1<<i))!=0){
          str+="*";
        }
        Astromaximum.customTime.cg.append(str,null);
      }
    } 
    catch (Exception ex) {
      Astromaximum.customTime.histCount=Astromaximum.customTime.lockFlags=0;
      Astromaximum.customTime.cg.deleteAll();
      timeGap.setSelectedIndex(2,true);
      layout.setSelectedIndex(0,true);
      optFlags=OPT_FLAGS;
    }
    for(int i=0; i<optList.size(); i++){
      optList.setSelectedIndex(i,(optFlags&(1<<i))!=0);
    }
    localOffset=getLocalOffset();
//#mdebug info      
      System.out.println(Integer.toBinaryString(Astromaximum.customTime.lockFlags));
//#enddebug      
  }

  static long currentTime(){
    long now=System.currentTimeMillis();
    if((optFlags & FLG_LOCALTIME)!=0){
      now-=Event.localOffset(now);
    }
    return now+localOffset;
  }

  private long getLocalOffset() {
    return Long.parseLong(timeGap.getString(timeGap.getSelectedIndex()))*3600000;
  }
//#endif  
}
