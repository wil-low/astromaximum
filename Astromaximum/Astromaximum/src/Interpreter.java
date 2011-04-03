
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
import java.util.*;
import javax.microedition.rms.*;

class Interpreter extends Canvas implements CommandListener {
//#if "timeBomb" @ protection
    static final int hj = 0x01234567;
//#endif
//    private boolean helpMode;
    private final int HMARGIN;
    private int VMARGIN;
    static final String[] riseKeys = {"asc", "mc", "dsc", "ic"};
    private int curX,  curY,  topLine = 0,  lineCount;
    int fontSize;
    private int lineHeight;
    static final int T_BUSINESS = 0;
    static final int T_FINANCE = 1;
    static final int T_LICENSE = 2;
    static final int T_EMPLOY = 3;
    static final int T_REALTY = 4;
    static final int T_VACATION = 5;
    static final int T_MEDICINE = 6;
    static final int T_DECUMB = 7;
    static final int T_LOVE = 8;
    static final int T_NONE = 10;
    String txt = "";
    static final String RESERVED_CHARS = "}$>*^{~#=@\0";
    static int topic = T_NONE;
    boolean isLogged = false;
    final Command[] cmds = new Command[3];
    private boolean mousePressed = false;

//#ifdef use_amtext
//#     Hashtable hamtext=new Hashtable();
//#     RecordStore rs;
//#endif
    Interpreter() {
        super();
        setFullScreenMode(true);
//    offScreenBuffer=Image.createImage(getWidth()-HMARGIN*2,getHeight()*VMARGIN*2);
        setCommandListener(this);
        
        HMARGIN = getWidth() / 25;
        fontSize = 0;
//#ifdef test_rs
//#         byte[] text = null;
//#         String msg="";
//# try{
//#         RecordStore rs = RecordStore.openRecordStore("AMtext", "S&W Axis", "AMtext");
//#         byte[] data=rs.getRecord(1);
//#         for (int i = 0; i < data.length; i++) {
//#             text=rs.getRecord(i+2);
//#             long sum=0;
//#             for(int j=0; j<text.length; j++){
//#                 sum+=text[j];
//#             }
//#             msg+=Long.toString(sum)+",";
//#         }
//# }catch(Exception ex){};
//#         Astromaximum.log(msg);
//#endif

//#ifdef use_amtext
//#         try {
//#             rs = RecordStore.openRecordStore("AMtext", "S&W Axis", "AMtext");
//#             byte[] data=rs.getRecord(1);
//#             String msg1="";
//#             for(int i=0; i<data.length; i++){
//#                 hamtext.put(new Integer(data[i]), new Integer(i+2));
//# //                outs+=new Integer(data[i]).toString()+",";
//#                 byte[] interp=rs.getRecord(i+2);
//#                 long sum=0;
//#                 for(int j=0; j<interp.length; j++){
//#                     sum+=interp[j];
//#                 }
//#                 msg1+=Long.toString(sum)+",";
//#              }
//# //            Astromaximum.log(msg);
//#             msg1="";
//#             for(Enumeration e=hamtext.keys(); e.hasMoreElements();){
//#                 Integer kk=(Integer) e.nextElement();
//#                 msg1+=kk.toString()+"=>"+hamtext.get(kk).toString()+", ";
//#             }
//#             Astromaximum.log(msg1);
//#         } catch (RecordStoreException ex) {
//#             Astromaximum.log("RS:"+ex.getMessage());
//#         }
//#endif

    }

    void recreateCommands() {
        cmds[0] = new Command(Astromaximum.getstr(94), Command.BACK, 1);//back
        cmds[1] = new Command(Astromaximum.getstr(140), Command.BACK, 2);//text font
        cmds[2] = new Command(Astromaximum.getstr(90), Command.BACK, 3);//help

        addCommand(cmds[0]);
        addCommand(cmds[1]);
        addCommand(cmds[2]);
    }
    
