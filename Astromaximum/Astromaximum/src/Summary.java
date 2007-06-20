/**
 * <p>Title: Summary</p>
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
 * @noinspection CastToConcreteClass
 */
//#define imgPhase

//#ifdef build.desktop
//# package com.sw_axis;
//# 
//# import java.awt.Canvas;
//# 
//# class Summary extends Canvas{
//#else
import java.io.*;
import javax.microedition.lcdui.*;
import java.util.*;

class Summary extends Canvas implements CommandListener, Runnable{
//#ifdef UseBuffer
//#   static Image offScreenBuffer;
//#endif
  static int moonPhaseH;
  static private SummItem timerTask;
//  private SummItem prevPH;
  private short[] bounds;
  private short[] _bounds;
  Date date=new Date();
  private static final int BOUNDS_VARS=12;
  static long period0;
  static long period1;
  static int size;
  int selItem;
//  static boolean isCurDay;
  private int previousPage=PAGE_SUMMARY;
  int pageNum;
//#if "imeiCheck" @ protection
  static int hj;
//#endif
//  private boolean fullScreen=true;
  private boolean needRender=true;
//  private Image imgLogo;
  private SummItem statItem;
  private static final byte[] hourSeq={Event.SE_SUN,Event.SE_VENUS,Event.SE_MERCURY,
  Event.SE_MOON,Event.SE_SATURN,Event.SE_JUPITER,Event.SE_MARS};
  SummItem[] items=null;
  long cusTime=0;
//  private boolean firstRun=true;
  static boolean isCurrentDay;
  static boolean isShowCustom=false;
  private static final long DELAY=15*1000;
  private static final long LOGO_DELAY=200;
  private static final byte[] weekStartHour={0,3,6,2,5,1,4};
  private static final byte[] decumbAspects={45,15,30,30,15,45,45,15,30,30,15,45};
  static final byte[] decumbKeys={0,1,2,3,2,1,3,1,2,3,2,1,4};
  static final int PAGE_MONTH=2;//6;
  static final int PAGE_WEEK=1;//7;
  static final int PAGE_PANEL=3;//4;
  static final int PAGE_DECUMB=0;//5;
  static final int PAGE_SUMMARY=4;
  static final int PAGE_HELP=8;
  //#ifdef ELECTIO
//#   static final int PAGE_ELECTIO=9;
  //#endif
  static int PAGE_LAST;
  static int IMG_HEIGHT;
  static int IMG_WIDTH;
  static Image imgPlanet;
  static Image imgZodiac;
  static Image imgAspect;
  static Image imgPanel;
  static Image imgService;
  static Image imgOpaq;
  Vector moonPhase;
  Event[] aNavroz=new Event[2];
  private Command[] cmds=new Command[8];
  static Image imgPanelSmall;
  /**
   * Summary
   *
   *
   *
   */
  Summary(String file, int frames, int progr) {
    progress = progr;
    frameCount = frames;
//#ifdef imgPhase 
    moonFile = file;
    img = Astromaximum.extractImg(0, file);
//#endif    
//    try {
//      imgLogo=Image.createImage("/res/logo.png");
//    } 
//    catch (IOException ex) {
//    }
    selItem=0;
//    prevPH=new SummItem(Event.EV_LAST);
    SummItem.owner=this;
    try {
      imgPanelSmall =Image.createImage("/res/panel2.png");
    } 
    catch (IOException ex) {
    }
//#if MIDP=="2.0"
    setFullScreenMode(true);
//#else
//#endif
    createCommands(0);
    setCommandListener(this);
    pageNum=PAGE_SUMMARY;
  }
  
  /**
   * paint
   *
   * @param graphics Graphics
   */
  protected void paint(Graphics graphics) {

//#ifdef UseBuffer    
//#     if(goon){
//#       super.paint(graphics);
//#     } else{
//#       if(needRender) {
//#         render(offScreenBuffer.getGraphics());
//#       }
//#       graphics.drawImage(offScreenBuffer, 0, 0, Graphics.LEFT | Graphics.TOP);
//#     }
//#else
    if(goon){
      final int x=graphics.getClipX();
      final int y=graphics.getClipY();
      final int w=graphics.getClipWidth();
      final int h=graphics.getClipHeight();
//#ifdef imgPhase
      graphics.setClip(moonX,moonY,img.getWidth(),img.getHeight());
      graphics.drawImage(img,moonX,moonY,Graphics.LEFT|Graphics.TOP);
//#else
//#       graphics.setClip(moonX,moonY,width,width);
//#       graphics.fillArc(moonX,moonY,moonX,moonY,-90,180);
//#endif      
      graphics.setClip(x,y,w,h);
//        graphics.setColor(0);
//        graphics.fillRect(0,0,getWidth(),getHeight());
//        graphics.drawImage(imgLogo,getWidth()/2,getHeight()/2,Graphics.HCENTER|Graphics.VCENTER);
    } 
    else{
      
      if(needRender) {
//#if midp2y2007notest
//# //        DataFile.hj*=(selItem%3+1);
//# //        System.err.println("hj = " + DataFile.hj );
//#endif
        render(graphics);
      }
//      graphics.drawImage(Astromaximum.offScreenBuffer, 0, 0, Graphics.LEFT | Graphics.TOP);
    }
//#endif
  }
  
  /**
   * render
   */
  private void render(Graphics osg) {
/*    osg.setColor(Astromaximum.BACK_COLOR);
    osg.fillRect(0,0,getWidth(),getHeight());
    drawPhase(osg,10,10,10,0);
    drawPhase(osg,70,10,10,1);
    drawPhase(osg,10,70,10,2);
    drawPhase(osg,70,70,10,3);*/
    
    final long now=Options.currentTime();
    if(pageNum == Summary.PAGE_MONTH || pageNum == Summary.PAGE_WEEK) {
      osg.setColor(Astromaximum.CURRENT_MONTH_COLOR);
    } 
    else {
      osg.setColor(Astromaximum.BACK_COLOR);
    }
//    osg.setFont(Astromaximum.sizer.getFont());
    osg.fillRect(0,0,getWidth(),getHeight());
    statItem.initString();
    osg.setColor(0xff0000);
    osg.setFont(Font.getFont(Font.FACE_PROPORTIONAL,Font.STYLE_PLAIN,Font.SIZE_SMALL));
//    isCurDay=(now>=period0)&&(now<period1);
    for(int i=0; i<items.length; i++) {
      if (items[i] != null && i != selItem && items[i].isOnPage()) {
        items[i].render(osg, false, now, isShowCustom);
      }
    }
    
    items[selItem].render(osg,true,now,isShowCustom);
  }
  
  /**
   * keyPressed
   *
   * @param keyCode int
   */
  protected void keyReleased(int keyCode) {
    final int ga=getGameAction(keyCode);
    SummItem si=getSelectedItem();
    switch (ga) {
      case Canvas.FIRE:
        selectSummItem(si,false);
        return;
      case Canvas.LEFT:
        keyNavigate(0);
        break;
      case Canvas.RIGHT:
        keyNavigate(1);
        break;
      case Canvas.UP:
        keyNavigate(2);
        break;
      case Canvas.DOWN:
        keyNavigate(3);
        break;
      default:
        switch (keyCode){
          case Canvas.KEY_NUM1:
//            changeDay(-1);
            moveFocus(1,1);
            break;
//          case KEY_NUM3:
//            changeDay(1);
//            break;
//#mdebug info
          case Canvas.KEY_STAR:
            si.dump();
            break;
//#enddebug
          case Canvas.KEY_NUM9:
            if(pageNum==PAGE_DECUMB){
              return;
            }
            int pn=pageNum+1;
            if(pageNum==PAGE_HELP){
              pn=PAGE_SUMMARY;
            }
            else{
              if(pn > PAGE_LAST) {
                pn = PAGE_WEEK;
              }
            }
            setCurPage(pn);
            break;
          case Canvas.KEY_NUM7:
            if(pageNum==PAGE_WEEK && date.getTime()!=selDate.getTime()){
              System.out.println(date);
              System.out.println(selDate);
              showDaySummary();
              return;
            }
            if(pageNum==PAGE_DECUMB){
              return;
            }
            pn=pageNum-1;
            if(pageNum==PAGE_HELP){
              pn=PAGE_SUMMARY;
            }
            else{
              if(pn < PAGE_WEEK) {
                pn = PAGE_LAST;
              }
            }
            setCurPage(pn);
            break;
          case Canvas.KEY_NUM0:
            if((pageNum>=PAGE_SUMMARY && pageNum<=PAGE_LAST)|| pageNum==PAGE_PANEL){
              SummItem sip=getItem(Event.EV_PANEL);
              selectSummItem(sip,false);
              if(sip.isOnPage()){
                selItem=0;
              }
            }
            else{
              showMoonIngress();
            }
            break;
          case Canvas.KEY_POUND:
            Astromaximum.logBox.showLog(this);
            break;
        }
    }
  }
  
