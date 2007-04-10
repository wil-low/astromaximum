/** Interpreter
 * <p>Title: Nomad</p>
 *
 * <p>Description: </p>
 *
 * <p>Copyright: Copyright (c) 2006</p>
 *
 * <p>Company: Wiland Inc.</p>
 *
 * @author Andrei Ivushkin
 * @version 1.0
 */

import javax.microedition.lcdui.*;
import java.io.*;

class Interpreter extends Canvas implements CommandListener {
//#if "timeBomb" @ protection
//#   static int hj=0x01234567;
//#endif
  private boolean helpMode;
  private int HMARGIN;
  private int VMARGIN;
  private final Command[] cmds=new Command[3];
  private final String[] labels={
    LocalizationSupport.getMessage("Back"),
    LocalizationSupport.getMessage("Text_font"),
    LocalizationSupport.getMessage("Help")
  };
  static final String[] riseKeys={"asc","mc","dsc","ic"};
  private int curX, curY, topLine=0, lineCount;
  private int fontSize, lineHeight;
  static final int T_BUSINESS=0;
  static final int T_FINANCE=1;
  static final int T_LICENSE=2;
  static final int T_EMPLOY=3;
  static final int T_REALTY=4;
  static final int T_VACATION=5;
  static final int T_MEDICINE=6;
  static final int T_DECUMB=7;
  static final int T_LOVE=8;
  byte[] interp;
  String txt="";
  static String RESERVED_CHARS="}$>*^{~#=@\0";
  static int topic=10;
  boolean isLogged=false;
  
  Interpreter(){
    super();
//#if MIDP == "2.0"
    setFullScreenMode(true);
//#endif
//    offScreenBuffer=Image.createImage(getWidth()-HMARGIN*2,getHeight()*VMARGIN*2);
    setCommandListener(this);
    for(int i=0; i < labels.length; i++){
      cmds[i]=new Command(labels[i], Command.BACK, 1);
      addCommand(cmds[i]);
    }
    VMARGIN=HMARGIN=getWidth()/25;
    fontSize=Font.SIZE_SMALL;
  }
  