    /**
     * findText
     *
     * @param si
     * @param ignoreAllTopics
     * @return boolean
     * @noinspection InfiniteLoopStatement
     */
    boolean findText(SummItem si, boolean ignoreAllTopics) {
        txt = "";
        final long[] params = si.getParams(si.selIndex);
        if (params == null) {
            return false;
        }
        boolean isTopicTitle = false;
        String s = extractArticle(params);
        if (s == null) {
            return false;
//            txt = s = Astromaximum.getstr(110);//demo texts
        } else {
            if (si.type == Event.EV_HELP) // Help ignores selected topic
            {
                ignoreAllTopics = true;
            }
            StringBuffer sb = new StringBuffer(s);
            if (!ignoreAllTopics) {
                //    System.out.println("FullText:");
                //    System.out.println(s);
                char allowed0 = 0, allowed1;
                isTopicTitle = si.haveTopic((int) params[0]);
                int topp = topic;
                if (params[0] == Event.EV_MOON_MOVE) {
                    allowed0 = '@';
                } else {
                    topp = isTopicTitle ? topic : T_NONE;
                }
                if (topp != T_NONE) {
                    allowed1 = RESERVED_CHARS.charAt(topp);
                    if (topp != T_MEDICINE && topp != T_DECUMB && topp != T_VACATION && topp != T_LOVE) {
                        allowed0 = '@';
                    }
                    final int rclen = RESERVED_CHARS.length();
                    for (int i = 0; i < rclen; i++) {
                        char rc = RESERVED_CHARS.charAt(i);
                        if (allowed0 != rc && allowed1 != rc) {
                            for (int j = 0; j < sb.length(); j++) { // do not optimize
                                if (rc == sb.charAt(j)) {
                                    sb.deleteCharAt(j--);
                                }
                            }
                        }
                    }
                    s = sb.toString();
                    sb = new StringBuffer();
                    int slen = s.length();
                    for (int i = 0; i < slen; i++) {
                        char rc = s.charAt(i);
                        if (allowed0 == rc || allowed1 == rc) {
                            int pos = s.indexOf(rc, i + 1);
                            sb.append(s.substring(i + 1, pos));
                            i = pos;
                        }
                    }
                } else {
                    s = removeTopic(s, T_LOVE);
                    s = removeTopic(s, T_MEDICINE);
                    s = removeTopic(s, T_DECUMB);
                    sb = new StringBuffer(s);
                }
            }
            if (si.type != Event.EV_HELP) {
                for (int i = 0; i < sb.length(); i++) { // do not optimize
                    if (RESERVED_CHARS.indexOf(sb.charAt(i)) >= 0) {
                        sb.deleteCharAt(i--);
                    }
                }
            }
            s = sb.toString();
            s = s.trim();
            if (s.length() == 0) {
                s = Astromaximum.getstr(126);//minor index
            }
            //    System.out.println(">"+s+"<");

            //    if(!s.endsWith(".")){
            //      s+=".";
            //    }

            StringBuffer res = new StringBuffer();
            switch ((int) params[0]) {
                case Event.EV_MOON_DAY:
                    res.append(Astromaximum.getstr(121)).append(params[2]);//moon day#
                    break;
                case Event.EV_NAVROZ:
                    if (params[2] >= 360) {
                        params[2] = 359 - params[2];
                    }
                    res.append(Astromaximum.getstr(120)).append(params[2]);//Sun_day#
                    break;
                case Event.EV_DEG_2ND:
                case Event.EV_DEGPASS0:
                case Event.EV_DEGPASS1:
                case Event.EV_DEGPASS2:
                case Event.EV_DEGPASS3:
                    res.append(Astromaximum.PLANETS[(int) params[1]]).append(" ").append(params[3]).append("\u00b0").append(Astromaximum.CONSTELL[(int) params[4]]);
                    if (params[5] > 0) {
                        res.append(" ").append(Astromaximum.getstr(133 + (int) params[5]));//deg
                    }
                    break;
                case Event.EV_TITHI:
                    res.append(Astromaximum.getstr(122)).append(" #").append(params[2]);//tithi
                    break;
                case Event.EV_VOC:
                    res.append(Astromaximum.getstr(123));//VOC
                    break;
                case Event.EV_VIA_COMBUSTA:
                    res.append("Via Combusta");
                    break;
                case Event.EV_TOP_DAY:
                    res.append(Astromaximum.getstr((int) params[2] - 1)).
                            append(" - ").append(Astromaximum.getstr(27)).append(" ").
                            append(Astromaximum.getstr(40 + SummItem.weekPlanets[(int) params[2] - 1]));//of_
                    break;
                //      case Event.EV_HELP:
                //      case Event.EV_DECUMBITURE:
                //        res="";
                //        break;
                case Event.EV_SIGN_ENTER:
                    res.append(getFullPlanet(params[1])).append(" ").append(Astromaximum.getstr(133) //in
                            ).append(" ").append(Astromaximum.CONSTELL[(int) params[2]]);
                    break;
                case Event.EV_ECLIPSE:
                    res.append(Astromaximum.getstr(143)).append(" ").append( //eclipse
                            Astromaximum.getstr(40 + (int) params[2]));//of_
                    break;
                case Event.EV_PLANET_HOUR:
                    res.append(Astromaximum.getstr(138)).append(" ").append( //Hour
                            Astromaximum.getstr(40 + (int) params[2]));//of_
                    break;
                case Event.EV_MOON_PHASE:
                    //        res.append(Astromaximum.getstr(70+(int)params[2]));//mph
                    break;
                case Event.EV_RISE:
                    res.append("(-40/+28 ").append(Astromaximum.getstr(129)) //min.
                            .append(") ").append(getFullPlanet(params[2])).append(" ").append(getRiseString((int) params[3]));
                    break;
                case Event.EV_MOON_MOVE:
                    //        System.out.println(params[4]);
                    res.append(getFullPlanet(params[4])).append(">").append(params[3] == Event.SE_MOON ? "VOC" : getFullPlanet(params[3]));
                    break;
                case Event.EV_ASP_EXACT:
                    if (params[1] == Event.SE_MOON) {
                        res.append(Astromaximum.PLANETS[(int) params[1]]).append("-").append(params[4]).append("\u00b0-").append(Astromaximum.PLANETS[(int) params[2]]);
                    } else {
                        res.append(Astromaximum.PLANETS[(int) params[2]]).append("-").append(params[5]).append("\u00b0-").append(Astromaximum.PLANETS[(int) params[3]]).append(" ");
                        res.append(Astromaximum.getstr(params[2] <= Event.SE_MARS ? 124 : //+-week
                                125));//long_aspect
                    }
                    break;
                case Event.EV_RETROGRADE:
                    //        System.out.println(params[2]);
                    res.append(getFullPlanet(params[2]));
                    break;
                case Event.EV_ASP_EXACT_MOON:
                    res.append("\u00b16 ").append(Astromaximum.getstr(137));
                    break;
            }
            String ss = "";
            if (params[0] == Event.EV_VOC) {
                if (params[params.length - 4] != 0) {
                    ss = Event.long2String(params[params.length - 4], 0, false) + " - ";
                }
                if (params[params.length - 3] != 0) {
                    ss += Event.long2String(params[params.length - 3], 0, false);
                }
                if (params[params.length - 2] != 0) {
                    ss += "|" + Event.long2String(params[params.length - 2], 0, false) + " - ";
                }
                if (params[params.length - 1] != 0) {
                    ss += Event.long2String(params[params.length - 1], 0, false);
                }
            }
            else {
                if (params[params.length - 2] != 0) {
                    ss = Event.long2String(params[params.length - 2], 0, false);
                }
                if (params[0] == Event.EV_TITHI) {
                    ss += " - (" + Event.long2String(params[params.length - 3], 0, false) + ")";
                }
                if (params[params.length - 1] != 0) { // 2nd date
                    ss += " - ";
                    //      if(Astromaximum.sizer.getSize()==0)
                    //        ss+="\n";
                    ss += Event.long2String(params[params.length - 1], 0, true);
                }
            }
            if (topic != 10 && isTopicTitle) {
                res.append(" <").append(Astromaximum.getstr(50 + topic)) //fb
                        .append(">");
            }
            if (res.length() > 0) {
                res.append(":  ");
            }
            txt = ss + "|" + res.toString() + "|" + s;
        }
        prepareText();
//  final String et="iET="+Long.toString(System.currentTimeMillis()-tick);
//  Astromaximum.log(et);
        return s.length() > 1;
    }

/*
    protected void sizeChanged(int w, int h) {
        Astromaximum.instance.recalcBounds(getWidth(),getHeight());
    }
*/