  void showMoonIngress(){
    if(pageNum==PAGE_WEEK && getSelectedItem().type==Event.EV_WEEK_GRID){
      long p0=selDate.getTime();
      p0-=Event.localOffset(p0);
      long p1=p0+Astromaximum.MSECINDAY-1;
//#mdebug debug
      System.out.println("period0="+Event.long2String(p0,0,false));
      System.out.println("period1="+Event.long2String(p1,0,false));
      Astromaximum.evDump(mIngress);
//#enddebug

      for (Enumeration e = mIngress.elements() ; e.hasMoreElements() ;) {
        final Event ev=(Event)e.nextElement();
//        final Event ev=Astromaximum.evAt(mIngress,i);
//        ev.dump();
//        if(ev.planet0==Event.SE_MOON && ev.isDateBetween(0, p0, p1)){
        if(ev.planet0==Event.SE_MOON && ev.date1>=p1){
          SummItem si=new SummItem(Event.EV_MOON_SIGN_LARGE);
          si.events=new Event[1];
          si.setEvents(0,ev);
//          System.out.println(Event.long2String(ev.date0,0,false));
//          System.out.println(Event.long2String(ev.date1,0,false));
//          si.dump();
          selectSummItem(si,true);
          break;
        }
      }
    }
  }
  /**
   * changeDay
   *
   * @param delta int
   */
  long changeDay(int delta) {
    final long tick=Options.currentTime();
    long tmp=Astromaximum.instance.changeDate(date,delta);
    if(tmp!=0){
      gatherSummary(tmp);
      setCurPage(pageNum);
//      repaint();
    }
    return Options.currentTime()-tick;
  }
  