  /**
   * findText
   *
   * @return boolean
   * @noinspection InfiniteLoopStatement
   * @param si
   */
/*  
  boolean findText(SummItem si) {
//    System.out.println("ft");
    txt="";
    final long[] params=si.getParams(si.selIndex);
    if(params==null) {
      return false;
    }
    boolean isTopicTitle=false;
//    final long tick=System.currentTimeMillis();
    final DataInputStream dis=new DataInputStream(
        getClass().getResourceAsStream("/interp.txt"));
    boolean f;
    String s ="???";
    try {
      while(dis.available() > 0){
        final int evt = dis.readUnsignedShort();
        final int partsz = dis.readInt();
        if(evt!=params[0]){
          dis.skip(partsz-6);
          continue;
        }
        final int plt=dis.readByte();
        if(plt != -1 && plt != params[1]){
          dis.skip(partsz-7);
          continue;
        }
        final int paramcount=dis.readShort();
        int recnum=dis.readUnsignedShort();
        while(recnum > 0){
          f=true;
          for(int j=0; j<paramcount; j++){
            final int rsh=dis.readShort();
            if(params[j+2]!=rsh){
              f=false;
              dis.skip(2*(paramcount-j-1));
              break;
            }
          }
          if(f){
//            int sh=dis.readInt();
//            System.out.println(sh);
//            s="";
            s=dis.readUTF();
//            System.out.println("FullText:");
//            System.out.println(s);
            isTopicTitle=si.haveTopic((int)params[0]);
            int topp=isTopicTitle? topic: 10;
            char[] allowed=new char[2];
            StringBuffer sb=new StringBuffer(s);
            if(topp!=10){
              allowed[1]=RESERVED_CHARS.charAt(topp);
              if(topp!=T_MEDICINE && topp!=T_DECUMB && topp!=T_VACATION && topp!=T_LOVE){
                allowed[0]='@';
              }
              for(int i=0; i<RESERVED_CHARS.length(); i++){
                char rc=RESERVED_CHARS.charAt(i);
                if(allowed[0]!=rc && allowed[1]!=rc){
                  for(int j=0; j<sb.length(); j++){
                    if(rc==sb.charAt(j)){
                      sb.deleteCharAt(j--);
                    }
                  }
                }
              }
              s=sb.toString();
              sb=new StringBuffer();
              for(int i=0; i<s.length(); i++){
                char rc=s.charAt(i);
                if(allowed[0]==rc || allowed[1]==rc){
                  int pos=s.indexOf(rc,i+1);
                  sb.append(s.substring(i+1,pos));
                  i=pos;
                }
              }
            }
            else{
              s=removeTopic(s,T_LOVE);
              s=removeTopic(s,T_MEDICINE);
              s=removeTopic(s,T_DECUMB);
              sb=new StringBuffer(s);
            }
            
            for(int i=0; i<sb.length(); i++){
              if(RESERVED_CHARS.indexOf(sb.charAt(i))>=0){
                sb.deleteCharAt(i--);
              }
            }
            s=sb.toString();
            s=s.trim();
            break;
          }
          dis.skip(dis.readUnsignedShort());
          --recnum;
        }
      }
    } catch (IOException ex) {
      Astromaximum.log(ex.toString());
    }
    if(s.length()==0){
      s=LocalizationSupport.getMessage("Minor_index");
    }
//    System.out.println(">"+s+"<");
    
    if(!s.endsWith(".")){
      s+=".";
    }
//    final String[] sp=new String[params.length-1];
//    for(int j=1; j<params.length; j++) {
//      sp[j - 1] = Long.toString(params[j]);
//    }
    String res="-???-";
    switch((int)params[0]){
      case Event.EV_MOON_DAY:
        res=LocalizationSupport.getMessage("Moon_day#")+Integer.toString((int)params[2]);
        break;
      case Event.EV_NAVROZ:
        if(params[2]>=360){
          params[2]=359-params[2];
        }
        res=LocalizationSupport.getMessage("Sun_day#")+Integer.toString((int)params[2]);
        break;
      case Event.EV_DEG_2ND:
      case Event.EV_DEGREE_PASS:
        res=Astromaximum.PLANETS[(int)params[1]]+" "+
            Integer.toString((int)params[3])+"\u00b0"+Astromaximum.CONSTELL[(int)params[4]];
        if(params[5]>0){
          res+=" "+LocalizationSupport.getMessage("deg"+Integer.toString((int)params[5]));
        }
        break;
      case Event.EV_TITHI:
        res=LocalizationSupport.getMessage("Tithi")+" #"+Integer.toString((int)params[2]);
        break;
      case Event.EV_VOC:
        res=LocalizationSupport.getMessage("VOC");
        break;
      case Event.EV_VIA_COMBUSTA:
        res="Via Combusta";
        break;
      case Event.EV_WEEK:
        res=LocalizationSupport.getMessage("wd"+Integer.toString((int)params[2]));
        break;
      case Event.EV_HELP:
      case Event.EV_DECUMBITURE:
        res="";
        break;
      case Event.EV_SIGN_ENTER:
        res=getFullPlanet(params[1])+" "+LocalizationSupport.getMessage(
            "in_")+Astromaximum.CONSTELL[(int)params[2]];
        break;
      case Event.EV_ECLIPSE:
        res=LocalizationSupport.getMessage("Eclipse")+" "+
            LocalizationSupport.getMessage("of_"+Astromaximum.PLANETS[(int)params[2]]);
        break;
      case Event.EV_PLANET_HOUR:
        res=LocalizationSupport.getMessage("Hour")+" "+
            LocalizationSupport.getMessage("of_"+Astromaximum.PLANETS[(int)params[2]]);
        break;
      case Event.EV_MOON_PHASE:
        res=LocalizationSupport.getMessage("mph"+Integer.toString((int)params[2]));
        break;
      case Event.EV_RISE:
        res= "(\u00b124 "+LocalizationSupport.getMessage("min.")+") "+
            getFullPlanet(params[2])+" "+
            getRiseString((int)params[3]);
        break;
      case Event.EV_MOON_MOVE:
        //        System.out.println(params[4]);
        final String s1= getFullPlanet(params[4]);
        final String s2= params[3] == Event.SE_MOON ? "VOC" : getFullPlanet(params[3]);
        res=s1+">"+s2;
        break;
      case Event.EV_ASP_EXACT:
        if(params[1] == Event.SE_MOON){
          res=Astromaximum.PLANETS[(int)params[1]]+"-"+Integer.toString((int)params[4])+"\u00b0-"+
              Astromaximum.PLANETS[(int)params[2]];
        }
        else{
          res=Astromaximum.PLANETS[(int)params[2]]+"-"+Integer.toString((int)params[5])+"\u00b0-"+
              Astromaximum.PLANETS[(int)params[3]]+" ";
          res+=LocalizationSupport.getMessage(params[2]<=Event.SE_MARS?
            "+-week": "long_aspect");
        }
        break;
      case Event.EV_RETROGRADE:
//        System.out.println(params[2]);
        res=getFullPlanet(params[2]);
        break;
    }
   
    String ss="";
    if(params[params.length - 2] != 0) {
      ss = Event.long2String(params[params.length - 2], 0, false);
    }
    if(params[0] == Event.EV_TITHI) {
      ss += " - (" + Event.long2String(params[params.length - 3], 0, false) + ")";
    }
    if(params[params.length - 1] != 0){ // 2nd date
      ss+=" - ";
      //      if(Astromaximum.sizer.getSize()==0)
      //        ss+="\n";
      ss+=Event.long2String(params[params.length-1],0,true);
    }
    if(topic!=10 && isTopicTitle){
      res+=" <"+LocalizationSupport.getMessage("fb"+Integer.toString(topic))+">";
    }
    if(res.length()>0) {
      res += ":  ";
    }
    txt=ss+"|"+res+"|"+s;
    prepareText();
//    final String et="iET="+Long.toString(System.currentTimeMillis()-tick);
//    Astromaximum.log(et);
    return s.length()>1;
  }
*/  
  boolean findText(SummItem si, boolean ignoreAllTopics) {
//    System.out.println("ft");
    txt="";
    final long[] params=si.getParams(si.selIndex);
    if(params==null) {
      return false;
    }
    boolean isTopicTitle=false;
    boolean f;
    String s=extractArticle(params);
    StringBuffer sb=new StringBuffer(s);
    if(!ignoreAllTopics){
//    System.out.println("FullText:");
//    System.out.println(s);
      char allowed0=0, allowed1=0;
      isTopicTitle=si.haveTopic((int)params[0]);
      int topp=topic;
      if(params[0]==Event.EV_MOON_MOVE){
        allowed0='@'; 
      }
      else{
        topp=isTopicTitle? topic: 10;
      }
      if(topp!=10){
        allowed1=RESERVED_CHARS.charAt(topp);
        if(topp!=T_MEDICINE && topp!=T_DECUMB && topp!=T_VACATION && topp!=T_LOVE){
            allowed0='@';
        }
        for(int i=0; i<RESERVED_CHARS.length(); i++){
          char rc=RESERVED_CHARS.charAt(i);
          if(allowed0!=rc && allowed1!=rc){
            for(int j=0; j<sb.length(); j++){
              if(rc==sb.charAt(j)){
                sb.deleteCharAt(j--);
              }
            }
          }
        }
        s=sb.toString();
        sb=new StringBuffer();
        for(int i=0; i<s.length(); i++){
          char rc=s.charAt(i);
          if(allowed0==rc || allowed1==rc){
            int pos=s.indexOf(rc,i+1);
            sb.append(s.substring(i+1,pos));
            i=pos;
          }
        }
      }
      else{
        s=removeTopic(s,T_LOVE);
        s=removeTopic(s,T_MEDICINE);
        s=removeTopic(s,T_DECUMB);
        sb=new StringBuffer(s);
      }
    }
    for(int i=0; i<sb.length(); i++){
      if(RESERVED_CHARS.indexOf(sb.charAt(i))>=0){
        sb.deleteCharAt(i--);
      }
    }
    s=sb.toString();
    s=s.trim();
    if(s.length()==0){
      s=LocalizationSupport.getMessage("Minor_index");
    }
//    System.out.println(">"+s+"<");
    
//    if(!s.endsWith(".")){
//      s+=".";
//    }
    
    StringBuffer res=new StringBuffer();
//#if 1==1    
    switch((int)params[0]){
      case Event.EV_MOON_DAY:
        res.append(LocalizationSupport.getMessage("Moon_day#")).append(params[2]);
        break;
      case Event.EV_NAVROZ:
        if(params[2]>=360){
          params[2]=359-params[2];
        }
        res.append(LocalizationSupport.getMessage("Sun_day#")).append(params[2]);
        break;
      case Event.EV_DEG_2ND:
      case Event.EV_DEGPASS0:
      case Event.EV_DEGPASS1:
      case Event.EV_DEGPASS2:
      case Event.EV_DEGPASS3:
        res.append(Astromaximum.PLANETS[(int)params[1]]).append(" ")
            .append(params[3]).append("\u00b0").append(Astromaximum.CONSTELL[(int)params[4]]);
        if(params[5]>0){
          res.append(" ").append(LocalizationSupport.getMessage("deg"+Integer.toString((int)params[5])));
        }
        break;
      case Event.EV_TITHI:
        res.append(LocalizationSupport.getMessage("Tithi")).append(" #").append(params[2]);
        break;
      case Event.EV_VOC:
        res.append(LocalizationSupport.getMessage("VOC"));
        break;
      case Event.EV_VIA_COMBUSTA:
        res.append("Via Combusta");
        break;
      case Event.EV_WEEK:
        res.append(LocalizationSupport.getMessage("wd"+Integer.toString((int)params[2])));
        break;
//      case Event.EV_HELP:
//      case Event.EV_DECUMBITURE:
//        res="";
//        break;
      case Event.EV_SIGN_ENTER:
        res.append(getFullPlanet(params[1])).append(" ").append(LocalizationSupport.getMessage(
            "in_")).append(Astromaximum.CONSTELL[(int)params[2]]);
        break;
      case Event.EV_ECLIPSE:
        res.append(LocalizationSupport.getMessage("Eclipse")).append(" ").append(
            LocalizationSupport.getMessage("of_"+Astromaximum.PLANETS[(int)params[2]]));
        break;
      case Event.EV_PLANET_HOUR:
        res.append(LocalizationSupport.getMessage("Hour")).append(" ").append(
            LocalizationSupport.getMessage("of_"+Astromaximum.PLANETS[(int)params[2]]));
        break;
      case Event.EV_MOON_PHASE:
//        res.append(LocalizationSupport.getMessage("mph"+Integer.toString((int)params[2])));
        break;
      case Event.EV_RISE:
        res.append("(-40/+28 ").append(LocalizationSupport.getMessage("min."))
            .append(") ").append(getFullPlanet(params[2])).append(" ")
            .append(getRiseString((int)params[3]));
        break;
      case Event.EV_MOON_MOVE:
        //        System.out.println(params[4]);
        res.append(getFullPlanet(params[4])).append(">")
          .append(params[3] == Event.SE_MOON ? "VOC" : getFullPlanet(params[3]));
        break;
      case Event.EV_ASP_EXACT:
        if(params[1] == Event.SE_MOON){
          res.append(Astromaximum.PLANETS[(int)params[1]]).append("-")
              .append(params[4]).append("\u00b0-")
              .append(Astromaximum.PLANETS[(int)params[2]]);
        }
        else{
          res.append(Astromaximum.PLANETS[(int)params[2]]).append("-")
              .append(params[5]).append("\u00b0-")
              .append(Astromaximum.PLANETS[(int)params[3]]).append(" ");
          res.append(LocalizationSupport.getMessage(params[2]<=Event.SE_MARS?
            "+-week": "long_aspect"));
        }
        break;
      case Event.EV_RETROGRADE:
//        System.out.println(params[2]);
        res.append(getFullPlanet(params[2]));
        break;
    }
//#endif   
    String ss="";
    if(params[params.length - 2] != 0) {
      ss = Event.long2String(params[params.length - 2], 0, false);
    }
    if(params[0] == Event.EV_TITHI) {
      ss += " - (" + Event.long2String(params[params.length - 3], 0, false) + ")";
    }
    if(params[params.length - 1] != 0){ // 2nd date
      ss+=" - ";
      //      if(Astromaximum.sizer.getSize()==0)
      //        ss+="\n";
      ss+=Event.long2String(params[params.length-1],0,true);
    }
    if(topic!=10 && isTopicTitle){
      res.append(" <").append(LocalizationSupport.getMessage("fb"+Integer.toString(topic)))
        .append(">");
    }
    if(res.length()>0) {
      res.append(":  ");
    }
    txt=ss+"|"+res.toString()+"|"+s;
    prepareText();
//    final String et="iET="+Long.toString(System.currentTimeMillis()-tick);
//    Astromaximum.log(et);
    return s.length()>1;
  }