    public void commandAction(Command c, Displayable d) {
        int priority = c.getPriority();
        if (d == Astromaximum.menu){
            if (c.getCommandType() == Command.OK) {
                priority = Astromaximum.menu.getSelectedIndex() + 1;
            }
        }
        switch (priority) {
            case 1:
                txt = null;
                Astromaximum.summary.dontRender();
                break;
            case 2:
                cycleFontSize();
            case 4:
                Astromaximum.disp.setCurrent(this);
                break;
            case 3:
                Astromaximum.summary.setCurPage(Summary.PAGE_HELP);
                Astromaximum.summary.dontRender();
        }

    }

    private static String getFullPlanet(long i) {
        return Astromaximum.getstr(30 + (int) i);
    }

    private static String getRiseString(int i) {
        return Astromaximum.getstr(80 + i - 1);
    }

    protected void paint(Graphics graphics) {
        graphics.setColor(Astromaximum.CURRENT_MONTH_COLOR);
        graphics.fillRect(0, 0, graphics.getClipWidth(), graphics.getClipHeight());
//#ifdef UseBuffer
//#     osg=Summary.offScreenBuffer.getGraphics();
//#endif
        graphics.setColor(Astromaximum.CURRENT_MONTH_COLOR);
        graphics.fillRect(0, 0, graphics.getClipWidth(), graphics.getClipHeight());

        int wid2 = Summary.imgService.getHeight() / 2;
        SummItem.drawImg(graphics, Summary.imgPanel, 9, wid2 + 1, wid2 + 1, Graphics.VCENTER | Graphics.HCENTER);
        SummItem.drawImg(graphics, Summary.imgService, 0, wid2 * 6 + 1, wid2 + 1,
                Graphics.VCENTER | Graphics.HCENTER);
        graphics.setColor(Astromaximum.BORDER_COLOR);
        graphics.drawRect(0, 0, wid2 *2 + 2, wid2 * 2 + 2);
        graphics.drawRect(wid2 * 5, 0, wid2 *2 + 2, wid2 * 2 + 2);

        graphics.setClip(0, VMARGIN, graphics.getClipWidth(), graphics.getClipHeight() - VMARGIN);
        
        graphics.setColor(0);
        Font oldFont = graphics.getFont();
//        graphics.setFont(Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, fontSize));
        graphics.setFont(Font.getFont(Astromaximum.options.getFontFace(), 
                Font.STYLE_PLAIN, Astromaximum.options.getFontSize(fontSize)));
        
        lineHeight = graphics.getFont().getHeight();
        curY = topLine;
        curX = 0;
        graphics.translate(HMARGIN - graphics.getTranslateX(), VMARGIN - graphics.getTranslateY());
        renderString(graphics, txt);

//#ifdef UseBuffer
//#     graphics.drawImage(Summary.offScreenBuffer, 0, 0, Graphics.LEFT | Graphics.TOP);
//#endif
        graphics.setFont(oldFont);
    }