  /**
   * daySummary
   * 
   * 
   * @noinspection AssignmentToForLoopParameter,ValueOfIncrementOrDecrementUsed,ProhibitedExceptionCaught
   * @param date0 
   */
  void gatherSummary(long date0) {
    final long tick=Options.currentTime();
    rowCount=1;
    date.setTime(date0);//new Date(date.getTime());
    period0 =date.getTime();
    period0-=Event.localOffset(period0);
//    System.out.println(period0);
//    System.out.println(new Date(period0).toString());
    period1 = period0 +Astromaximum.MSECINDAY-1;
    SummItem si=getItem(Event.EV_ECLIPSE,1);
    si.tag=1;
    SummItem si0=getItem(Event.EV_ECLIPSE,0);
    si0.tag=0;
    si0.events[0]=si.events[0]=null;
    for(Enumeration e= moonPhase.elements(); e.hasMoreElements();){
      Event ph=(Event)e.nextElement();
      if(ph.isDateBetween(0,period0,period1)){
        si.events[0]=ph;
        si.tag+=2;
        break;
      }
    }
    Event todayEclipse = Astromaximum.dataFile.todayEclipse(period0,3);
    if(todayEclipse!=null){
      si=getItem(Event.EV_ECLIPSE,todayEclipse.planet0);
      si.events[0]=todayEclipse;
      si.tag|=4;
    }
    for(int i=moonPhase.size()-1; i>=0; i--){
      Event ph=Astromaximum.evAt(moonPhase,i);
      if(ph.date0<period1){
        getItem(Event.EV_MOON_PHASE,0).events[0]=ph;
        break;
      }
    }
    isCurrentDay = isInCurrentDay(tick);
    Astromaximum.calendar.setTime(new Date((period0 + period1)/2));
    final int weekDay= Astromaximum.calendar.get(Calendar.DAY_OF_WEEK);
//****** week day
    getItem(Event.EV_WEEK).setEvents(1,new Event(date.getTime(), weekDay));
//****** VOC
    getItem(Event.EV_VOC).setEvents(0, Astromaximum.dataFile.getEventOnPeriod(
        Event.EV_VOC,Event.SE_MOON,false, period0, period1));
//****** VIA COMBUSTA
    getItem(Event.EV_VIA_COMBUSTA).setEvents(0, Astromaximum.dataFile.getEventOnPeriod(
        Event.EV_VIA_COMBUSTA,Event.SE_MOON,false, period0, period1));
//#if logger
//#       Astromaximum.instance.logger(" VC");
//#endif      
//****** SUN & MOON RISES & SETS
    for(int i=Event.SE_SUN; i <= Event.SE_MOON; i++){
      Event eop= Astromaximum.dataFile.getEventOnPeriod(Event.EV_RISE,i,true, period0, period1);
      if(eop==null || eop.date0<period0) {
        eop = new Event(0, i);
      }
      getItem(Event.EV_SUN_RISE+i).setEvents(0,eop);
      eop= Astromaximum.dataFile.getEventOnPeriod(Event.EV_SET,i,false, period0, period1);
      if(eop==null || eop.date0<period0) {
        eop = new Event(0, i);
      }
      getItem(Event.EV_SUN_RISE+i).events[0].date1=eop.date0;
    }
//#if logger
//#       Astromaximum.instance.logger(" SO,MO riseset");
//#endif      
    
    int pltDaySun;
//****** SUN DEGREE PASS
    getItem(Event.EV_SUN_DEGREE_LARGE).setEvents(0, Astromaximum.dataFile.getEventOnPeriod(
        Event.EV_DEGREE_PASS,Event.SE_SUN,true, period0, period1));
//#if logger
//#       Astromaximum.instance.logger(" SO degpass");
//#endif      
//****** MOON SIGN ENTER
    getItem(Event.EV_MOON_SIGN_LARGE).setEvents(0, Astromaximum.dataFile.getEventOnPeriod(
        Event.EV_SIGN_ENTER,Event.SE_MOON,true, period0, period1));
//#if logger
//#       Astromaximum.instance.logger(" MO signenter");
//#endif      
    
//    Astromaximum.evDump(vNavroz);
    long navroz=aNavroz[1].date0;
    final long sunrise=getItem(Event.EV_SUN_RISE).events[0].date0;
    if(sunrise<navroz) {
      navroz=aNavroz[0].date0;
    } 
    pltDaySun = (int) ((sunrise - navroz)*1000 / Astromaximum.MSECINDAY+500)/1000;
    if(pltDaySun < 360) {
      pltDaySun = Astromaximum.getSignDegree(pltDaySun);
    }
//****** SUN DAY
    Event ev=new Event(Astromaximum.dataFile.getEventOnPeriod(
        Event.EV_RISE,Event.SE_SUN,true, period0, period1));
    ev.setDegree(pltDaySun);
//    ev.date1=getItem(Event.EV_SUN_RISE).events[1].date1;
//    System.out.println(pltDaySun);
//    ev.dump();
    getItem(Event.EV_SUN_DAY).setEvents(0,ev);
//****** MOON DAY
    final Vector mdd=new Vector();
    Astromaximum.dataFile.getEventsOnPeriod(mdd,Event.EV_RISE,Event.SE_MOON,false,
        period0, period1,0);
//    Astromaximum.dataFile.getEventsOnPeriod(vNavroz,Event.EV_STATUS,Event.SE_MOON,false,
//        period0, period1,1);
//    mergeEvents(mdd,vNavroz,true);
    getItem(Event.EV_MOON_DAY).setEvents(mdd);
//****** TITHI
    final Vector tith=new Vector();
//    System.out.println("Tithi!");
    Astromaximum.dataFile.getEventsOnPeriod(tith,Event.EV_TITHI,Event.SE_MOON,
        false, period0, period1,0);
//    Astromaximum.evDump(tith);
//#if logger
//#       Astromaximum.instance.logger("tithi found="+Integer.toString(tith.size()));
//#endif      
    getItem(Event.EV_TITHI).setEvents(tith);
//#if logger
//#       Astromaximum.instance.logger(" tithi");
//#endif      
    
//****** MOON ASPECTS
    final Vector asp=new Vector();
    SummItem.moonMoveVec.removeAllElements();
    
    Astromaximum.dataFile.getAspectsOnPeriod(SummItem.moonMoveVec,Event.SE_MOON,
        period0 -Astromaximum.MSECINDAY*2, period1 +Astromaximum.MSECINDAY*2);
    
//****** ASPECTS
    Astromaximum.dataFile.getAspectsOnPeriod(asp,-1,
        period0 -Astromaximum.MSECINDAY, period1 +Astromaximum.MSECINDAY);
    
    getItem(Event.EV_ASP_EXACT).setEvents(evInCurrentDay(new Vector(),asp));
//#if logger
//#       Astromaximum.instance.logger(" aspExact");
//#endif      
    
    asp.removeAllElements();
    Astromaximum.dataFile.getEventsOnPeriod(asp,Event.EV_SIGN_ENTER,Event.SE_MOON,
        true, period0 -Astromaximum.MSECINDAY*2, period1 +Astromaximum.MSECINDAY*2,0);
    for (Enumeration e = asp.elements() ; e.hasMoreElements() ;){
      ev=(Event)e.nextElement();
      ev.planet1=Event.SE_MOON;
      ev.date1=ev.date0;
    }
    mergeEvents(SummItem.moonMoveVec,asp,true);
    asp.removeAllElements();
    mergeEvents(asp,SummItem.moonMoveVec,false);
    
    int id1=-1;
    int id2=-1;
    int counter=0;
    for (Enumeration e = asp.elements() ; e.hasMoreElements() ;) {
      ev=(Event)e.nextElement();
      final long dat=ev.date0;
      if(dat< period0){
        id1=counter;
      }
      if(id2 == -1 && dat >= period1){
        id2=counter;
      }
      ++counter;
    }
    asp.setSize(id2+1);
    try {
      for(int i=0; i<id1; i++) {
        asp.removeElementAt(0);
      }
    } catch(ArrayIndexOutOfBoundsException aie){
    }
    int sz=asp.size();
    int idx=1;
    for(int i=0; i<sz-1; i++){
      final long dd=Astromaximum.evAt(asp,idx-1).date1;
      ev=new Event(dd,-1);
      ev.setDegree(200);
      ev.date1=Astromaximum.evAt(asp,idx).date0;
      ev.planet0=Astromaximum.evAt(asp,idx-1).planet1;
      ev.planet1=Astromaximum.evAt(asp,idx).planet1;
//      ev.dump();
      asp.insertElementAt(ev,idx);
      idx+=2;
    }
    
    getItem(Event.EV_MOON_MOVE).setEvents(asp);
//#if logger
//#       Astromaximum.instance.logger(" moonMove");
//#endif      
    
    final Vector retrograde=new Vector();
    final Vector workVec=new Vector();
//****** SELECTED DEGREES & RETROGRADE
    for(int i=Event.SE_SUN; i <= Event.SE_PLUTO; i++){
      Astromaximum.dataFile.getEventsOnPeriod(workVec,Event.EV_DEGREE_PASS,i,
          false, period0, period1,0);
      Astromaximum.dataFile.getEventsOnPeriod(retrograde,Event.EV_RETROGRADE,i,
          false, period0, period1,0);
    }
    for(int i=0; i<workVec.size(); i++) {
      if (Astromaximum.evAt(workVec, i).getDegType() == 0) {
        workVec.removeElementAt(i--);
      }
    }
    asp.removeAllElements();
//    Astromaximum.evDump(workVec);
    mergeEvents(asp,workVec,true);
    getItem(Event.EV_SEL_DEGREES).setEvents(asp);
//#if logger
//#       Astromaximum.instance.logger(" selDegree");
//#endif      
    workVec.removeAllElements();
    for(int i=Event.SE_SUN; i <= Event.SE_PLUTO; i++){
      Astromaximum.dataFile.getEventsOnPeriod(workVec,Event.EV_RETROGRADE,i,
          false, period0, period1,0);
    }
    getItem(Event.EV_RETROGRADE).setEvents(workVec);
//#if logger
//#       Astromaximum.instance.logger(" retrograde");
//#endif      

//******* common risesets
    long pp0=period0 -Astromaximum.MSECINDAY, pp1=period1 +Astromaximum.MSECINDAY;
    for(int i=Event.SE_SUN; i <= Event.SE_WHITE_MOON; i++){
      Vector tmp2=new Vector();
      if(i == Event.SE_MOON) {
        Astromaximum.dataFile.getEventsOnPeriod(tmp2,
            Event.EV_SIGN_ENTER, i, false, period0, period1, 0);
      } 
      else {
        Astromaximum.dataFile.getEventsOnPeriod(tmp2,
            Event.EV_DEGREE_PASS, i, false, period0, period1, 0);
      }
      getItem(Event.EV_DEG_2ND,i).setEvents(tmp2);
      if(i > Event.SE_SATURN) {
        continue;
      }
      final Vector tmp=new Vector();
      tmp2.removeAllElements();
      Astromaximum.dataFile.getEventsOnPeriod(tmp,Event.EV_ASTRORISE,i,
          false, pp0, pp1,0);
      for(int j=0; j<tmp.size(); j++){
        ev=Astromaximum.evAt(tmp,j);
        if(i==Event.SE_MOON && ev.getDegree()==1){
          tmp.removeElement(ev);
//          System.out.println("Removed: ");
//          ev.dump();
          --j;
        }
        else{
          ev.date1=ev.date0;
          ev.setDegree(1);
        }
      }
//      System.out.println("Astrorise");
//      Astromaximum.evDump(tmp);
//      if(pp0>0)
//        continue;
      Astromaximum.dataFile.getEventsOnPeriod(tmp,Event.EV_ASTROSET,i,
          false, pp0,pp1,3);
      mergeEvents(tmp2,tmp,true);
//      Astromaximum.evDump(tmp2);
      tmp.removeAllElements();
      sz=tmp2.size();
      for(int j=0; j<sz-1; j++){
        ev=Astromaximum.evAt(tmp2,j);
        final Event enew=new Event((Astromaximum.evAt(tmp2,j+1).date0+ev.date0)/2,i);
        enew.setDegree(ev.getDegree() == 1 ? 2 : 4);
        tmp2.addElement(enew);
      }
//      Astromaximum.evDump(tmp2);
      mergeEvents(tmp,tmp2,true);
//      Astromaximum.evDump(tmp);
//      break;
      for(Enumeration e=tmp.elements(); e.hasMoreElements();){
        ev=(Event)e.nextElement();
        if(ev.getDegree() == 200) {
          tmp.removeElement(ev);
        }
      }
      
      tmp2= evInCurrentDay(new Vector(),tmp);
      getItem(Event.EV_RISE,i).setEvents(tmp2);
//#if logger
//#       Astromaximum.instance.logger(" other risesets");
//#endif      
//      System.out.println("EvDump:");
//      Astromaximum.evDump(tmp2);
    }
///////////////////////////////////////
//#if logger
//#       Astromaximum.instance.logger("end risesets");
//#endif      
    
//****** PLANETARY HOURS
    ev= Astromaximum.dataFile.getEventOnPeriod(Event.EV_RISE,Event.SE_SUN,true,
      period0 +Astromaximum.MSECINDAY, period1 +Astromaximum.MSECINDAY);
//    System.out.println("PH dump:");
//    getItem(Event.EV_SUN_RISE).events[0].dump();
//    ev.dump();
    Event[] aev=calcPlanetHours(getItem(Event.EV_SUN_RISE).events[0],ev,weekStartHour[weekDay -1]);
    for(int i=0; i<24; i++){
      getItem(i < 12 ? Event.EV_DAY_HOURS : Event.EV_NIGHT_HOURS).setEvents(i%12,aev[i]);
    }
//#if logger
//#       Astromaximum.instance.logger(" planetHours");
//#endif      
//    long pp0=period0 + Astromaximum.MSECINDAY, pp1=period1 + Astromaximum.MSECINDAY;
//    ev= Astromaximum.dataFile.getEventOnPeriod(Event.EV_RISE,Event.SE_SUN,true,
//      pp0, pp1);
//    ev.date1=Astromaximum.dataFile.getEventOnPeriod(Event.EV_SET,Event.SE_SUN,false,
//      pp0, pp1).date0;
////        ev.dump();
//    aev=calcPlanetHours(ev,getItem(Event.EV_SUN_RISE).events[0],weekStartHour[(weekDay+1)%7]);
//    prevPH.setEvents(aev);
//    prevPH.recalcSelection(period0, true);
//    prevPH.dump();
    if(cusTime==0){
      si=getItem(Event.EV_RISE,Event.SE_SUN);
      for(int i=0; i<si.events.length; i++){
        if(si.events[i].getDegree()==2){
          long tm=si.events[i].date0;
          tm+=Event.localOffset(tm);
          Astromaximum.customTime.timeField.setDate(new Date(
             tm% Astromaximum.MSECINDAY));
          break;
        }
      }
    }
//#if logger
//#       Astromaximum.instance.logger("before setTime");
//#endif      
    Astromaximum.customTime.setTime(false);
//#if logger
//#       Astromaximum.instance.logger("after setTime");
//#endif      
    final String et="gET="+Long.toString(Options.currentTime()-tick);
  }
  