  protected void sizeChanged(int w, int h) {
    //    Astromaximum.instance.recalcBounds(getWidth(),getHeight());
  }
  
 
  public void commandAction(Command c, Displayable d)  {
    if (c==cmds[0]) {
      txt=null;
      System.gc();
      Astromaximum.summary.dontRender();
    }
    if (c==cmds[1]) {
      switch(fontSize){
        case Font.SIZE_LARGE:
          fontSize=Font.SIZE_SMALL;
          break;
        case Font.SIZE_MEDIUM:
          fontSize=Font.SIZE_LARGE;
          break;
        case Font.SIZE_SMALL:
          fontSize=Font.SIZE_MEDIUM;
          break;
      }
      repaint();
    }
    if (c==cmds[2]) {
      Astromaximum.summary.setCurPage(Summary.PAGE_HELP);
      Astromaximum.summary.dontRender();
      
    }
    
  }
  
  private static String getFullPlanet(long i){
    return LocalizationSupport.getMessage(Astromaximum.PLANETS[(int)i]);
  }
  
  private static String getRiseString(int i) {
    return LocalizationSupport.getMessage(riseKeys[i-1]);
  }
  
  protected void paint(Graphics graphics) {
    graphics.setColor(Astromaximum.CURRENT_MONTH_COLOR);
    graphics.fillRect(0,0,graphics.getClipWidth(),graphics.getClipHeight());
    Graphics osg=graphics;
//#ifdef UseBuffer    
//#     osg=Summary.offScreenBuffer.getGraphics();
//#endif
    osg.setColor(Astromaximum.CURRENT_MONTH_COLOR);
    osg.fillRect(0,0,osg.getClipWidth(),osg.getClipHeight());
    osg.setColor(0);
    Font oldFont=osg.getFont();
    osg.setFont(Font.getFont(Font.FACE_PROPORTIONAL,Font.STYLE_PLAIN,fontSize));
    lineHeight=osg.getFont().getHeight();
    curY=topLine; curX=0;
    graphics.translate(HMARGIN-graphics.getTranslateX(),VMARGIN-graphics.getTranslateY());
    drawArticle(osg,txt);
//#ifdef UseBuffer
//#     graphics.drawImage(Summary.offScreenBuffer, 0, 0, Graphics.LEFT | Graphics.TOP);
//#endif
    osg.setFont(oldFont);
  }
  