    private void cycleFontSize() {
        if ((fontSize < 0) || (fontSize >= 2))
            fontSize = 0;
        else
            ++fontSize;
        repaint();
        Astromaximum.options.saveHistory();
    }

    private void renderString(Graphics osg, String s) {
        int len = s.length();
        if (len == 0) {
            return;
        }
        Font fnt = osg.getFont();
        int width = getWidth() - HMARGIN * 2;
        char[] ca = new char[len];
        s.getChars(0, len, ca, 0);
        int spaceW = fnt.charWidth(' ');
        int start = 0, i, cw;
        char curc = 0;
        while (start < len) {
            for (i = start; i < len; i++) {
                curc = ca[i];
                if (curc == ' ' || curc == '|') {
                    break;
                }
            }
            int wlen = i - start;
            cw = fnt.charsWidth(ca, start, wlen);
            if (cw > width) {
                wlen = (width - curX) * wlen / cw;
                curc = '\0';
            }
            else if (curX + cw > width) {
                newLine();
            }
            osg.drawChars(ca, start, wlen, curX, curY, Graphics.LEFT | Graphics.TOP);
            switch (curc) {
                case ' ':
                    curX += cw + spaceW;
                    ++wlen;
                    break;
                case '|':
                    ++wlen;
                case '\0':
                    newLine();
            }
            start += wlen;
        }

        newLine();
    }

    void prepareText() {
        lineCount = topLine = 0;
        Astromaximum.errCode = 6711;
        VMARGIN = Summary.imgService.getHeight() + 6;
        Astromaximum.errCode = 6712;
    }

    private void newLine() {
        curY += Font.getDefaultFont().getHeight();
        curX = 0;
        ++lineCount;
    }

    protected void keyPressed(int keyCode) {
        mousePressed = true;
    }