  private final String title = "";
// --Commented out by Inspection START (1/12/07 1:48 PM):
//  /**
//   * period2string
//   *
//   * @param period byte[]
//   * @return String
//   */
//  static String period2string(long period) {
//    return new Date(period).toString();
///*    String s="";
//    s+=Long.toString(period)+"-";
//    return s;*/
//  }
// --Commented out by Inspection STOP (1/12/07 1:48 PM)
  /**
   * sizeChanged
   *
   * @param w int
   * @param h int
   */
  void changeSize() {
    recalcBounds(getWidth(),getHeight());
//    Astromaximum.log("Bounds="+Integer.toString(bounds.length));
    if(items==null){
      items=new SummItem[bounds.length/ BOUNDS_VARS];
      for(int i=0; i<items.length; i++) {
        items[i] = new SummItem(
            border(i, 0), border(i, 1), border(i, 2), border(i, 3),
            border(i, 4), border(i, 5), border(i, 6),
            border(i, 7),
            border(i, 8), border(i, 9), border(i, 10), border(i, 11));
      }
      
//#if logger
//#       Astromaximum.instance.logger(" si created");
//#endif      
      statItem=getItem(Event.EV_STATUS);
//      getItem(Event.EV_ZODIAC_SIGN).initString();
    }
//    Astromaximum.log("size change");
    if(pageNum == Summary.PAGE_MONTH){
      rowCount=6; colCount=7;
    } 
    else{
      rowCount=7; colCount=1;
    }
  }
  
  /**
   * pointerPressed
   *
   * @param x int
   * @param y int
   * @noinspection AssignmentToMethodParameter
   */
  protected void pointerPressed(int x, int y) {
    int oldSelection=selItem;
    int oldEvent=-1;
    SummItem si=null;
    
    for(int i=0; i<items.length; i++){
      SummItem it=items[i];
      oldEvent=it.selIndex;
      if(it != null && it.isOnPage() && !it.isEmpty() && it.checkSelection(x, y)){
        si=it; selItem=i;
        break;
      }
    }
    if(si==null) {
      return;
    }
    final int sind=si.selIndex;
    switch(si.type){
      case Event.EV_GRID_DATE:
        if(sind != 1){
          keyNavigate(sind==0? 0: 1);
          return;
        }
        break;
      case Event.EV_WEEK:
        if(sind != 1){
          changeDay(sind-1);
          si.selIndex=1;
          //#ifdef ELECTIO
//#           if(pageNum==PAGE_ELECTIO){
//#             calcElectio();
//#           }
          //#endif
          repaint();
          return;
        }
        break;
      case Event.EV_WEEK_GRID:
      case Event.EV_MONTH_GRID:
        y-=si.top;
        x-=si.left;
        final int ss=x*colCount/si.width+y*rowCount/si.height*colCount;
        moveDay(ss-selCell,true);
        repaint();
        if(pageNum==PAGE_WEEK && x<IMG_WIDTH*2){
          showMoonIngress();
          return;
        }
        if(ss == selCell) {
          showDaySummary();
        } 
        return;
    }
//#mdebug debug
//    System.out.print("Oldsel ");
//    System.out.println(oldSelection);
//#enddebug    
    if(oldSelection == selItem && sind == oldEvent) {
      selectSummItem(si,false);
    } 
    else{
      si.prepareTithi();
      repaint();
    }
  }
  
  private void selectSummItem(SummItem si, boolean ignoreAllTopics) {
    switch(si.type){
      case Event.EV_STATUS:
        if(si.selIndex==0){
          Astromaximum.customTime.init(pageNum);
        }
        else{
          isShowCustom=!isShowCustom;
          recalcAllSelections();
          repaint();
        }
        break;
      case Event.EV_PANEL:
        int pn;
        if(pageNum == PAGE_PANEL){
          pn=previousPage;
        } 
        else{
          previousPage=pageNum;
          pn=PAGE_PANEL;
        }
        setCurPage(pn);
        break;
      case Event.EV_FAST_BUTTON:
        switch(si.tag){
          case 7: // decumbiture
            Astromaximum.customTime.init(pageNum);
            return;
          default:
            Astromaximum.interpreter.topic=si.tag;
        }
        setCurPage(PAGE_SUMMARY);
        break;
      case Event.EV_MONTH_GRID:
      case Event.EV_WEEK_GRID:
        if(Astromaximum.dataFile.isDateAvailable(selDate)){
          showDaySummary();
        }
        break;
      case Event.EV_SUN_RISE:
      case Event.EV_MOON_RISE:
        setCurPage(PAGE_SUMMARY+1);
        break;
      case Event.EV_ASP_EXACT:
        if(si.events.length==0){
          setCurPage(PAGE_WEEK);
          break;
        }
      default:
        if((Interpreter.topic==10) && ((Options.optFlags & Options.FLG_ALLTEXT)!=0)){
          ignoreAllTopics=true;
        }
        if(Astromaximum.interpreter.findText(si,ignoreAllTopics)) {
          Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.interpreter);
        }
        break;
    }
  }
  
  String getSummaryTitle() {
    return title;
  }
  
  
  /**
   *
   * @param delta
   */
  void moveFocus(int delta, int dir) {
    SummItem si;
    do{
      selItem+=delta;
      if(selItem>items.length-1) {
        selItem = 0;
      }
      if(selItem < 0) {
        selItem = items.length - 1;
      }
      si=items[selItem];
//      delta=(dir&1)==0? 1: -1;
      delta=delta>0? 1: -1;
    }while(!si.isOnPage());
    
    if(si.type==Event.EV_DAY_HOURS || si.type==Event.EV_NIGHT_HOURS){
      si.selIndex=5;
    }

//    si.selIndex=(dir>0)? 0: si.events.length-1;
//    repaint();
  }
  