  private void drawArticle(Graphics osg, String string) {
//    Font oldFont=osg.getFont();
//    osg.setFont(Font.getFont(Font.FACE_PROPORTIONAL,Font.STYLE_UNDERLINED,fontSize));
    renderString(osg,string);
//    osg.setFont(oldFont);
  }
  
  private void renderString(Graphics osg,String s) {
    Font fnt=osg.getFont();
    int len=s.length();
    if(len==0)
      return;
    boolean isLastSpace=false;
    int width=getWidth()-HMARGIN*2;
    char[] ca=new char[len];
    s.getChars(0,len,ca,0);
    int spaceW=fnt.charWidth(' ');
    int start=0, i, cw=0; char curc=0;
    while(start<len){
      boolean crlf=false;
      for(i=start; i<len; i++){
        curc=ca[i];
        if(curc==' ' || curc=='|'){
          break;
        }
      }
      int wlen=i-start;
      cw=fnt.charsWidth(ca,start,wlen);
      if(curX+cw>width){
        newLine();
      }
      osg.drawChars(ca, start, wlen, curX, curY, Graphics.LEFT|Graphics.TOP);
      isLastSpace=false;
      if(curc=='|'){
        newLine();
      } 
      else{
        if(!isLastSpace){
          curX+=cw+spaceW;
          isLastSpace=true;
        }
      }
      start+=wlen+1;
    }
    
    newLine();
  }
  