    protected void keyReleased(int keyCode) {
        if (!mousePressed) return;
        mousePressed = false;
        final int ga = getGameAction(keyCode);
        if (Astromaximum.poundPressed) { // process shifted keys
            Astromaximum.poundPressed = false;
            switch (keyCode) {
                case Canvas.KEY_POUND: // ## does nothing
                    repaint();
                    break;
                case Canvas.KEY_NUM1: // #1 cycles font
                    cycleFontSize();
                    break;
            }
            return;
        }
        switch (ga) {
            case Canvas.UP:
                if (topLine + VMARGIN * 3 < getHeight()) {
                    topLine += lineHeight;
                    repaint();
                }
                break;
            case Canvas.DOWN:
                if (curY > VMARGIN * 2) {
                    topLine -= lineHeight;
                    repaint();
                }
                break;
            default:
                switch (keyCode) {
                    case Canvas.KEY_POUND: // # is like Shift
                        // waiting for next digit key
                        Astromaximum.poundPressed = !Astromaximum.poundPressed;
                        repaint();
                        break;
                    case Canvas.KEY_NUM0:
                        Astromaximum.instance.showMenu(this, cmds);
                        break;
                    default:
//#if logger
        if(isLogged){
///#           Astromaximum.instance.logger("Stopping log...");
///#           isLogged=false;
///#           Astromaximum.summary.stop();
              Astromaximum.LOGGER_SLEEP=0;
        }
        else{
          Astromaximum.summary.dontRender();
        }
//#else
//#                     Astromaximum.summary.dontRender();
//#endif
                }
        }
    }

    protected void pointerPressed(int x, int y) {
        if (y < VMARGIN){
           int wid2 = Summary.imgService.getHeight() / 2;
           x = (x + 2)/ wid2;
           switch (x) {
               case 0:
               case 1:
                   Astromaximum.instance.showMenu(this, cmds);
                   break;
               case 5:
               case 6:
                   Astromaximum.summary.dontRender();
                   break;
           }
        }
        else{
            y -= VMARGIN;
            if (y > topLine && y < curY) {
                topLine += (getHeight() / 2 - y - VMARGIN);
                repaint();
            }
        }
    }

    private String removeTopic(String s, int index) {
        char tch = RESERVED_CHARS.charAt(index);
//    System.out.println(tch);
        int pos = s.indexOf(tch);
        if (pos >= 0) {
            s = s.substring(0, pos) + s.substring(s.indexOf(tch, pos + 1) + 1);
        }
        return s;
    }

    String extractArticle(long[] params) {
        String res = null;
        byte[] interp;
        try {
//#ifdef use_amtext
//#             if(params[0]!=Event.EV_MSG){
//#                 try{
//#                     Integer val=(Integer)hamtext.get(new Integer((int)params[0]));
//#                     Astromaximum.log(val.toString());
//#                     interp=new byte[rs.getRecordSize(val.intValue())];
//#                     interp=rs.getRecord(val.intValue());
//#                     long sum=0;
//#                     for(int j=0; j<interp.length; j++){
//#                         sum+=interp[j];
//#                     }
//#                     Astromaximum.log(new Integer(interp.length).toString()+">"+Long.toString(sum));
//#
//#                 } catch (Exception ex){
//#                     Astromaximum.log("1:"+ex.getMessage());
//#                     return null;
//#                 }
//#             }
//#             else{
//#endif
            InputStream is = getClass().getResourceAsStream(Long.toString(params[0]));
            if (is == null) {
                return null;
            }
            interp = new byte[is.available()];
            is.read(interp);
            is.close();
//#ifdef use_amtext
//#             }
//#endif
            DataInputStream dis = new DataInputStream(
                    new ByteArrayInputStream(interp));

//      while(true){
            final int evt = dis.readUnsignedShort();
            final int partsz = dis.readInt();
            if (evt != params[0]) {
                dis.skip(partsz - 6);
//          continue;
//          return res;
            }
            final int plt = dis.readByte();
            if (plt != -1 && plt != params[1]) {
                dis.skip(partsz - 7);
//          continue;
//          return res;
            }
            final int paramcount = dis.readShort();
            int recnum = dis.readUnsignedShort();
            while (recnum > 0) {
                boolean f = true;
                for (int j = 0; j < paramcount; j++) {
                    final int rsh = dis.readUnsignedShort();
                    if (params[j + 2] != rsh) {
                        f = false;
                        dis.skip(2 * (paramcount - j - 1));
                        break;
                    }
                }
                if (f) {
                    res = dis.readUTF();
                    break;
                }
                dis.skip(dis.readUnsignedShort());
                --recnum;
            }
//      }
        } catch (IOException ex) {
            Astromaximum.log("2:" + ex.toString());
        }
        return res;
    }
}

// # vi:et:ts=4:sw=4