//  protected void showNotify() {
//    setFullScreenMode(fullScreen);
//  }
//
  private SummItem getItem(int tp) {
    for(int i=0; i<items.length; i++) {
      if (items[i].type == tp) {
        return items[i];
      }
    }
    return null;
  }
  
  /** @noinspection AssignmentToMethodParameter*/
  private SummItem getItem(int tp, int index) {
    for(int i=0; i<items.length; i++) {
      if (items[i].type == tp) {
        if (index <= 0) {
          return items[i];
        }
        --index;
      }
    }
    return null;
  }
  
  /**
   *
   * @return
   */
  SummItem getSelectedItem() {
    return items[selItem];
  }
  
  void setCustomTime(int h, int m) {
    period0 =date.getTime();
    period0-=Event.localOffset(period0);
    period1 = period0 +Astromaximum.MSECINDAY-1;
    Astromaximum.calendar.setTime(new Date((period0 + period1)/2));
    Astromaximum.calendar.set(Calendar.HOUR_OF_DAY,h);
    Astromaximum.calendar.set(Calendar.MINUTE,m);
    cusTime=Astromaximum.calendar.getTime().getTime();
    cusTime-=Event.localOffset(cusTime);
    recalcAllSelections();
  }
  
  static boolean isInCurrentDay(long date) {
    return date >= period0 && date <= period1;
  }
  
  /**
   *
   * @param c
   * @param d
   */
  public void commandAction(Command c, Displayable d) {
    if(pageNum==PAGE_DECUMB){
      isShowCustom=false;
    }
    switch(c.getPriority()){
      case 0:
        setCurPage(Summary.PAGE_HELP);
//        Astromaximum.summary.dontRender();
        break;
      //#ifdef ELECTIO
//#       case 7:
//#         calcElectio();
//#         setCurPage(Summary.PAGE_ELECTIO);
//#         break;
      //#endif
      case 2:
      case 3:
        setCurPage(c.getPriority() == 2 ? Summary.PAGE_WEEK : Summary.PAGE_MONTH);
//        Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.instance.summary);
        break;
      case 6:
        Interpreter.topic=10;
        if(pageNum==PAGE_DECUMB){
          setCurPage(PAGE_SUMMARY);
          createCommands(0);
        }
        repaint();
        break;
      case 4:
        Astromaximum.options.init();
        Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.options);
        break;
      case 1:
        selDate.setTime(Astromaximum.instance.getMidnight(Options.currentTime()));
        showDaySummary();
        repaint();
        break;
      case 5: // back to CustomTime
        Astromaximum.customTime.init(pageNum);
        break;
/*      case 1:
        if(pageNum==PAGE_DECUMB){
//          Astromaximum.interpreter.topic=9;
          Date tm=Astromaximum.customTime.dateField.getDate();
          if(!selDate.equals(tm)){
            selDate=tm;
            showDaySummary();
            break;
          }
        }
        setCurPage(PAGE_SUMMARY);
        repaint();
 */
    }
  }
  
  private static Vector evInCurrentDay(Vector dest, Vector src) {
    for (Enumeration e = src.elements() ; e.hasMoreElements() ;) {
      final Event ev=(Event)e.nextElement();
      if(ev.isDateBetween(0, period0, period1)){
        dest.addElement(ev);
      }
    }
    return dest;
  }
  
  /** @noinspection AssignmentToForLoopParameter,ValueOfIncrementOrDecrementUsed */ /* setCurPage
   *
   * @param wMode int
   */
  void setCurPage(int page) {
    int oldPage=pageNum;
    pageNum=page;
    if(pageNum==PAGE_WEEK || pageNum==PAGE_MONTH){
      colCount= page == PAGE_WEEK ? 1 : 7;
      rowCount= page == PAGE_WEEK ? 7 : 6;
//      System.out.println(date.toString());
      setCell(date.getTime(),true);
//#if logger
//#       Astromaximum.instance.logger("after setCell");
//#endif      
      gatherMonth();
//#if logger
//#       Astromaximum.instance.logger("gatherMonth");
//#endif      
    }
    recalcAllSelections();
    if(oldPage!=pageNum){
      selItem=-1;
      if(pageNum!=PAGE_WEEK && pageNum!=PAGE_MONTH){
        moveFocus(1,1);
      }
      if(pageNum!=PAGE_DECUMB && pageNum!=PAGE_HELP){
        moveFocus(1,1);
      }
    }
    final SummItem selsi=getItem(Event.EV_TITHI);
    if(selsi!=null && selsi.isOnPage()){
      selsi.setSelection();
//#if logger
//#       Astromaximum.instance.logger("recalcsel");
//#       Astromaximum.instance.logger(Integer.toString(selsi.nowSelection));
//#       Astromaximum.instance.logger(Integer.toString(selsi.selIndex)+" of "+
//#        Integer.toString(selsi.events.length));
//#endif      
      selsi.prepareTithi();
    }
//#if logger
//#       Astromaximum.instance.logger("end SetCurPage");
//#endif      
    repaint();
  }
  
  int colCount;
  int rowCount;
  int selMonth;
  static final Date firstGridDate=new Date();
  Date selDate=new Date();
  private int selCell;
  /**
   * getSelX
   *
   * @return int
   */
  int getSelX() {
    return selCell%colCount;
  }
  
  /**
   * getSelX
   *
   * @return int
   */
  int getSelY() {
    return selCell/colCount;
  }
  
  /**
   * moveMonth
   *
   * @param delta int
   */
  void moveMonth(int delta) {
    int oldMonth=selMonth;
    oldMonth+=delta;
    if(oldMonth >= Calendar.JANUARY && oldMonth <= Calendar.DECEMBER){
      selMonth=oldMonth;
      Astromaximum.calendar.setTime(selDate);
      Astromaximum.calendar.set(Calendar.MONTH,selMonth);
      Astromaximum.calendar.set(Calendar.DAY_OF_MONTH,1);
      selDate=Astromaximum.calendar.getTime();
//      System.out.println(selDate);
      setCell(selDate.getTime(),true);
//      gatherMonth();
    }
  }
  
  /**
   * moveDay
   *
   * @param delta int
   */
  void moveDay(int delta, boolean changePage){
//    System.out.println("Was "+Event.long2String(selDate.getTime(),0,false));
    long tmp=Astromaximum.instance.changeDate(selDate,delta);
//    System.out.println(" now= "+Event.long2String(selDate.getTime(),0,false));
    if(tmp!=0) {
//      System.out.println(" tmp="+Event.long2String(tmp,0,false));
      setCell(tmp,changePage);
    } 
    else{
      moveFocus(delta<0? -1: 1,delta);
    }
//    System.out.println(" now= "+Event.long2String(selDate.getTime(),0,false));
  }
  
  void setCell(long date1, boolean changePage){
    int first=1, diff=1;
    final int oldMonth=selMonth;
//    System.out.println("setCell");
    if(!changePage){
      diff=(int)((date1 - firstGridDate.getTime()) / Astromaximum.MSECINDAY);
      if(diff < 0 || diff >= rowCount * colCount){
        moveFocus(diff < 0? -1: 1, diff);
        return;
//        changePage=true;
      }
    }
    selDate.setTime(date1);

//    System.out.println(selDate.toString());
    if(changePage){
      Astromaximum.calendar.setTime(selDate);
      if(pageNum != PAGE_WEEK){
        diff = Astromaximum.calendar.get(Calendar.DAY_OF_MONTH);
        Astromaximum.calendar.set(Calendar.DAY_OF_MONTH, 1);
      }
      selMonth = Astromaximum.calendar.get(Calendar.MONTH);
      first = Astromaximum.calendar.get(Calendar.DAY_OF_WEEK) - 1;
      final Date dd2 = Astromaximum.calendar.getTime();
      firstGridDate.setTime(dd2.getTime() - first * Astromaximum.MSECINDAY);
      //    System.out.println("fgd="+firstGridDate.toString());
    }
    
    selCell = diff + first - 1;
    if(changePage) {
//      System.out.println(selMonth);
      gatherMonth();
    }
 
//    repaint();
  }
  
  final Vector mSelDeg=new Vector();
  final Vector mRetro=new Vector();
  final Vector mIngress=new Vector();
  Vector mAsp=new Vector();
  
  void gatherMonth() {
//    long tick=System.currentTimeMillis();
    final int cells=rowCount*colCount;
    SummItem.places=new byte[cells];
    getItem(Event.EV_GRID_DATE).setEvents(1,new Event(selDate.getTime(),-1));
    getItem(Event.EV_GRID_DATE).initString();
    period0 = firstGridDate.getTime();
    if(period0<Astromaximum.dataFile.startJD){
      period0=Astromaximum.dataFile.startJD;
    }
//    System.out.println("gatherMonth");
    period1 = period0 +cells*Astromaximum.MSECINDAY;
    period0-=Event.localOffset(period0);
    period1-=Event.localOffset(period1);
    if(period1>Astromaximum.dataFile.finalJD){
      period1=Astromaximum.dataFile.finalJD;
    }
//    System.out.println(firstGridDate.toString());
    mRetro.removeAllElements();
    for(int i=Event.SE_MERCURY; i <= Event.SE_PLUTO; i++) {
      Astromaximum.dataFile.getEventsOnPeriod(mRetro, Event.EV_RETROGRADE, i, false,
          period0, period1, 0);
    }
//    Astromaximum.evDump(mRetro);
    for(Enumeration e=mRetro.elements(); e.hasMoreElements();){
      final Event ev=(Event)e.nextElement();
      if(ev.date0 <= period0 && ev.date1 >= period1) {
        ev.setDegree(1);
      } 
      else {
        ev.setDegree(0);
      }
    }
    mIngress.removeAllElements();
    for(int i=Event.SE_SUN; i <= Event.SE_PLUTO; i++) {
      if (i != Event.SE_MOON || pageNum == PAGE_WEEK) {
        Astromaximum.dataFile.getEventsOnPeriod(mIngress, Event.EV_SIGN_ENTER,
            i, false, period0, period1, 0);
      }
    }
    if(pageNum == PAGE_WEEK){
      mSelDeg.removeAllElements();
      for(int i=Event.SE_SUN; i <= Event.SE_PLUTO; i++) {
        if (i != Event.SE_MOON) {
          mergeEvents(mSelDeg, Astromaximum.dataFile.getEvents(
              Event.EV_DEGREE_PASS, i,period0,period1), false);
        }
      }
      for(int i=0; i<mSelDeg.size(); i++) {
        if (Astromaximum.evAt(mSelDeg, i).getDegType() == 0) {
          mSelDeg.removeElementAt(i--);
        }
      }
      mAsp= Astromaximum.dataFile.getEvents(Event.EV_ASP_EXACT,-1,period0,period1);
    }
  }
  