  private void prepareText() {
    lineCount=topLine=0;
  }
  
  private void newLine() {
    curY+=Font.getDefaultFont().getHeight();
    curX=0;
    ++lineCount;
  }
  
  protected void keyReleased(int keyCode) {
    final int ga=getGameAction(keyCode);
    switch (ga) {
      case Canvas.FIRE:
//#if logger
//#         if(isLogged){
//#           Astromaximum.instance.logger("Stopping log...");
//#           isLogged=false;
//#           Astromaximum.summary.stop();
//#         }
//#         else{
//#           Astromaximum.summary.dontRender();
//#         }
//#else
        Astromaximum.summary.dontRender();
//#endif
        break;
      case Canvas.UP:
        if(topLine+VMARGIN*3<getHeight()){
          topLine+=lineHeight;
          repaint();
        }
        break;
      case Canvas.DOWN:
        if(curY>VMARGIN*2){
          topLine-=lineHeight;
          repaint();
        }
        break;
        
    }
  }
  
  protected void pointerPressed(int x, int y) {
    y-=VMARGIN;
    if(y>topLine && y<curY){
      topLine+=(getHeight()/2-y-VMARGIN);
      repaint();
    }
  }

  private String removeTopic(String s, int index) {
    char tch=RESERVED_CHARS.charAt(index);
//    System.out.println(tch);
    int pos=s.indexOf(tch);
    if(pos>=0){
      s=s.substring(0,pos)+s.substring(s.indexOf(tch,pos+1)+1);
    } 
    return s;
  }
  
  String extractArticle(long[] params){
    String res="???";
    try {
      InputStream is=getClass().getResourceAsStream(Long.toString(params[0])+".txt");
      interp=new byte[is.available()];
      is.read(interp);
      is=null;
      DataInputStream dis=new DataInputStream(
          new ByteArrayInputStream(interp));
//      while(true){
        final int evt = dis.readUnsignedShort();
        final int partsz = dis.readInt();
        if(evt!=params[0]){
          dis.skip(partsz-6);
//          continue;
//          return res;
        }
        final int plt=dis.readByte();
        if(plt != -1 && plt != params[1]){
          dis.skip(partsz-7);
//          continue;
//          return res;
        }
        final int paramcount=dis.readShort();
        int recnum=dis.readUnsignedShort();
        while(recnum > 0){
          boolean f=true;
          for(int j=0; j<paramcount; j++){
            final int rsh=dis.readShort();
            if(params[j+2]!=rsh){
              f=false;
              dis.skip(2*(paramcount-j-1));
              break;
            }
          }
          if(f){
            res=dis.readUTF();
            dis=null;
            break;
          }
          dis.skip(dis.readUnsignedShort());
          --recnum;
        }
//      }
    } 
    catch (IOException ex) {
//      Astromaximum.log(ex.toString());
    }
    interp=null;
    return res;
  }
}