//  private void weeklyForecast(int zodSign) {
//    System.out.println("Weekly forecast for "+Astromaximum.CONSTELL[zodSign]);
//  }
  
  private void recalcBounds(int w, int h) {
//#mdebug info
    System.out.print("*** Bounds = ");
    System.out.print(w);
    System.out.print(" x ");
    System.out.println(h);
    Astromaximum.log(Integer.toString(w)+" x "+Integer.toString(h));
//#enddebug
//    sizer.setSize(w,h);
    if(needRender){
//#ifdef UseBuffer
//#       offScreenBuffer = Image.createImage(w,h);
//#endif      
      PAGE_LAST=PAGE_SUMMARY+1;
      if(h < 230) {
        moonPhaseH = 28;
      } 
      else {
        moonPhaseH = 50;
      }
      if(h <= 220) {
        IMG_HEIGHT = IMG_WIDTH = 9;
      } 
      else {
        IMG_HEIGHT = IMG_WIDTH = 12;
      }
      size=Options.layout.getSelectedIndex();
      if(size==0){
        size=2; 
        if(h<210){
          size=1;
        }
        if(h < w){
          size=3;
        }
      }
      if(size==3){
        IMG_HEIGHT = IMG_WIDTH = 12;
        moonPhaseH = 50;
      }
      if(size!=2){
        PAGE_LAST+=2;
      }
      String ext="/res/sz"+Integer.toString(IMG_HEIGHT)+".dat";
      imgService=Astromaximum.extractImg(0,ext);
//#if logger
//#       Astromaximum.instance.logger(" imgService");
//#endif      
      imgZodiac=Astromaximum.extractImg(1,ext);
//#if logger
//#       Astromaximum.instance.logger(" imgZodiac");
//#endif      
      imgPlanet=Astromaximum.extractImg(2,ext);
//#if logger
//#       Astromaximum.instance.logger(" imgPlanet");
//#endif      
      imgOpaq=Astromaximum.extractImg(3,ext);
//#if logger
//#       Astromaximum.instance.logger(" imgOpaq");
//#endif      
      imgAspect=Astromaximum.extractImg(4,ext);
//#if logger
//#       Astromaximum.instance.logger(" imgAspect");
//#endif      
//      String ext=Integer.toString(IMG_HEIGHT)+".png";
      final int dx= w*10;
      final int dy= h*10;
      Astromaximum.log(Integer.toString(dy));
      bounds=loadArray("/res/size"+Integer.toString(size)+".dat");
      _bounds=new short[bounds.length];
      System.arraycopy(bounds, 0, _bounds, 0, bounds.length);
      for(int i=0; i<bounds.length/ BOUNDS_VARS; i++) {
        for (int j = 0; j < 4; j++) {
          _bounds[i * BOUNDS_VARS + j] = (short)(bounds[i * BOUNDS_VARS + j] * (j % 2 == 0 ? dx : dy) / 1000);
        }
      }
    }
  }
  
  private short[] loadArray(String resName){
    short[] arr=null;
    try {
      final DataInputStream dis=new DataInputStream(getClass().getResourceAsStream(resName));
      final int count=dis.available()/2;
      arr=new short[count];
      for(int i=0; i<count; i++) {
        arr[i] = dis.readShort();
      }
      dis.close();
    } catch (IOException ex) {
      ex.printStackTrace();
    }
    return arr;
  }
  
  private int border(int ind0, int ind1) {
    return _bounds[ind0* BOUNDS_VARS +ind1];
  }
  
  /**
   * showDaySummary
   *
   * @param date Date
   */
  void showDaySummary(){
    gatherSummary(selDate.getTime());
//#if logger
//#       Astromaximum.instance.logger("gatherSummary");
//#endif      
    setCurPage(PAGE_SUMMARY);
//#if logger
//#       Astromaximum.instance.logger("setCurPage");
//#endif      
    selItem=0;
    moveFocus(1,1);
//    Display.getDisplay(this).setCurrent(summary);
    Astromaximum.options.addImeiChar(this);
  }
  
  void dontRender() {
    needRender = false;
    Display.getDisplay(Astromaximum.instance).setCurrent(this);
    repaint();
    needRender=true;
  }
  
  private static void mergeEvents(Vector dest, Vector add, boolean isSort) {
    for(Enumeration e=add.elements();e.hasMoreElements();){
      final Event ev=(Event)e.nextElement();
      if(isSort){
        int idx=0; final long dat=ev.date0;
        final int sz=dest.size();
        while(idx < sz && dat > Astromaximum.evAt(dest, idx).date0) {
          ++idx;
        }
        dest.insertElementAt(ev,idx);
      } 
      else {
        dest.addElement(ev);
      }
//      System.out.println("Iteration");
//      evDump
    }
  }
  
  void startRealtime() {
    timer=new Timer();
    timer.schedule(new SummItem(1),DELAY,DELAY);
  }
  
  
  void calcDecumbiture(){
    Astromaximum.interpreter.topic=Interpreter.T_DECUMB;
    long startDate=Astromaximum.customTime.decumbDate;
    gatherSummary(Astromaximum.instance.getMidnight(startDate));
    Vector moonSign=new Vector();
    long p0=startDate-5*Astromaximum.MSECINDAY/2,p1=startDate+32*Astromaximum.MSECINDAY;
    Astromaximum.dataFile.getEventsOnPeriod(moonSign,Event.EV_SIGN_ENTER,Event.SE_MOON,
        false,p0,p1,0);
    Event e0=null,e1=null;
    int index;
    for(index=0; index<moonSign.size(); index++){
      e0=Astromaximum.evAt(moonSign,index);
      if(SummItem.contains(e0,startDate)){
        e1=Astromaximum.evAt(moonSign,index+1);
        break;
      }
    }
    period1 = period0 +Astromaximum.MSECINDAY-1;
    for(int i=moonPhase.size()-1; i>=0; i--){
      Event ph=Astromaximum.evAt(moonPhase,i);
      if(ph.date0<=startDate){
        getItem(Event.EV_MOON_PHASE,1).events[0]=ph;
        break;
      }
    }
    int ddegree=e1.getDegree()-e0.getDegree();
    if(ddegree<0){
      ddegree+=12;
    }
    ddegree*=30;
    int dgr=(int)((startDate-e0.date0)*ddegree/(e1.date0-e0.date0)+
        e0.getDegree()*30);
//    System.out.println(dgr);
    long[] decumb=new long[13];
    decumb[0]=startDate;
    for(int i=0; i<decumbAspects.length; i++){
      dgr+=decumbAspects[i];
//      System.out.println("***");
//      System.out.println(dgr);
      for(; index<moonSign.size(); index++){
        e0=Astromaximum.evAt(moonSign,index-1);
        e1=Astromaximum.evAt(moonSign,index);
        if(e0.getDegree()==dgr%360/30){
//          e0.dump();
//          e1.dump();
          ddegree=e1.getDegree()-e0.getDegree();
          if(ddegree<0){
            ddegree+=12;
          }
          ddegree*=30;
          decumb[i+1]=(dgr%360-e0.getDegree()*30)*(e1.date0-e0.date0)/
              ddegree+e0.date0;
//          System.out.println(Event.long2String(decumb[i+1],false,false));
          break;
        }
      }
    }
    for(int i=0; i<=decumbAspects.length; i++){
      getItem(Event.EV_DECUMBITURE,i).setEvents(0,new Event(decumb[i],i));
      long delta=Astromaximum.MSECINDAY/((i==0 || i==decumbAspects.length)? 1: 2);
      moonSign.removeAllElements();
      Astromaximum.dataFile.getAspectsOnPeriod(moonSign,Event.SE_MOON,decumb[i]-delta,
        decumb[i]+delta);
//      Astromaximum.evDump(moonSign);
      for(int j=0; j<moonSign.size(); j++){
        e0=Astromaximum.evAt(moonSign,j);
        dgr=e0.planet1;
        if(dgr>Event.SE_SATURN || dgr==Event.SE_MERCURY){
          moonSign.removeElementAt(j--);
        }
      }
      getItem(Event.EV_DECUMB_ASPECT,i).setEvents(moonSign);
    }
    
    Vector asi=new Vector();
    SummItem si=getItem(Event.EV_MOON_DAY);
    si.recalcSelection(startDate, true);
    asi.addElement(si.getCusSelEvent());
    si=getItem(Event.EV_DAY_HOURS);
    si.recalcSelection(startDate, true);
    Event ev=si.getCusSelEvent();
    if(ev==null){
      si=getItem(Event.EV_NIGHT_HOURS);
      si.recalcSelection(startDate, true);
      ev=si.getCusSelEvent();
      if(ev==null){
        long pp0=period0 - Astromaximum.MSECINDAY, pp1=period1 - Astromaximum.MSECINDAY;
        ev= Astromaximum.dataFile.getEventOnPeriod(Event.EV_RISE,Event.SE_SUN,true,
          pp0, pp1);
        ev.date1=Astromaximum.dataFile.getEventOnPeriod(Event.EV_SET,Event.SE_SUN,false,
          pp0, pp1).date0;
//        ev.dump();
        int weekDay=getItem(Event.EV_WEEK).events[1].planet0+5;
        Event[] aev=calcPlanetHours(ev,getItem(Event.EV_SUN_RISE).events[0],weekStartHour[weekDay%7]);
        si=new SummItem(Event.EV_LAST);
        si.setEvents(aev);
        si.recalcSelection(startDate, true);
//        si.dump();
        ev=si.getCusSelEvent();        
      }
    }
    asi.addElement(ev);
//    setCurPage(PAGE_SUMMARY+1);
    for(int plt=Event.SE_VENUS; plt<=Event.SE_SATURN; plt++){
      si=getItem(Event.EV_RISE,plt);
      si.recalcSelection(startDate, true);
//      si.dump();
      ev=si.getCusSelEvent();
      if(ev!=null /*&& ev.getDegree()%2==0*/){
        asi.addElement(ev);
      }
    }
//    Astromaximum.evDump(asi);

    getItem(Event.EV_DECUMB_BEGIN).setEvents(asi);
    setCurPage(PAGE_DECUMB);
    createCommands(1);
  }

 
  protected void sizeChanged(int w, int h){
    if(!Astromaximum.firstRun){
//      Astromaximum.log("chs");
//      items=null;
//      recalcBounds(w,h);
//      changeSize();
//      setCurPage(pageNum);
//      Display.getDisplay(Astromaximum.instance).setCurrent(this);
//      repaint(0, 0, getWidth(), getHeight()); 
    }
  }

  void recalcAllSelections() {
    long cur=Options.currentTime();
    long cus=isShowCustom? cusTime: 0;
    for(int i=0; i<items.length; i++) {
      SummItem si=items[i];
      if (si != null && si.isOnPage()) {
        si.initString();
        si.recalcSelection(cus, true);
        si.recalcSelection(cur, false);
      }
    }
  }
  
  protected void showNotify(){
//#if MIDP == "2.0"
    setFullScreenMode(true);
//#endif
//    if(timerTask!=null)
//      timerTask.run();
  }
  
  public void stop(){
    goon=false;
//#if imgPhase
    img=null;
//#endif
    if(timer!=null){
      timer.cancel();
    }
    progress=0;
  }

  private Event[] calcPlanetHours(Event starte, Event ende, int startHour) {
    Event[] ar=new Event[24];
    final long dHour=(starte.date1-starte.date0)/12;
//    ee.dump();
    final long nHour=(ende.date0-starte.date1)/12;
//    System.out.println(nHour);
    long st=starte.date0;
    for(int i=0; i < 24; i++){
      Event ev=new Event(st, hourSeq[startHour%7]);
      st += i < 12 ? dHour : nHour;
      ev.date1=st;
//      if(i==6 || i==18){
//        ev.date1+=60*1000; // +1 min for MC, IC
//      }
      ar[i]=ev;
      ++startHour;
    }
    return ar;
  }
  
  public void run() {
//#ifndef logger
    goon=true;
    timer=new Timer();
    timer.schedule(new SummItem(0),LOGO_DELAY,LOGO_DELAY);
//#endif    
    DataInputStream dis=new DataInputStream(getClass().getResourceAsStream("/res/panel.png"));
    try{
//#ifdef logger
//#     Astromaximum.instance.logger(Integer.toString(dis.available()));
//#endif    
      byte[] buf=new byte[dis.available()];
      dis.read(buf);
//#ifdef logger
//#     Astromaximum.instance.logger(Integer.toString(buf[1896]));
//#     Astromaximum.instance.logger(Integer.toString(buf[1897]));
//#endif    
      imgPanel =Image.createImage(buf,0,buf.length);
//#ifdef logger
//#     Astromaximum.instance.logger("imgPanel");
//#endif    
      dis=new DataInputStream(new ByteArrayInputStream(buf));
      int chlen=4, chtype, num=0;
      do{
	dis.skip(chlen+4);
	chlen=dis.readInt();
	chtype=dis.readInt();
//#mdebug debug
	System.out.print("Chunk=");
	System.out.println(chtype);
//#enddebug
	if(chtype==0x634f4445){
	  dis.read(buf,0,chlen);
	  DataFile.ids.addElement(LogBox.access(new String(buf,0,chlen),num++));
//#debug debug	  
	  System.out.println(DataFile.ids.lastElement());
	  chlen=0;
	}
      }while(chtype!=0x49454e44);
    }
    catch(IOException e){}
  }
  
  void drawPhase(Graphics osg, int x, int y, int wh, int phase) {
    int old_color=osg.getColor();
    osg.setColor(0);
    if((wh & 1)!=0){
      wh++;
    }
    switch(phase){
      case 2:
        osg.setColor(0xffffff);
      case 0:
        osg.fillArc(x,y,wh,wh,0,360);
        osg.setColor(0);
        break;
      case 1:
        osg.setColor(0);
        osg.fillArc(x,y,wh,wh,90,180);
        osg.setColor(0xffffff);
        osg.fillArc(x,y,wh,wh,270,180);
        osg.setColor(0);
        break;
      case 3:
        osg.setColor(0xf0f0f0);
        osg.fillArc(x,y,wh,wh,90,180);
        osg.setColor(0);
        osg.fillArc(x,y,wh,wh,270,180);
        break;
    }
    osg.drawArc(x,y,wh,wh,0,360);
    osg.setColor(old_color);
  }

  /** @noinspection AssignmentToMethodParameter*/
  void keyNavigate(int dir) {
    int dn;
    do{
      dn=getSelectedItem().defaultNavigate(dir);
    }while(dn<0);
    boolean vert=dir>=2;
    int delta= (dir&1)==0? -1: 1;
    SummItem si=getSelectedItem();
    switch(dn){
      case 0:
        break;
      case 21:
        changeDay(delta);
        long tm=date.getTime();
        tm+=Event.localOffset(tm);
        Astromaximum.customTime.dateField.setDate(new Date(tm));
        //#ifdef ELECTIO
//#         if(pageNum==PAGE_ELECTIO){
//#           calcElectio();
//#           break;
//#         }
        //#endif
        return;
      case 22:
        si.selIndex=1-si.selIndex;
        break;
      case 23: // Event.EV_MONTH_GRID
        moveDay(delta*(vert? colCount: 1),false/*!vert*/);
        break;
      case 24: // Event.EV_WEEK_GRID:
        moveDay(delta*(vert? 1: rowCount),!vert);
        break;
      case 25: // Event.EV_DATE_GRID:
        si.selIndex=1;
        if(pageNum == Summary.PAGE_MONTH) {
          moveMonth(delta);
        } 
        else{
          moveDay(delta*rowCount,true);
        }
        break;
      case 26: // Event.EV_MOON_MOVE
        if(si.selIndex<si.events.length/2){ // at head
          moveFocus(delta>0? 1: (size==1? -3: -4), dir);
        }
        else{ // at tail
          moveFocus(delta>0? 2: -1, dir);
        }
        break;
      case 27: // Event.EV_WEEK
        moveFocus(delta,1);
        if(pageNum==PAGE_MONTH){
          setCell(firstGridDate.getTime()+
                (delta>0? rowCount/2: rowCount*colCount-rowCount/2-1)*Astromaximum.MSECINDAY,false);
        }
        if(pageNum==PAGE_WEEK){
          setCell(firstGridDate.getTime()+
                (delta>0? 0: rowCount-1)*Astromaximum.MSECINDAY,false);
        }
        break;
      case 28: // Event.EV_ASP_EXACT
        if(size==2 && dir==2){
          if(si.selIndex<si.events.length/2){ // at head
            moveFocus(-2, dir);
          }
          else{ // at tail
            moveFocus(-1, dir);
          }
          if(getSelectedItem().isEmpty()){
            moveFocus(delta, dir);
          }
        }
        break;
    }
    si=getSelectedItem();
    if(si.isEmpty()){
      si.defaultNavigate(dir);
    }
    repaint();
  }

  //#ifdef ELECTIO
//#   void calcElectio()
//#   {
//#     Vector vElectio=new Vector();
//#     Astromaximum.dataFile.getEventsOnPeriod(vElectio,Event.EV_ASCAPHETICS,-1,false,
//#         period0, period1,0);
//# //    Astromaximum.evDump(vElectio);
//#     getItem(Event.EV_ASCAPHETICS).setEvents(vElectio);
//#   }
  //#endif  

  private void createCommands(int mode) {
    for(int i=0; i<cmds.length; i++){
      removeCommand(cmds[i]);
      cmds[i]=null;
    }
    if(mode==0){
      cmds[0]=new Command(LocalizationSupport.getMessage("Help"), Command.SCREEN, 0);
      cmds[1]=new Command(LocalizationSupport.getMessage("Today"), Command.SCREEN, 1);
      cmds[2]=new Command(LocalizationSupport.getMessage("Week"), Command.SCREEN, 2);
      cmds[3]=new Command(LocalizationSupport.getMessage("Month"), Command.SCREEN, 3);
      cmds[4]=new Command(LocalizationSupport.getMessage("Options"),Command.SCREEN,4);
      cmds[6]=new Command(LocalizationSupport.getMessage("No_theme"), Command.SCREEN, 6);
      //#ifdef ELECTIO
//#       cmds[7]=new Command(LocalizationSupport.getMessage("Aphetics"),Command.SCREEN,7);
      //#endif
    }
    if(mode==1){
      cmds[5]=new Command(LocalizationSupport.getMessage("Ch_decumb"), Command.SCREEN, 5);
    }
    cmds[6]=new Command(LocalizationSupport.getMessage("No_theme"), Command.SCREEN, 6);
    for(int i=0; i<cmds.length; i++){
      if(cmds[i]!=null){
        addCommand(cmds[i]);
      }
    }
  }

  protected Timer timer;

  private int progress;

  protected boolean goon;

  private Image img;

  private final String moonFile;

  private int moonX;

  private int moonY;

  private final int frameCount;

  void setMoonXY(int x, int y, int flags) {
    moonX = x;
    moonY = y;
    if ((flags & Graphics.HCENTER) > 0) {
      moonX -= img.getWidth() >> 1;
    }
    if ((flags & Graphics.VCENTER) > 0) {
      moonY -= img.getHeight() >> 1;
    }
    if ((flags & Graphics.RIGHT) > 0) {
      moonX -= img.getWidth();
    }
    if ((flags & Graphics.BOTTOM) > 0) {
      moonY -= img.getHeight();
    }
  }

  protected void drawFrame() {
    if (goon) {
      if (progress < frameCount / 2) {
        img = Astromaximum.extractImg(progress, moonFile);
//        System.out.println("drawFrame");
        repaint();
        serviceRepaints();
        progress+=2;
      }
    }
  }
//#endif  
}

