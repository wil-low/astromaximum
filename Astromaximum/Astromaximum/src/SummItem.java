/*
 * Event.EV_java
 *
 * Created on 25 ������ 2006, 18:01
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

/**
 *
 * @author Administrator
 * @noinspection CastToConcreteClass
 */
//#define aspswap
import javax.microedition.lcdui.*;
import java.util.*;
import javax.microedition.rms.RecordFilter;

final class SummItem extends TimerTask implements RecordFilter {

    private static final int XLEFT = 0;
    private static final int XCENTER = 1;
    private static final int XRIGHT = 2;
    private short[] widths;
    private int rowCount = 1;
    byte tag;
    private short[] nav = null;
    Event[] events;
    final int left;
    final int top;
    final int width;
    final int height;
    final int type;
    static byte[] places;
    int selIndex;
    int nowSelection = -1;
    int cusSelection = -1;
    private int page;
    //#if "imeiCheck" @ protection
    static int hj;
    //#endif
    private String str;
    private static Image tithi;
    static final Vector moonMoveVec = new Vector();
    private static final long DEGREE_DELTA_MSEC1 = -40 * 60 * 1000;
    private static final long DEGREE_DELTA_MSEC2 = 28 * 60 * 1000;
    static Summary owner;
    private static final int[] ASP_ANGLES = {0, 180, 90, 120, 60, 45, 30, 15, 72, 150};
    private static final int[] OWN_SIGN_REVERSE = {
        Event.SE_MARS, Event.SE_VENUS, Event.SE_MERCURY, Event.SE_MOON, Event.SE_SUN, Event.SE_MERCURY,
        Event.SE_VENUS, Event.SE_MARS, Event.SE_JUPITER, Event.SE_SATURN, Event.SE_SATURN, Event.SE_JUPITER
    };
    static final byte[] weekPlanets = {0, 1, 4, 2, 5, 3, 6};
    private static final Hashtable topics = new Hashtable();

    static {
        topics.put(new Integer(Event.EV_MOON_DAY), "}#");
        topics.put(new Integer(Event.EV_ASP_EXACT), "~*^$}>@");
        topics.put(new Integer(Event.EV_VIA_COMBUSTA), "{~#*^$}>@");
        topics.put(new Integer(Event.EV_NAVROZ), ">");
        topics.put(new Integer(Event.EV_RISE), "}~#*^");
        topics.put(new Integer(Event.EV_ASP_EXACT_MOON), "*~#^$}>@={");
        topics.put(new Integer(Event.EV_MOON_PHASE), "^$}~#*>@");
        topics.put(new Integer(Event.EV_VOC), "~*^$}>@");
        topics.put(new Integer(Event.EV_HELP1), "*");
        topics.put(new Integer(Event.EV_SIGN_ENTER), "*}{~=^");
        topics.put(new Integer(Event.EV_RETROGRADE), "*^$}>@");
        topics.put(new Integer(Event.EV_DECUMBITURE), "#");
        topics.put(new Integer(Event.EV_TITHI), "}{~*^$>@=");
        topics.put(new Integer(Event.EV_MOON_MOVE), "*^$}>@");
        topics.put(new Integer(Event.EV_TOP_DAY), "^}{~=$");
        topics.put(new Integer(Event.EV_ECLIPSE), "*^$}>@");
        topics.put(new Integer(Event.EV_PLANET_HOUR), "*^$}{~#=");
///////////////
        topics.put(new Integer(Event.EV_MOON_MOVE), "*^${}>@=~");
        topics.put(new Integer(Event.EV_MOON_SIGN_LARGE), topics.get(new Integer(Event.EV_SIGN_ENTER))); //EV_SIGN_ENTER
        topics.put(new Integer(Event.EV_DAY_HOURS), topics.get(new Integer(Event.EV_PLANET_HOUR))); //EV_PLANET_HOUR
        topics.put(new Integer(Event.EV_NIGHT_HOURS), topics.get(new Integer(Event.EV_PLANET_HOUR)));//EV_PLANET_HOUR
        topics.put(new Integer(Event.EV_SUN_RISE), "#*^}~");
        topics.put(new Integer(Event.EV_MOON_RISE), "#*^}~");
        topics.put(new Integer(Event.EV_SUN_DAY), topics.get(new Integer(Event.EV_NAVROZ))); //EV_NAVROZ
    }

    SummItem(int tt) {
        left = top = width = height = page = 0;
        type = tt;
    }

    /**
     * Creates a new instance of SummItem
     *
     * @param widCount
     * @param rowCount
     * @param top
     * @param w
     * @param page
     * @param left
     * @param h
     * @param type
     * @param nleft
     * @param nright
     * @param nup
     * @param ndown
     * @noinspection AssignmentToMethodParameter,NestedAssignment
     */
    SummItem(int left, int top, int w, int h, int page, int widCount, int rowCount, int type,
            int nleft, int nright, int nup, int ndown) {
        this.type = type;
        this.left = left;
        this.top = top;
        nav = new short[4];
        nav[0] = (short) nleft;
        nav[1] = (short) nright;
        nav[2] = (short) nup;
        nav[3] = (short) ndown;
        width = w;
        height = h;
        this.page = page;
        if (type == Event.EV_TOPIC_BUTTON || type == Event.EV_ECLIPSE ||
                type == Event.EV_MOON_PHASE || type == Event.EV_HELP) {
            tag = (byte) rowCount;
            rowCount = 1;
        }
        if (rowCount == 0) {
            this.rowCount = widCount;
        }

        if (widCount > 0) {
            events = new Event[widCount];
            widths = new short[widCount / this.rowCount];
            widths[0] = 1000;
        }
        switch (type) {
            case Event.EV_HELP:
                for (int i = 0; i < widCount; i++) {
                    setEvents(i, new Event(0, 0));
                }
                break;
            case Event.EV_STATUS:
                widths[0] = 800;
                widths[1] = 200;
                break;
            case Event.EV_TOP_MONTH:
            case Event.EV_TOP_DAY:
                widths[0] = widths[2] = 150;
                widths[1] = 700;
                selIndex = 1;
                break;
        }
    }

    void initString() {
        StringBuffer sb=new StringBuffer();
        switch (type) {
            case Event.EV_TOP_MONTH:
                final Calendar cal = Astromaximum.calendar;
                cal.setTime(new Date(events[1].date0));
                selIndex = 1;
                sb.append(Astromaximum.months[cal.get(Calendar.MONTH)]).append("'").
                        append(Integer.toString(cal.get(Calendar.YEAR)).substring(2, 4));
                break;
            case Event.EV_TOP_DAY:
                selIndex = 1;
                sb.append(Astromaximum.localizedDateString(new Date(events[1].date0)));
                break;
            case Event.EV_VOC:
            case Event.EV_VIA_COMBUSTA:
                if (events[0] != null) {
                    sb.append(events[0].getDateString(0, 1)).append("-").append(events[0].getDateString(1, 1));
                }
                break;
            case Event.EV_SUN_RISE:
            case Event.EV_MOON_RISE:
                if (events[0] != null) {
                    sb.append(events[0].getDateString(0, 1)); // rise time
                }
//        if(events[1]!=null) {
//          str += events[1].getDateString(0, true); // rise time
//        }
                break;
            case Event.EV_STATUS:
                final SummItem si = owner.getSelectedItem();
                if (si.type != Event.EV_STATUS) {
                    sb.append(si.getStatus());
                }
                if (sb.length() == 0) {
                    sb.append(Event.long2String(owner.noonTime, 1, false));
                    tag = 1;
                } else {
                    tag = 0;
                }
//        System.out.println(">>"+str);
//        System.out.println(owner.cusTime);
            case Event.EV_DECUMBITURE:
                break;
//      case Event.EV_DECUMB_BEGIN:
//      case Event.EV_DEG_2ND:
//        if(events==null){
//          break;
//        }
            default:
//        try{
                final int sz = events.length;
                widths = new short[sz];
                for (int i = 0; i < sz; i++) {
                    widths[i] = (short) (1000 / sz);
                }
//        }
//        catch(Exception e){
//          System.out.println("Exception!!!!");
//          dump();
//        }
                break;
        }
        try {
            if (selIndex >= events.length) {
                selIndex = events.length - 1;
            }
            if (selIndex < 0) {
                selIndex = 0;
            }
        } catch (NullPointerException npe) {
//          System.out.println("Exception!!!!");
//          dump();
            selIndex = 0;
        }
        str=sb.toString();
    }

    /**
     * @param osg
     * @param isSelected
     * @param now
     * @param isCus
     * @noinspection ValueOfIncrementOrDecrementUsed,ProhibitedExceptionCaught
     */
    void render(Graphics osg, boolean isSelected, long now, boolean isCus) {
//    if((page%2)>0) // on 1st page
        boolean timeoff = Options.isRealtimeOff;
        int fontHeight = osg.getFont().getHeight();
        if (owner.pageNum != Summary.PAGE_PANEL && type != Event.EV_RISE) {

            if (!isEmpty() && haveTopic(type)) {
                osg.setColor(Astromaximum.TOPIC_COLOR);
                switch (type) {
                    case Event.EV_TOP_DAY:
                        osg.fillRect(getX(1, XLEFT), top + 1, width * widths[1] / 1000 - 1, height - 2);
                        break;
                    case Event.EV_MOON_MOVE:
                        for (int i = 0; i < events.length; i += 2) {
                            osg.fillRect(getX(i, XLEFT), top + 1, width * widths[i] / 1000 - 1, height - 2);
                        }
                        break;
                    default:
                        osg.fillRect(left + 1, top + 1, width - 2, height - 2);
                }
            }
        }
        int x100 = 0;
//    if(!isSelected){
        osg.setColor(Astromaximum.BORDER_COLOR);
        //    else
        //      osg.setColor(isSelected? Astromaximum.BACK_COLOR: Astromaximum.BORDER_COLOR);
        if (type != Event.EV_PANEL) {
            if (rowCount == 1) {
                if (widths.length > 0) {
                    for (int i = 0; i < widths.length; i++) {
                        osg.drawRect(left, top, width * x100 / 1000, height);
                        x100 += widths[i];
                    }
                }
            } else {
                x100 = height * 1000 / rowCount;
                int yy = 0;
                for (int i = 0; i < rowCount; i++) {
                    osg.drawRect(left, top + yy / 1000, width, x100 / 1000);
                    yy += x100;
                }
            }
            osg.drawRect(left, top, width, height);
        }
//    }
        if (events.length == 0) {
            if (type == Event.EV_ASP_EXACT && isSelected) {
                osg.setColor(0);
                osg.drawString(Astromaximum.getstr(146), left + width / 2,//Week mode
                        top + height - 2, Graphics.BASELINE | Graphics.HCENTER);
            }
            return;
        }
        if (!timeoff && isSelected) {
            osg.setColor(Astromaximum.SELECTION_COLOR);
//      osg.setStrokeStyle(Graphics.DOTTED);
            int l1, t1, w1, h1;

            if (rowCount == 1) {
                int xx = 0;
                for (int i = 0; i < selIndex; i++) {
                    xx += widths[i];
                }
                l1 = left + xx * width / 1000 + 1;
                t1 = top + 1;
                w1 = widths[selIndex] * width / 1000 - 2;
                h1 = height - 2;
            } else {
                l1 = left + 1;
                t1 = top + 1 + selIndex * height / rowCount;
                w1 = width - 2;
                h1 = height / rowCount - 2;
            }
            if ((page & 6) > 0) {
                osg.fillRect(l1, t1, w1, h1);
            } else {
                osg.drawRect(l1, t1, w1, h1);
                osg.drawRect(l1 - 2, t1 - 2, w1 + 4, h1 + 4);
            }
        }
        int y = top + height / 2;
        osg.setColor(0);
        final int xr;
        switch (type) {
            case Event.EV_HELP:
                switch (tag) {
                    case 1:
                        osg.setColor(Astromaximum.RUBY_COLOR);
//                        Font old = osg.getFont();
                        osg.drawString(Astromaximum.URL, width / 2, top - 2,
                                Graphics.BOTTOM | Graphics.HCENTER);
//                        osg.setFont(old);
                    case 2:
                        for (int i = 0; i < events.length; i++) {
                            drawImg(osg, Summary.imgPlanet, (tag == 1) ? i : i + 7, getX(i, XCENTER), y,
                                    Graphics.VCENTER | Graphics.HCENTER);
                        }
                        break;
                    case 3:
                    case 4:
                        for (int i = 0; i < events.length; i++) {
                            drawImg(osg, Summary.imgZodiac, (tag == 3) ? i : i + 6, getX(i, XCENTER), y,
                                    Graphics.VCENTER | Graphics.HCENTER);
                        }
                        break;
                    case 5:
                        osg.drawString(Astromaximum.getstr(166), getX(0, XCENTER), top + height - 1,
                                Graphics.BASELINE | Graphics.HCENTER);
                        for (int i = 1; i < events.length; i++) {
                            osg.drawString(Interpreter.riseKeys[i - 1], getX(i, XCENTER), top + height - 1,
                                    Graphics.BASELINE | Graphics.HCENTER);
                        }
                        break;
                    case 6:
                        for (int i = 0; i < 4; i++) {
                            owner.drawPhase(osg, getX(i + 1, XCENTER) - Summary.IMG_HEIGHT / 2,
                                    y - Summary.IMG_HEIGHT / 2, Summary.IMG_HEIGHT, i);
                        }
                        osg.drawString("VOC", getX(0, XCENTER), top + height - 1,
                                Graphics.BASELINE | Graphics.HCENTER);
                        osg.drawString("VC", getX(5, XCENTER), top + height - 1,
                                Graphics.BASELINE | Graphics.HCENTER);
                        break;
                    case 7:
                        for (int i = 0; i < 5; i++) {
                            drawImg(osg, Summary.imgAspect, i, getX(i, XCENTER), y,
                                    Graphics.VCENTER | Graphics.HCENTER);
                        }
                        drawImg(osg, Summary.imgService, 1, getX(5, XCENTER), y,
                                Graphics.VCENTER | Graphics.HCENTER);
                        drawImg(osg, Summary.imgService, 3, getX(6, XCENTER), y,
                                Graphics.VCENTER | Graphics.HCENTER);
                        drawImg(osg, Summary.imgService, 4, getX(7, XCENTER), y,
                                Graphics.VCENTER | Graphics.HCENTER);
                        break;
                    case 8:
                        osg.drawString(Astromaximum.getstr(122), getX(0, XCENTER), top + height - 1,//Tithi
                                Graphics.BASELINE | Graphics.HCENTER);
                        osg.drawString(Astromaximum.getstr(127), getX(1, XCENTER), top + height - 1,//S.D.
                                Graphics.BASELINE | Graphics.HCENTER);
                        osg.drawString(Astromaximum.getstr(128), getX(2, XCENTER), top + height - 1,//M.D.
                                Graphics.BASELINE | Graphics.HCENTER);
                        drawImg(osg, Summary.imgAspect, 10, getX(3, XCENTER), y,
                                Graphics.VCENTER | Graphics.HCENTER);
                        drawImg(osg, Summary.imgAspect, 11, getX(4, XCENTER), y,
                                Graphics.VCENTER | Graphics.HCENTER);
                        break;
                    case 9:
                        for (int i = 0; i < events.length; i++) {
                            drawImg(osg, Summary.imgPanelSmall, i, getX(i, XCENTER), y,
                                    Graphics.VCENTER | Graphics.HCENTER);
                        }
                }
                break;
            case Event.EV_TOP_MONTH:
            case Event.EV_TOP_DAY:
                osg.drawString(str, getX(1, XCENTER), top + (height - fontHeight) / 2,
                        Graphics.TOP | Graphics.HCENTER);
                if (isSelected) {
                    for (int i = 0; i <= 2; i += 2) {
                        drawImg(osg, Summary.imgService, i, getX(i, XCENTER), y,
                                Graphics.VCENTER | Graphics.HCENTER);
                    }
                }
                break;
            case Event.EV_VOC:
            case Event.EV_VIA_COMBUSTA:
                if (nowSelection == 0) {
                    osg.setColor(Astromaximum.RUBY_COLOR);
                }
                if (isCus && cusSelection == 0) {
                    osg.setColor(Astromaximum.CUST_COLOR);
                }
                osg.drawString(type == Event.EV_VIA_COMBUSTA ? "vc" : "voc", getX(0, XLEFT) + 3, top + 1,
                        Graphics.TOP | Graphics.LEFT);
                if (str != null) {
                    osg.drawString(str, getX(0, XRIGHT) - 1, top + 1, Graphics.TOP | Graphics.RIGHT);
                }
                break;
            case Event.EV_SUN_RISE:
                drawRiseSetCell(osg, now);
                break;
            case Event.EV_SUN_DEGREE_LARGE:
                Event ev = events[0];
                xr = getX(0, XLEFT);
                String tmp = Integer.toString(Astromaximum.getSignDegree(ev.getDegree())) + "\u00b0";
                osg.drawString(tmp, xr + 1, top, Graphics.TOP | Graphics.LEFT);
                osg.setColor(0);
                drawImg(osg, Summary.imgZodiac, ev.getDegree() / 30,
                        xr + osg.getFont().stringWidth(tmp) + 2, top + 1, Graphics.TOP | Graphics.LEFT);
                drawSignString(osg, ev, now, isCus, xr);
                break;
            case Event.EV_MOON_RISE:
                drawRiseSetCell(osg, now);
                break;
            case Event.EV_MOON_SIGN_LARGE:
                ev = events[0];
                xr = getX(0, XLEFT);
                if (ev != null) {
                    drawImg(osg, Summary.imgZodiac, ev.getDegree(),
                            xr + 1, top + 1, Graphics.TOP | Graphics.LEFT);
                    drawSignString(osg, ev, now, isCus, xr);
                }
                break;
            case Event.EV_MOON_PHASE:
                if (tag == 1) {
                    osg.drawImage(tithi, left + width / 2, top + height / 2,
                            Graphics.VCENTER | Graphics.HCENTER);
                } else {
                    owner.drawPhase(osg, left + width / 2 - Summary.IMG_HEIGHT / 2,
                            top + height / 2 - Summary.IMG_HEIGHT / 2, Summary.IMG_HEIGHT, events[0].planet1);
                }
                break;
            case Event.EV_TITHI:
                for (int i = 0; i < events.length; i++) {
                    x100 = getX(i, XCENTER);
                    osg.setColor(0);
                    if (nowSelection == i) {
                        osg.setColor(Astromaximum.RUBY_COLOR);
                    }
                    if (isCus && cusSelection == i) {
                        osg.setColor(Astromaximum.CUST_COLOR);
                    }
                    osg.drawString(Integer.toString(events[i].getDegree()),
                            x100,  top + (height - fontHeight) / 2,
                            Graphics.TOP | Graphics.HCENTER);
                }
                break;
            case Event.EV_MOON_MOVE:
                final int ww = width * widths[0] / 1000 - 3;
                if (events.length == 0) {
                    break;
                }
                osg.setColor(Astromaximum.DIMMED_COLOR);
                osg.fillRect(getX(0, XLEFT) + 2, top + 2, ww, height - 4);
                osg.fillRect(getX(events.length - 1, XLEFT) + 2, top + 2, ww - 1, height - 4);
                for (int i = 0; i < events.length; i++) {
                    ev = events[i];
                    x100 = getX(i, XCENTER);
                    if (ev.getDegree() == 200) {// this is transition > , not aspect
                        int imgid = 4;
                        if (nowSelection == i) {
                            imgid = 5;
                        }
                        if (isCus && cusSelection == i) {
                            imgid = 6;
                        }
                        drawImg(osg, Summary.imgService, imgid, x100, y,
                                Graphics.VCENTER | Graphics.HCENTER);
                    } else if (ev.planet1 == Event.SE_MOON) { // this is sign enter, not aspect
                        drawImg(osg, Summary.imgZodiac, ev.getDegree(), x100, y,
                                Graphics.VCENTER | Graphics.HCENTER);
                    } else {
                        drawImg(osg, Summary.imgPlanet, ev.planet1, x100, y,
                                Graphics.BOTTOM | Graphics.HCENTER);
                        drawImg(osg, Summary.imgAspect, getAspIndex(
                                ev.getDegree()), x100, y + 1,
                                Graphics.TOP | Graphics.HCENTER);
                    }
                }
                break;
            case Event.EV_SEL_DEGREES:
                for (int i = 0; i < events.length; i++) {
                    boolean isHighlighted = false;
                    Event e = events[i];
                    drawSelDegree(osg, e, getX(i, XCENTER), y,
                            Graphics.VCENTER | Graphics.HCENTER);
                    if (contains(e, now)) {
                        osg.setColor(Astromaximum.RED_COLOR);
                        isHighlighted = true;
                    }
                    if (isCus && contains(e, owner.cusTime)) {
                        osg.setColor(Astromaximum.CUST_COLOR);
                        isHighlighted = true;
                    }
                    if (isHighlighted) {
                        final int x = getX(i, XLEFT);
                        osg.drawRect(x + 2, top, getX(i, XRIGHT) - x - 4, height - 1);
                    }
                }
                break;
            case Event.EV_RETROGRADE:
                for (int i = 0; i < events.length; i++) {
                    final Event e = events[i];
                    final int x = getX(i, XCENTER);
                    drawImg(osg, Summary.imgPlanet, e.planet0, x, y,
                            Graphics.VCENTER | Graphics.HCENTER);
                    drawImg(osg, Summary.imgService, 1, x - 1, y + 3,
                            Graphics.VCENTER | Graphics.LEFT);
                }
                break;
            case Event.EV_ASP_EXACT:
                for (int i = 0; i < events.length; i++) {
                    drawAspect(osg, events[i], getX(i, XCENTER), y, Graphics.VCENTER);
                }
                break;
            case Event.EV_NIGHT_HOURS:
                osg.setColor(Astromaximum.DIMMED_COLOR);
            case Event.EV_DAY_HOURS:
                for (int i = 0; i < 12; i++) {
                    int plt = events[i].planet0;
                    if (type == Event.EV_NIGHT_HOURS) {
                        plt += 13;
                    }
                    if (events[i].date1 >= Summary.period1) {
                        int xx = getX(i, XLEFT) + 2;
                        osg.fillRect(xx, top + 2, getX(i, XRIGHT) - xx - 2, height - 3);
                    }
                    drawImg(osg, Summary.imgOpaq, plt, getX(i, XCENTER),
                            i % 2 > 0 ? y + 2 : y - 1, Graphics.VCENTER | Graphics.HCENTER);
                }
                for (int i = 0; i < events.length; i++) {
                    if (isCus && cusSelection == i) {
                        osg.setColor(Astromaximum.CUST_COLOR);
                    } else if (nowSelection == i) {
                        osg.setColor(Astromaximum.RED_COLOR);
                    } else {
                        continue;
                    }
                    final int x = getX(i, XLEFT);
                    osg.drawRect(x, top + 1, getX(i, XRIGHT) - x, height - 2);
                }
                break;
            case Event.EV_PANEL:
                drawImg(osg, Summary.imgPanel, 9, left + width / 2, top + height / 2, Graphics.HCENTER | Graphics.VCENTER);
                break;
            case Event.EV_BACK:
                drawImg(osg, Summary.imgService, 0, left + width / 2, top + height / 2, Graphics.HCENTER | Graphics.VCENTER);
                break;
            case Event.EV_TOPIC_BUTTON:
                osg.setColor(Interpreter.topic == tag ? Astromaximum.BLUE_COLOR : Astromaximum.SEA_COLOR);
                osg.fillRect(left + 2, top + 2, width - 4, height - 4);
                drawImg(osg, Summary.imgPanel, tag, left + width / 2, top + height / 2,
                        Graphics.HCENTER | Graphics.VCENTER);
                break;
            case Event.EV_STATUS:
                int colo = 0;
                if (tag == 1) {
                    colo = Astromaximum.CUST_COLOR;
                }

                final Font old = osg.getFont();
                y = top + height - 2;
                int x = getX(0, XCENTER);
                if (str != null) {
                    if (str.length() >= 9 && str.charAt(2) != ':' && str.charAt(8) == ':') {
                        int strw = old.stringWidth(str);
                        if (strw > width)
                            x = (width - strw) / 2;
                        else
                            x -= strw / 2;
                        String ss = str.substring(0, 5);
                        osg.setColor(Astromaximum.SELECTION_COLOR);
                        osg.drawString(ss, x, y, Graphics.LEFT | Graphics.BASELINE);
                        x += old.stringWidth(ss);
                        osg.setColor(colo);
                        ss = str.substring(5, 11);
                        osg.drawString(ss, x, y, Graphics.LEFT | Graphics.BASELINE);
                        if (str.length() > 11) {
                            int space = 1;
                            if (Summary.IMG_HEIGHT == 12) {
                                space += 2;
                            }
                            x += old.stringWidth(ss);
                            ss = str.substring(11, 11 + space);
                            osg.drawString(ss, x, y, Graphics.LEFT | Graphics.BASELINE);
                            x += old.stringWidth(ss);
                            ss = str.substring(11 + space, 17 + space);
                            osg.setColor(Astromaximum.SELECTION_COLOR);
                            osg.drawString(ss, x, y, Graphics.LEFT | Graphics.BASELINE);
                            x += old.stringWidth(ss);
                            osg.setColor(colo);
                            ss = str.substring(17 + space);
                            osg.drawString(ss, x, y, Graphics.LEFT | Graphics.BASELINE);
                            if (strw > width)
                                break; // do not draw current time
                        }
                    } else {
                        osg.drawString(str, x, y, Graphics.HCENTER | Graphics.BASELINE);
                    }
                }
                osg.setFont(old);

                if (timeoff) {
                    int w = owner.getWidth();
                    osg.setColor(Astromaximum.BORDER_COLOR);
                    osg.fillRect(w / 2, top, w /2 , height);
                    osg.setFont(Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, Font.SIZE_MEDIUM));
                    osg.setColor(0);
                    osg.drawString(Astromaximum.options.getCurrentCity(true), w * 3 / 4, y - 2 , Graphics.HCENTER | Graphics.BASELINE);
                    osg.setFont(old);
                }
                else
                if (Summary.isInCurrentDay(now)) {
                    osg.setColor(Astromaximum.RUBY_COLOR);
                    if (Summary.IMG_HEIGHT == 12) {
                        osg.setFont(Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_PLAIN, Font.SIZE_LARGE));
                    }
                    osg.drawString(Event.long2String(now, 1, false),
                            getX(1, XCENTER), y, Graphics.HCENTER | Graphics.BASELINE);
                    osg.setFont(old);
                }
//        osg.setColor(Astromaximum.DIMMED_COLOR);
//        osg.drawString(Long.toString(Runtime.getRuntime().freeMemory()>>10)+"k",
//            owner.getWidth()-1,top,Graphics.RIGHT|Graphics.BOTTOM);
                break;
            case Event.EV_SUN_DAY:
            case Event.EV_MOON_DAY:
                for (int i = 0; i < events.length; i++) {
                    osg.setColor(0);
                    if (nowSelection == i) {
                        osg.setColor(Astromaximum.RUBY_COLOR);
                    }
                    if (isCus && cusSelection == i) {
                        osg.setColor(Astromaximum.CUST_COLOR);
                    }
                    int day = events[i].getDegree();
                    if (day >= 360) {
                        day = -(day - 359);
                    }
                    osg.drawString(Integer.toString(day),
                            getX(i, XCENTER), top + (height - fontHeight) / 2,
                            Graphics.TOP | Graphics.HCENTER);
                }
                break;
            case Event.EV_ZODIAC_SIGN:
                for (int i = 0; i < widths.length; i++) {
                    drawImg(osg, Summary.imgZodiac, i, getX(i, XCENTER), top + 1,
                            Graphics.TOP | Graphics.HCENTER);
                }
                break;
            case Event.EV_ECLIPSE:
                try {
                    int xxx = left - Summary.IMG_WIDTH * 3 / 2;
                    int plt = tag & 1;
                    if ((tag & 6) != 6) {
                        drawImg(osg, Summary.imgPlanet, plt, xxx, top + 1,
                                Graphics.TOP | Graphics.LEFT);
                    }
                    int pll = events[0].planet1;
                    if ((tag & 4) != 0) {
                        drawImg(osg, Summary.imgAspect, plt + 10, left + width / 2, top + height / 2,
                                Graphics.HCENTER | Graphics.VCENTER);
                        pll = 2;
                    } else {
                        xxx = left + width / 2 - Summary.IMG_HEIGHT / 2;
                    }
                    if ((tag & 2) != 0) {
                        owner.drawPhase(osg, xxx,
                                top + height / 2 - Summary.IMG_HEIGHT / 2,
                                Summary.IMG_HEIGHT, pll);
                    }
                } catch (NullPointerException npe) {
                }
                break;
            case Event.EV_RISE:
//        ev=events[0];
                xr = left + width - 2;
//        Astromaximum.drawImg(osg,Summary.imgPlanet,events[2].planet0,getX(0,XLEFT)+1,
//            top+1,Graphics.TOP|Graphics.LEFT);
////        if(ev!=null){
//        if(ev.planet0==Event.SE_MOON){
//          Astromaximum.drawImg(osg,Summary.imgZodiac,ev.getDegree(),
//              xr-1,top+2, Graphics.TOP | Graphics.RIGHT);
//        }
//        else{
//          Astromaximum.drawImg(osg,Summary.imgZodiac,ev.getDegree()/30,
//              xr-1,top+2, Graphics.TOP | Graphics.RIGHT);
//          osg.drawString(Integer.toString(Astromaximum.getSignDegree(ev.getDegree()))+"\u00b0",
//              xr-Summary.IMG_WIDTH-1,top, Graphics.TOP|Graphics.RIGHT);
//        }
                for (int i = 0; i < events.length; i++) {
                    ev = events[i];
                    if (nowSelection == i) {
                        osg.setColor(Astromaximum.RUBY_COLOR);
                    }
                    if (isCus && cusSelection == i) {
                        osg.setColor(Astromaximum.CUST_COLOR);
                    }
                    osg.drawString(ev.getDateString(0, 1),
                            xr, top + height * (i + 1) / rowCount - 2, Graphics.BASELINE | Graphics.RIGHT);
                    osg.drawString(Interpreter.riseKeys[ev.getDegree() - 1],
                            left + 2, top + height * (i + 1) / rowCount - 2, Graphics.BASELINE | Graphics.LEFT);
                    osg.setColor(0);
                }
                break;
            case Event.EV_DEG_2ND:
                ev = events[0];
                final boolean isMoon = ev.planet0 == Event.SE_MOON;
                drawImg(osg, Summary.imgPlanet, events[0].planet0, getX(0, XLEFT) - 1,
                        top + height - 1, Graphics.BOTTOM | Graphics.RIGHT);
                for (int i = 0; i < events.length; i++) {
                    ev = events[i];
                    if (isMoon) {
                        drawImg(osg, Summary.imgZodiac, ev.getDegree(),
                                getX(i, XCENTER), top + height - 1, Graphics.BOTTOM | Graphics.HCENTER);
                        if (isCus && cusSelection == i) {
                            osg.setColor(Astromaximum.CUST_COLOR);
                        } else if (nowSelection == i) {
                            osg.setColor(Astromaximum.RED_COLOR);
                        } else {
                            continue;
                        }
                        x = getX(i, XLEFT);
                        osg.drawRect(x, top + 1, getX(i, XRIGHT) - x, height - 1);
                    } else {
                        if (nowSelection == i) {
                            osg.setColor(Astromaximum.RUBY_COLOR);
                        }
                        if (isCus && cusSelection == i) {
                            osg.setColor(Astromaximum.CUST_COLOR);
                        }
                        osg.drawString(Integer.toString(Astromaximum.getSignDegree(ev.getDegree())),
                                getX(i, XCENTER), top + height - 2, Graphics.BASELINE | Graphics.HCENTER);
                        osg.setColor(0);
                    }
                }
                if (!isMoon) {
                    drawImg(osg, Summary.imgZodiac, events[events.length - 1].getDegree() / 30,
                            left + width, top + height - 1, Graphics.BOTTOM | Graphics.LEFT);
                }
                break;
            case Event.EV_WEEK_GRID:
                drawWeek(osg, isSelected, now);
                break;
            case Event.EV_MONTH_GRID:
                drawMonth(osg, isSelected, now);
                break;
            case Event.EV_DECUMBITURE:
                osg.drawString(/*Event.long2String(events[0].date0,false,false)+" "+*/
                        Astromaximum.getstr(60 + Summary.decumbKeys[events[0].planet0]),//dec
                        left + 3, top + height - 2, Graphics.LEFT | Graphics.BASELINE);
                break;
            case Event.EV_DECUMB_ASPECT:
                for (int i = 0; i < events.length; i++) {
                    ev = events[i];
                    x = getX(i, XCENTER);
                    drawImg(osg, Summary.imgAspect, getAspIndex(ev.getDegree()), x, y,
                            Graphics.VCENTER | Graphics.RIGHT);
                    drawImg(osg, Summary.imgPlanet, ev.planet1, x, y,
                            Graphics.VCENTER | Graphics.LEFT);
                }
                break;
            case Event.EV_DECUMB_BEGIN:
                x = getX(0, XCENTER);
//        drawImg(osg,Summary.imgPlanet,Event.SE_MOON,x,y,Graphics.VCENTER|Graphics.RIGHT);
                if (events[0] != null) {
                    osg.drawString(Integer.toString(events[0].getDegree()), x + 1, top + height - 1,
                            Graphics.BASELINE | Graphics.LEFT);
                }
                if (events[1] != null) {
                    drawImg(osg, Summary.imgPlanet, events[1].planet0, getX(1, XCENTER),
                            y, Graphics.VCENTER | Graphics.HCENTER);
                }
                for (int i = 2; i < events.length; i++) {
                    ev = events[i];
                    if (ev != null) {
                        x = getX(i, XCENTER);
                        drawImg(osg, Summary.imgPlanet, ev.planet0, x, y, Graphics.VCENTER | Graphics.RIGHT);
                        osg.drawString(Interpreter.riseKeys[ev.getDegree() - 1], x + 1, top + height - 1,
                                Graphics.BASELINE | Graphics.LEFT);
                    }
                }
                break;
            case Event.EV_ASCAPHETICS:
                x = left + width / 2;
                osg.setColor(Astromaximum.BORDER_COLOR);
                osg.drawLine(x, top, x, top + height);
                osg.setColor(0);
                for (int i = 0; i < events.length; i++) {
                    ev = events[i];
                    y = top + height * i / rowCount + 1;
                    int si0 = ev.degree & 0xf, si1 = (ev.degree & 0xf0) >> 4;
                    drawImg(osg, Summary.imgZodiac, si0, x - 3, y, Graphics.TOP | Graphics.RIGHT);
                    drawImg(osg, Summary.imgZodiac, si1, x + 3, y, Graphics.TOP | Graphics.LEFT);
                    osg.drawString(Event.long2String(ev.date0, 1, false), x - 3 - Summary.IMG_WIDTH * 2, y,
                            Graphics.TOP | Graphics.RIGHT);
                    osg.drawString(Event.long2String(ev.date1, 1, true), x + 3 + Summary.IMG_WIDTH * 2, y,
                            Graphics.TOP | Graphics.LEFT);
                    y += 2 + Summary.IMG_WIDTH;
                    boolean peregr = (ev.planet0 & 0x80) != 0;
                    drawImg(osg, peregr ? Summary.imgOpaq : Summary.imgPlanet,
                            OWN_SIGN_REVERSE[si0] + (peregr ? 13 : 0), x - 3 - Summary.IMG_WIDTH, y, Graphics.TOP | Graphics.RIGHT);
                    peregr = (ev.planet1 & 0x80) != 0;
                    drawImg(osg, peregr ? Summary.imgOpaq : Summary.imgPlanet,
                            OWN_SIGN_REVERSE[si1] + (peregr ? 13 : 0), x + 3 + Summary.IMG_WIDTH, y, Graphics.TOP | Graphics.LEFT);
                    osg.drawString(Integer.toString((ev.planet0 & 0x7f) - 64), x - 3 - Summary.IMG_WIDTH * 4, y,
                            Graphics.TOP | Graphics.LEFT);
                    osg.drawString(Integer.toString((ev.planet1 & 0x7f) - 64), x + 3 + Summary.IMG_WIDTH * 4, y,
                            Graphics.TOP | Graphics.RIGHT);
                    si0 = (ev.degree >> 8) & 0xff;
                    if (si0 < 8) {
                        drawImg(osg, Summary.imgAspect, si0, x, y, Graphics.TOP | Graphics.HCENTER);
                    }
                }
                break;
            case Event.EV_TATTVAS:
                int rowh = Font.getDefaultFont().getHeight();
                int colw = Font.getDefaultFont().stringWidth("00:00 0  ");
                int w = width - 4;
                int h = height - 4;
                int col_count = w / colw;
                int row_count = h / rowh;
                int total_rows = Astromaximum.TATTVAS_IN_DAY / col_count;
                if (Astromaximum.TATTVAS_IN_DAY % col_count != 0)
                    ++total_rows;
                int xstart = left + 2 + (w - colw * col_count) / 2;
                int ystart = top + 2 + (h - rowh * row_count) / 2;
                int rows_delta = total_rows - row_count;
                if ((tag < 0) || (rows_delta <= 0))
                    tag = 0;
                if ((rows_delta > 0) && (tag > rows_delta))
                    tag = (byte)rows_delta;
//                System.out.println(tag);
                int counter = tag * col_count;
                long date = events[0].date0 + (counter * Astromaximum.MSECINTATTVA);
                int tattva = counter % 5;
                for (int row = 0; row < row_count; ++row) {
                    for (int col = 0; col < col_count ; ++col) {
                        if (counter >= Astromaximum.TATTVAS_IN_DAY)
                            break;
                        x = xstart + col * colw;
                        y = ystart + row * rowh;
                        osg.setColor(0);
                        if (nowSelection == counter) {
                            osg.setColor(Astromaximum.RED_COLOR);
                        }
                        if (isCus && cusSelection == counter) {
                            osg.setColor(Astromaximum.CUST_COLOR);
                        }
                        osg.drawString(Event.long2String(date, 2, false), x, y,
                                Graphics.TOP | Graphics.LEFT);
                        osg.setColor(Astromaximum.RUBY_COLOR);
                        osg.drawString(Integer.toString(tattva + 1), x + colw * 2 / 3, y,
                                Graphics.TOP | Graphics.LEFT);
                        date += Astromaximum.MSECINTATTVA;
                        if (tattva == 4)
                            tattva = 0;
                        else
                            ++tattva;
                        ++counter;
                    }
                }
                if ((rows_delta > 0) && (owner.getSelectedItem() == this)) {
                    int scrollbarh = h * row_count / total_rows;
                    int scrollbary = top + 2 + h * tag / total_rows;
                    osg.setColor(Astromaximum.SELECTION_COLOR);
                    osg.fillRect(w, scrollbary, 3, scrollbarh);
                }
                break;
        }
    }

    private void drawSignString(Graphics osg, Event ev, long now, boolean isCus, int x) {
        if (ev.date0 >= Summary.period0) {
            if (Summary.isCurrentDay && contains(ev, now)) {
                osg.setColor(Astromaximum.RUBY_COLOR);
            }
            if (isCus && contains(ev, owner.cusTime)) {
                osg.setColor(Astromaximum.CUST_COLOR);
            }
            osg.drawString(ev.getDateString(0, 1),
                    x + 1, top + height / 2 + 1,
                    Graphics.TOP | Graphics.LEFT);
        }
    }

    void setEvents(Vector _events) {
        events = new Event[_events.size()];
        for (int i = 0; i < events.length; i++) {
            events[i] = (Event) _events.elementAt(i);
        }
    }

    void setEvents(Event[] _events) {
        events = new Event[_events.length];
        System.arraycopy(_events, 0, events, 0, events.length);
    }

    void setEvents(int index, Event evt) {
        events[index] = evt;
    }

    /**
     * @param x
     * @param y
     * @return
     * @noinspection AssignmentToMethodParameter
     */
    boolean checkSelection(int x, int y) {
        final boolean chk = x >= left && x <= left + width && y >= top && y <= top + height;
        if (chk) {
            if (rowCount == 1) {
                x -= left;
                int ww = 0;
                for (int i = 0; i < widths.length; i++) {
                    ww += widths[i];
                    if (x < width * ww / 1000) {
                        selIndex = i;
                        break;
                    }
                }
            } else {
                selIndex = (y - top) * rowCount / height;
            }
        }
        return chk;
    }

    private int getX(int index, int mode) {
        int x = 0;
        for (int i = 0; i < index; i++) {
            x += widths[i];
        }
        if (mode == XCENTER) {
            x += widths[index] / 2;
        }
        if (mode == XRIGHT) {
            x += widths[index];
        }
        return left + width * x / 1000;
    }

    private Event getSelEvent() {
        try {
            return events[selIndex];
        } catch (IndexOutOfBoundsException e) {
            return null;
        }
    }

    /**
     * drawRiseSetCell
     *
     * @param osg Graphics
     * @param now
     */
    private void drawRiseSetCell(Graphics osg, long now) {
        int y1 = top + 1;
        int y2 = top + height / 2 - 1;
        int cus0 = 0;
        int now0 = 0;
        String s1 = Astromaximum.getstr(84) + " ";//rise
        String s2 = Astromaximum.getstr(85) + " ";//set
        long d1 = events[0].date0;
        long d2 = events[0].date1;

        if (d1 > d2) { // set before rise
            final String tmp = s1;
            final long ddd = d1;
            s1 = s2;
            d1 = d2;
            s2 = tmp;
            d2 = ddd;
        }
        if (d1 == 0) {
            y2 = top + height * 2 / 5;
        }
        if (d2 == 0) {
            y1 = top + height * 2 / 5;
        }
        if (Summary.isCurrentDay) {
            if (now >= d2) {
                now0 = 2;
            } else if (now > d1) {
                now0 = 1;
            }
        }
        if (owner.cusTime >= d2) {
            cus0 = 2;
        } else if (owner.cusTime > d1) {
            cus0 = 1;
        }
        if (d1 != 0) {
            osg.setColor(0);
            if (now0 == 1) {
                osg.setColor(Astromaximum.RUBY_COLOR);
            }
            if (Summary.isShowCustom && cus0 == 1) {
                osg.setColor(Astromaximum.CUST_COLOR);
            }
            osg.drawString(s1 + Event.long2String(d1, 1, false),
                    getX(0, XRIGHT), y1, Graphics.TOP | Graphics.RIGHT);
        }
        if (d2 != 0) {
            osg.setColor(0);
            if (now0 == 2) {
                osg.setColor(Astromaximum.RUBY_COLOR);
            }
            if (Summary.isShowCustom && cus0 == 2) {
                osg.setColor(Astromaximum.CUST_COLOR);
            }
            osg.drawString(s2 + Event.long2String(d2, 1, false),
                    getX(0, XRIGHT), y2, Graphics.TOP | Graphics.RIGHT);
        }
        osg.setColor(0);
    }

    boolean isOnPage() {
        return (page & 1 << owner.pageNum) != 0;
    }

    private void moveSelection(int delta) {
        if (events.length == 0) {
            return;
        }
        do {
            selIndex += delta;
            if (selIndex > widths.length - 1) {
                selIndex = 0;
            }
            if (selIndex < 0) {
                selIndex = widths.length - 1;
            }
        } while (events[selIndex] == null);
        prepareTithi();
    }

    long[] getParams(int idx) {
        int tp = type;
        if (tp == Event.EV_TATTVAS) {
            return new long[]{Event.EV_TATTVAS, -1, idx, 0, 0};
        }
        if (tp == Event.EV_HELP) {
            return new long[]{Event.EV_HELP0 + tag / 6, -1, tag * 10 + selIndex, 0, 0};
        }
        if (tp == Event.EV_TOPIC_BUTTON) {
            int tag1 = tag;
/* @todo: employment button opens travel help, need simpler and consistent sequence */
            if (tag1 == 3)
                tag1 = 5;
            if (tag1 > 2)
                --tag1;
            return new long[]{Event.EV_HELP1, -1, 90 + tag1, 0, 0};
        }
        if (tp == Event.EV_DECUMB_BEGIN) {
//      Astromaximum.interpreter.topic=Interpreter.T_DECUMB;
            switch (idx) {
                case 0:
                    tp = Event.EV_MOON_DAY;
                    break;
                case 1:
                    tp = Event.EV_DAY_HOURS;
                    break;
                default:
                    tp = Event.EV_RISE;
                    break;
            }
        }
        return getParams(events[idx], tp);
    }

    long[] getParams(Event evi, int t) {
        if (evi == null) {
            return null;
        }
        int plt = evi.planet0;
        final int dgr = evi.getDegree();
        final long d0 = evi.date0;
        final long d1 = evi.date1;
        switch (t) {
            case Event.EV_SUN_DAY:
                return new long[]{Event.EV_NAVROZ, plt, dgr, d0, 0};
            case Event.EV_MOON_DAY:
                return new long[]{t, plt, dgr, d0, d1};
            case Event.EV_RISE:
                return new long[]{t, -1, plt, dgr, d0, 0};
            case Event.EV_TITHI:
                final long d2 = (d0 + d1) / 2;
                return new long[]{t, plt, evi.getDegree(), d2, d0, d1};
            case Event.EV_ECLIPSE:
                if ((tag & 4) != 0) {
                    return new long[]{t, -1, plt, d0, 0};
                }
                return new long[]{Event.EV_MOON_PHASE, 1, evi.planet1 + 4, d0, 0};
            case Event.EV_MOON_PHASE:
                return new long[]{Event.EV_MOON_PHASE, 1, evi.planet1, d0, d1};
            case Event.EV_VIA_COMBUSTA:
            case Event.EV_VOC:
                return new long[]{t, plt, d0, d1};
            case Event.EV_RETROGRADE:
                return new long[]{t, -1, plt, d0, d1};
            case Event.EV_DAY_HOURS:
            case Event.EV_NIGHT_HOURS:
                return new long[]{Event.EV_PLANET_HOUR, -1, plt, d0, d1};
            case Event.EV_MOON_SIGN_LARGE:
            case Event.EV_DEG_2ND:
                if (plt == Event.SE_MOON) {
                    return new long[]{Event.EV_SIGN_ENTER, plt, evi.getDegree(), d0, d1};
                }
            case Event.EV_SUN_DEGREE_LARGE:
            case Event.EV_SEL_DEGREES:
                return new long[]{Event.EV_DEGPASS0 + dgr / 90, plt, dgr, Astromaximum.getSignDegree(dgr),
                            dgr / 30, evi.getDegType(), d0, d1
                        };
            case Event.EV_TOP_DAY:
                return new long[]{t, -1, plt, 0, 0};
            case Event.EV_DECUMBITURE:
                return new long[]{t, -1, plt, d0, 0};
            case Event.EV_ASP_EXACT:
                return new long[]{Event.EV_ASP_EXACT, -1, plt, evi.planet1, getBadGoodAspect(dgr), dgr, d0, 0};
            case Event.EV_DECUMB_ASPECT:
                return new long[]{Event.EV_ASP_EXACT_MOON, plt, evi.planet1, getBadGoodAspect(dgr), dgr, d0, 0};
            case Event.EV_MOON_MOVE:
                if (dgr == 200) {
                    int id1 = -1;
                    int id2 = -1;
                    int counter = 0;
                    for (Enumeration e = moonMoveVec.elements(); e.hasMoreElements();) {
                        final Event ev = (Event) e.nextElement();
                        if (ev.planet1 <= Event.SE_SATURN) {
                            final long dat = ev.date0;
                            if (dat <= d0) {
                                id1 = counter;
                            }
                            else if (id2 == -1 && dat >= d1) {
                                id2 = counter;
                            }
                        }
                        ++counter;
                    }
                    final Event e0 = Astromaximum.evAt(moonMoveVec, id1);
                    final Event e1 = Astromaximum.evAt(moonMoveVec, id2);
                    plt = e0.planet1;
                    final int plt2 = e1.planet1;
                    if (plt2 == Event.SE_MOON) {
                        plt = 255;
                    }
                    return new long[]{t, 1, plt, plt2, e0.planet1, e0.date0, e1.date0};
                } else {
                    final int plt2 = evi.planet1;
                    if (plt2 == Event.SE_MOON) {
                        return new long[]{Event.EV_SIGN_ENTER, plt, dgr, d0, d1};
                    } else {
//            Astromaximum.evDump(events);
                        //           System.out.println(dgr);
                        return new long[]{Event.EV_ASP_EXACT_MOON, plt, plt2, getBadGoodAspect(dgr), dgr, d0, 0};
                    }
                }
        }
        return null;
    }

    private String getStatus() {
        if (type == Event.EV_TOPIC_BUTTON) {
            return Astromaximum.getstr(50 + tag); // fb
        }
        if (type == Event.EV_PANEL) {
            return Astromaximum.getstr(102); //topics
        }
        if (type == Event.EV_BACK) {
            return Astromaximum.getstr(94); // back
        }
        if (type == Event.EV_TATTVAS) {
            return Astromaximum.getstr(180); // hotkey help
        }
        if (type == Event.EV_TOP_DAY) {
//#ifdef freetest
//# 	        if (Options.isRealtimeOff)
//# 				return Astromaximum.options.getCurrentCity(false);
//# 			else
//#endif
				return Astromaximum.getstr(27) + " " + Astromaximum.getstr( //Day, of_
						40 + weekPlanets[events[1].planet0 - 1]);
        }
        if (type == Event.EV_SUN_RISE || type == Event.EV_MOON_RISE) {
            return Astromaximum.getstr(145); // planets in axes
        }
        String s = "";
        final Event sel = getSelEvent();
        int hrOnly = 1;
        if (sel != null) {
            String tire = (Summary.IMG_HEIGHT == 12) ? " - " : "-";
            switch (type) {
                case Event.EV_MOON_MOVE:
                    final int sind = selIndex;
                    if ((sel.date0 == sel.date1) || (sel.planet0 == sel.planet1)) {
                        s = sel.getDateString(0, (sind != 0 && sind != events.length - 1) ? 1 : 0);
                    } else {
                        s = sel.getDateString(0, hrOnly) + tire + sel.getDateString(1, hrOnly);
                    }
                    break;
                case Event.EV_ECLIPSE:
                    s = sel.getDateString(0, 0);
                    break;
                default:
                    hrOnly = 0;
                    if (sel.date0 == sel.date1) {
                        s = sel.getDateString(0, hrOnly);
                    } else {
                        s = sel.getDateString(0, hrOnly) + tire +
                                sel.getDateString(1, hrOnly);
                    }
            }
/*
            Astromaximum.calendar.setTime(new Date(sel.date0));
            System.out.print("Status GMT= ");
            System.out.print(Astromaximum.calendar.get(Calendar.DAY_OF_MONTH));
            System.out.print(" # ");
            System.out.print(Astromaximum.calendar.get(Calendar.HOUR_OF_DAY));
            System.out.print(":");
            System.out.println(Event.to2String(Astromaximum.calendar.get(Calendar.MINUTE)));
*/
        }
        return s;
    }

    void recalcSelection(long time, boolean isCustom) {
        if (events == null) {
            return;
        }
        if (isCustom) {
            cusSelection = -1;
        } else {
            nowSelection = -1;
        }
//    if(!isCustom /*&& !Summary.isCurrentDay*/) {
//      return;
//    }
        if (type == Event.EV_TATTVAS) {
            long date = events[0].date0;
            for (int i = 0; i < Astromaximum.TATTVAS_IN_DAY; ++i) {
                if ((time >= date) && (time < date + Astromaximum.MSECINTATTVA)) {
                    if (isCustom) {
                        cusSelection = i;
                    } else {
                        nowSelection = i;
                    }
                    break;
                }
                date += Astromaximum.MSECINTATTVA;
            }
            return;
        }

        for (int i = 0; i < events.length; i++) {
            boolean flg = false;
            Event ev = events[i];
            if (ev != null) {
                if (type == Event.EV_RISE) {
                    long delta = time - ev.date0;
                    flg = (delta > DEGREE_DELTA_MSEC1) && (delta < DEGREE_DELTA_MSEC2);
                }
                else {
                    if (!(type == Event.EV_MOON_MOVE && ev.degree != 200)) {
                        flg = contains(events[i], time);
                    }
                }
                if (!flg) {
                    continue;
                }
                if (isCustom) {
                    cusSelection = i;
                } else {
                    nowSelection = i;
                }
                break;
            }
        }
    }

//#mdebug info
    void dump() {
        System.out.println("**SummItem dump**");
        System.out.print("Type: ");
        System.out.print(type);
        System.out.print(" cusSel=");
        System.out.print(cusSelection);
        System.out.print(" nowSel=");
        System.out.println(nowSelection);
        System.out.println("Events=");
        if (events != null) {
            Astromaximum.evDump(events);
        }
    }
//#enddebug

    void prepareTithi() {
        if (type != Event.EV_TITHI) {
            return;
        }
//    events[0].dump();
        int deg2 = getSelEvent().getDegree() - 1;
//#if logger
      Astromaximum.instance.logger(Integer.toString(deg2));
//#endif
        if (deg2 > 30) {
            deg2 = 0;
            Astromaximum.log("wrong tithi");
        }
        tithi = Astromaximum.extractImg(deg2, "/res/ph" + Integer.toString(Summary.moonPhaseH) + ".dat");
        Astromaximum.summary.calcPhase(getSelEvent().date1);
    }

    void setSelection() {
        if (Summary.isCurrentDay) {
            selIndex = nowSelection;
        } else {
//          Astromaximum.log("not current!");
            selIndex = 0;
        }
    }

    private void zeroPlaces() {
        for (int i = 0; i < places.length; i++) {
            places[i] = 0;
        }
    }

//  int getEventCount() {
//    return events.length;
//  }
    int defaultNavigate(int dir) {
        int delta = nav[dir];
        int msel = (dir % 2 == 0) ? -1 : 1;
        int where = 0;
        if (delta == 0) {
            moveSelection(msel);
            return 0;
        }
        if (delta > 30 && delta < 50) {
            delta -= 40;
            if (!((selIndex == 0 && msel < 0) || (selIndex == events.length - 1 && msel > 0))) {
                if (!isEmpty()) {
                    moveSelection(msel);
                    return 0;
                }
            } else if (type == Event.EV_DAY_HOURS || type == Event.EV_NIGHT_HOURS) {
                selIndex = events.length - 1 - selIndex;
                return 0;
            }

        }
        if (delta > 50 && delta < 70) { // to first
            delta -= 60;
            where = -1;
        }
        if (delta > 70 && delta < 90) { // to last
            delta -= 80;
            where = 1;
        }
        if (delta >= 91 && delta <= 93) { // EV_TATTVA horizontal scrolling
            delta -= 92;
            tag += delta;
            return 0;
        }
        if (delta > 20) {
            return delta;
        }
        owner.moveFocus(delta);
        SummItem si = owner.getSelectedItem();
        if (si.isEmpty()) {
            return -1;
        }
        if (where < 0) {
            si.selIndex = 0;
        }
        if (where > 0) {
            si.selIndex = si.events.length - 1;
        }
        return 0;
    }

    static boolean contains(Event ev, long date) {
//    date/=60000;
//    return ev != null && date >= (ev.date0/60000) && date < (ev.date1/60000);
        return ev != null && date >= ev.date0 && date < ev.date1;
    }

    private static void drawSelDegree(Graphics osg, Event event, int x, int y, int anchor) {
        int plt = event.planet0;
        if (event.getDegType() == 1) {
            plt += 13;//Astromaximum.PLANET_COUNT;
        }
        drawImg(osg, Summary.imgOpaq, plt, x, y, anchor);
    }

    private static void drawAspect(Graphics osg, Event event, int x, int y, int vanchor) {
        drawImg(osg, Summary.imgPlanet, event.planet0, x - Summary.IMG_WIDTH / 2, y,
                vanchor | Graphics.RIGHT);
        drawImg(osg, Summary.imgAspect, getAspIndex(event.getDegree()), x, y,
                vanchor | Graphics.HCENTER);
        drawImg(osg, Summary.imgPlanet, event.planet1, x + Summary.IMG_WIDTH / 2, y,
                vanchor | Graphics.LEFT);
    }

    private static void drawIngress(Graphics osg, Event event, int x, int y, int anchor) {
        drawImg(osg, Summary.imgPlanet, event.planet0, x, y, anchor);
        drawImg(osg, Summary.imgZodiac, event.getDegree(), x + Summary.IMG_WIDTH, y, anchor);
    }

    /**
     * getAspIndex
     *
     * @param angle int
     * @return int
     */
    private static int getAspIndex(int angle) {
        int idx = -1;
        for (int i = 0; i < ASP_ANGLES.length; i++) {
            if (ASP_ANGLES[i] == angle) {
                idx = i;
                break;
            }
        }
        return idx;
    }

    static void drawImg(Graphics osg, Image image, int index, int left, int top, int anchor) {
        final int h = image.getHeight();
//#if "2.0"!="2.0"
//#     osg.drawRegion(image,h*index,0,h,h, Sprite.TRANS_NONE,left,top,anchor);
//#else
        int ch = osg.getClipHeight(), cw = osg.getClipWidth();
        if ((anchor & Graphics.HCENTER) != 0) {
            left -= h / 2;
        }
        if ((anchor & Graphics.RIGHT) != 0) {
            left -= h;
        }
        if ((anchor & Graphics.VCENTER) != 0) {
            top -= h / 2;
        }
        if ((anchor & Graphics.BOTTOM) != 0) {
            top -= h;
        }
        osg.setClip(left, top, h, h);
        osg.drawImage(image, left - h * index, top, Graphics.LEFT | Graphics.TOP);
        osg.setClip(0, 0, cw, ch);
//#endif
    }

    boolean isEmpty() {
        switch (type) {
            case Event.EV_STATUS:
                return owner.pageNum == Summary.PAGE_PANEL;
            case Event.EV_ASP_EXACT:
            case Event.EV_PANEL:
            case Event.EV_BACK:
            case Event.EV_TOPIC_BUTTON:
            case Event.EV_WEEK_GRID:
            case Event.EV_MONTH_GRID:
                return false;
            default:
                return (events.length == 0) || (events.length == 1 && events[0] == null);
        }
    }

    Event getCusSelEvent() {
        try {
            return events[cusSelection];
        } catch (Exception e) {
            return null;
        }
    }

    public void run() {
        if (type == 1) {
            owner.recalcAllSelections();
//#if "imeiCheck" @ protection
            long tick = Options.currentTime();
            DataFile.hj = Options.hj * (int) ((tick << 10) & 0x6fedc6);
//#endif
//      GeoList.localOffset=TimeZone.getDefault().getRawOffset();
//      System.out.println(GeoList.localOffset);
            owner.repaint();
        } else {
//      System.out.println("SummItem timer");
            owner.drawFrame();
        }
    }

    private int getBadGoodAspect(int dgr) {
        dgr = getAspIndex(dgr);
        if (dgr >= 3) {
            dgr = 2;
        } else if (dgr != 0) {
            dgr = 1;
        }
        return dgr;
    }

    boolean haveTopic(int tt) {
        String ts = (String) topics.get(new Integer(tt));
        return (ts != null && ts.indexOf(Interpreter.RESERVED_CHARS.charAt(Interpreter.topic)) >= 0);
    }

    public boolean matches(byte[] b) {
        String s = Astromaximum.options.extractCityName(b);
        return b != null && s != null;
    }

    private void drawWeek(Graphics osg, boolean isSelected, long now) {
        Event ev;
        int y;
        osg.setColor(Astromaximum.BACK_COLOR);
        osg.fillRect(left, top, width, height);
        osg.setColor(0);
// @todo zero
        zeroPlaces();
        Date cur = new Date(Summary.firstGridDate.getTime());
        final int colWidth = width / owner.colCount;
        final int rowHeight = height / owner.rowCount;
        final int leftm = width - colWidth * owner.colCount;
        int cnt = 0;
        boolean[] nodrawNums = new boolean[owner.colCount * owner.rowCount];
        // dimmed days aside selected month
// @todo **********
        int count = 0;
        int count2 = 1;
        long fgd;
        final long fgd2;
        fgd = Summary.firstGridDate.getTime();
        fgd -= Event.localOffset(fgd);
        fgd2 = fgd + owner.rowCount * Astromaximum.MSECINDAY;
        for (int row = 0; row < owner.rowCount; row++) {
            for (int col = 0; col < owner.colCount; col++) {
//        int fontColor=0;
                int fillColor = Astromaximum.DIMMED_COLOR;
                Astromaximum.calendar.setTime(cur);
                if (owner.selMonth == Astromaximum.calendar.get(Calendar.MONTH)) {
                    fillColor = Astromaximum.CURRENT_MONTH_COLOR;
                }
                long ld = cur.getTime();
                ld -= Event.localOffset(ld);
                final long ld2 = ld + Astromaximum.MSECINDAY;
                Event eclipse = Astromaximum.dataFile.todayEclipse(ld, 3);
                boolean isCurDay = isSelected && col == owner.getSelX() && row == owner.getSelY();
                if (isCurDay) {
                    // current day highlight
                    if (fillColor != 0) {
                        if (type == Event.EV_MONTH_GRID) {
                            fillColor = Astromaximum.GRAY_COLOR;
//            fontColor=0;
                        }
                    } else {
                        fillColor = Astromaximum.BACK_COLOR;
                    }
                }
                int xx = leftm + col * colWidth;
                int yy = row * rowHeight + top + 2;
                osg.setColor(fillColor);
                osg.fillRect(xx + 1, yy, colWidth - 1, rowHeight - 1);
                if (isCurDay) {
                    osg.setColor(Astromaximum.SELECTION_COLOR);
                    osg.drawRect(xx + 1, yy, colWidth - 1, rowHeight - 1);
                }
                /* @todo Eclipse drawing */
                if (eclipse != null) {
                    drawImg(osg, Summary.imgAspect,
                            10 + eclipse.planet0,
                            (Summary.IMG_WIDTH * 5) + xx,
                            (3) + yy - 1,
                            Graphics.TOP | Graphics.RIGHT);
                    ++places[row];
//              nodrawNums[cnt]=true;
                }
                zeroPlaces();
                yy += rowHeight - 2;
//            Astromaximum.evDump(owner.mSelDeg);
                /* @todo Selected degrees drawing in week mode */
                for (Enumeration e = owner.mSelDeg.elements(); e.hasMoreElements();) {
                    ev = (Event) e.nextElement();
                    if (ev.isInPeriod(ld, ld2, false)) {
                        final int pos = ++places[row];
                        drawSelDegree(osg, ev, xx + colWidth * 2 / 5 + pos * Summary.IMG_HEIGHT, yy,
                                Graphics.BOTTOM | Graphics.HCENTER);
                    }
                }
                /* @todo Moon phase drawing in week mode */
                for (int i = 0; i < Summary.moonPhaseCount; i++) {
                    eclipse = Summary.aMoonPhase[i];
                    if (eclipse.isDateBetween(0, ld, ld2)) {
                        owner.drawPhase(osg, leftm + Summary.IMG_WIDTH * 5 / 2,
                                yy - Summary.IMG_HEIGHT, Summary.IMG_HEIGHT, eclipse.planet1);
//                  drawImg(osg,Summary.imgPhase,eclipse.planet1,xx+owner.IMG_WIDTH*2,yy,
//                      Graphics.BOTTOM | Graphics.LEFT);
                        nodrawNums[cnt] = true;
                        break;
                    }
                }
                yy -= rowHeight - 2;
                xx += 2;
                final long start = cur.getTime();
                cur.setTime(start + Astromaximum.MSECINDAY);
                if (now >= start && now < cur.getTime()) {
                    osg.setColor(Astromaximum.RED_COLOR);
                    osg.drawRect(col * colWidth + leftm, row * rowHeight + top + 1,
                            colWidth, rowHeight - 1);
                }
                ++cnt;
            }
        }
// @todo **********
        osg.setColor(0);
//    osg.setFont(oldFont);
// @todo zero
        zeroPlaces();
        for (Enumeration e = owner.mIngress.elements(); e.hasMoreElements();) {
            ev = (Event) e.nextElement();
            final long date = ev.date0;//-Event.localOffset(ev.date0);

            if (date <= Astromaximum.dataFile.startJD || date >= Astromaximum.dataFile.finalJD) {
                continue;
            }
			
            if (ev.isDateBetween(0, fgd, fgd2)) {
                final int day = (int) ((date - fgd) / Astromaximum.MSECINDAY);
                int x = day % owner.colCount * colWidth + 1;
                y = day / owner.colCount * rowHeight + top + 1;
                if (ev.planet0 == Event.SE_MOON) {
                    x = 0;
                } else {
                    places[day] += 5;
                    x += (places[day] - 5) * Summary.IMG_WIDTH / 2 + Summary.IMG_WIDTH * 7 / 2;
                }
                y += rowHeight - Summary.IMG_HEIGHT - 2;
                // @todo Moon ingress drawing
                drawIngress(osg, ev, x + leftm, y, Graphics.TOP | Graphics.LEFT);
            }
        }
        for (Enumeration e = owner.mRetro.elements(); e.hasMoreElements();) {
            ev = (Event) e.nextElement();
            int x;
//          if(ev.date0<=Astromaximum.dataFile.startJD || ev.date1>=Astromaximum.dataFile.finalJD)
//            continue;
            if (ev.getDegree() == 0) {
                for (int i = 0; i < 2; i++) {
                    final long date = (i > 0) ? ev.date1 : ev.date0;
                    if (ev.isDateBetween(i, fgd, fgd2)) {
                        final int day = (int) ((date - fgd) / Astromaximum.MSECINDAY);
                        places[day]++;
                        final int pos = places[day]++;
                        x = day % owner.colCount * colWidth + 1 + leftm;
                        y = day / owner.colCount * rowHeight + top + 2;
                        y += rowHeight - Summary.IMG_HEIGHT - 3;
                        x += pos * Summary.IMG_WIDTH / 2 + Summary.IMG_WIDTH * 7 / 2;
                        // @todo Retrograde drawing
                        drawImg(osg, Summary.imgPlanet, ev.planet0, x, y,
                                Graphics.TOP | Graphics.LEFT);
                        drawImg(osg, Summary.imgService, i * 2 + 1, x + 3, y + 3,
                                Graphics.TOP | Graphics.LEFT);
                        places[day]++;
                    }
                }
            }
            y = top - 2;
            x = count * (Summary.IMG_WIDTH + 3);
            if (ev.getDegree() == 0) {
                x = width - 2 - count2 * (Summary.IMG_WIDTH + 3);
                ++count2;
            } else {
                ++count;
            }
            // @todo Retrograde drawing
//          if(!weekMode){
            drawImg(osg, Summary.imgPlanet, ev.planet0, x, y,
                    Graphics.BOTTOM | Graphics.LEFT);
            drawImg(osg, Summary.imgService, 1, x + 3, y + 3, Graphics.BOTTOM | Graphics.LEFT);
//          }
        }
// @todo zero in week
        zeroPlaces();
        // @todo Aspect drawing in week mode
        int acnt = Summary.aAspects.length;
        for (int i = 0; i < acnt; i++) {
            ev = Summary.aAspects[i];
            long date = ev.date0;
            if (ev.isDateBetween(0, fgd, fgd2)) {
                final int day = (int) ((date - fgd) / Astromaximum.MSECINDAY);
                places[day] += (Summary.size == 2 ? 3 : 3);
                final int x = colWidth + (2 - places[day]) * (Summary.IMG_HEIGHT + 1) - 1;
                y = day / owner.colCount * rowHeight + top + 3;
                drawAspect(osg, ev, x, y, Graphics.TOP);
                if (places[day] > 10) {
                    nodrawNums[day] = true;
                }
//              osg.drawChar('.',x-Summary.IMG_HEIGHT*3/2-2,y+Summary.IMG_HEIGHT,Graphics.BASELINE|Graphics.RIGHT);
            }
        }
        /* @todo Day # drawing in week mode */
        cur = new Date(Summary.firstGridDate.getTime());
        cnt = 0;
        for (int row = 0; row < owner.rowCount; row++) {
            for (int col = 0; col < owner.colCount; col++) {
                int xx = leftm;
                xx += Summary.IMG_WIDTH * 2;
                int yy = (row + 1) * rowHeight + top + 1;
                Astromaximum.calendar.setTime(cur);
                yy -= rowHeight / 2;
                if (Astromaximum.calendar.get(Calendar.DAY_OF_WEEK) == Calendar.SUNDAY) {
                    osg.setColor(Astromaximum.RUBY_COLOR);
                } else {
                    osg.setColor(0);
                }
                osg.drawString(Astromaximum.getstr(20 + Astromaximum.calendar.get(Calendar.DAY_OF_WEEK) - 1),
                        xx + Summary.IMG_WIDTH, yy - 1, Graphics.BASELINE | Graphics.HCENTER);
                if (!nodrawNums[cnt++]) {
                    osg.drawString(Integer.toString(Astromaximum.calendar.get(Calendar.DAY_OF_MONTH)),
                            xx + (Summary.IMG_WIDTH), yy,
                            (Graphics.TOP) | Graphics.HCENTER);
                }
                final long start = cur.getTime();
                cur.setTime(start + Astromaximum.MSECINDAY);
            }
        }
    }

    private void drawMonth(Graphics osg, boolean isSelected, long now) {
        try{
        Event ev;
        int y;
        osg.setColor(Astromaximum.BACK_COLOR);
        osg.fillRect(left, top, width, height);
        osg.setColor(0);
// @todo zero
        zeroPlaces();
        Date cur = new Date(Summary.firstGridDate.getTime());
//        final Font oldFont = osg.getFont();
        final int colWidth = width / owner.colCount;
        final int rowHeight = height / owner.rowCount;
        final int leftm = width - colWidth * owner.colCount;
        int cnt = 0;
        boolean[] nodrawNums = new boolean[owner.colCount * owner.rowCount];
        // dimmed days aside selected month
// @todo **********
        int count = 0;
        int count2 = 1;
        long fgd;
        final long fgd2;
        // weekday numbering
        fgd = Summary.period0;
        fgd2 = Summary.period1;
        for (int row = 0; row < owner.rowCount; row++) {
            for (int col = 0; col < owner.colCount; col++) {
                int fillColor = Astromaximum.DIMMED_COLOR;
                Astromaximum.calendar.setTime(cur);
                if (owner.selMonth == Astromaximum.calendar.get(Calendar.MONTH)) {
                    fillColor = Astromaximum.CURRENT_MONTH_COLOR;
                }
                long ld = cur.getTime();
                ld -= Event.localOffset(ld);
                Event eclipse = Astromaximum.dataFile.todayEclipse(ld, 0);
                if (eclipse != null) {
                    fillColor = 0;
                    nodrawNums[cnt] = true;
                }
                boolean isCurDay = isSelected && col == owner.getSelX() && row == owner.getSelY();
                if (isCurDay) {
                    // current day highlight
                    if (fillColor != 0) {
                        if (type == Event.EV_MONTH_GRID) {
                            fillColor = Astromaximum.GRAY_COLOR;
                        }
                    } else {
                        fillColor = Astromaximum.BACK_COLOR;
                    }
                }
                int xx = leftm + col * colWidth;
                int yy = row * rowHeight + top + 2;
                osg.setColor(fillColor);
                osg.fillRect(xx + 1, yy, colWidth - 1, rowHeight - 1);
                /* @todo Eclipse drawing */
                if (eclipse != null) {
                    drawImg(osg, Summary.imgAspect,
                            10 + eclipse.planet0,
                            (colWidth) + xx,
                            (rowHeight - Summary.IMG_HEIGHT) + yy - 1,
                            Graphics.TOP | Graphics.RIGHT);
//              nodrawNums[cnt]=true;
                }

                yy += rowHeight - 2;
//            Astromaximum.evDump(owner.mSelDeg);
                final long start = cur.getTime();
                cur.setTime(start + Astromaximum.MSECINDAY);
                if (now >= start && now < cur.getTime()) {
                    osg.setColor(Astromaximum.RED_COLOR);
                    osg.drawRect(col * colWidth + leftm, row * rowHeight + top + 1,
                            colWidth, rowHeight - 1);
                }
                ++cnt;
            }
        }
// @todo **********
        osg.setColor(0);
//        osg.setFont(oldFont);
// @todo zero

        for (Enumeration e = owner.mIngress.elements(); e.hasMoreElements();) {
            ev = (Event) e.nextElement();
            if (ev.date0 <= Astromaximum.dataFile.startJD) {
                continue;
            }
            if (ev.isDateBetween(0, fgd, fgd2)) {
                final int day = (int) ((ev.date0 - fgd) / Astromaximum.MSECINDAY);
                int x = day % owner.colCount * colWidth + 1;
                y = day / owner.colCount * rowHeight + top + 1;
                places[day]++;
                y += (places[day] - 1) * Summary.IMG_HEIGHT;
                // @todo Ingress drawing
                drawIngress(osg, ev, x + leftm, y, Graphics.TOP | Graphics.LEFT);
                if (places[day] > 1) {
                    nodrawNums[day] = true;
                }
            }
        }
        for (Enumeration e = owner.mRetro.elements(); e.hasMoreElements();) {
            ev = (Event) e.nextElement();
            int x;
            if (ev.getDegree() == 0) {
                for (int i = 0; i < 2; i++) {
                    final long date = (i > 0) ? ev.date1 : ev.date0;
                    if (ev.isDateBetween(i, fgd, fgd2)) {
                        final int day = (int) ((date - fgd) / Astromaximum.MSECINDAY);
                        final int pos = places[day]++;
                        x = day % owner.colCount * colWidth + 1 + leftm;
                        y = day / owner.colCount * rowHeight + top + 2;
                        y += pos * Summary.IMG_HEIGHT;
                        // @todo Retrograde drawing
                        drawImg(osg, Summary.imgPlanet, ev.planet0, x, y,
                                Graphics.TOP | Graphics.LEFT);
                        drawImg(osg, Summary.imgService, i * 2 + 1, x + 3, y + 3,
                                Graphics.TOP | Graphics.LEFT);
                        if (places[day] > 1) {
                            nodrawNums[day] = true;
                        }
                    }
                }
            }
            y = top - 2;
            x = count * (Summary.IMG_WIDTH + 3);
            if (ev.getDegree() == 0) {
                x = width - 2 - count2 * (Summary.IMG_WIDTH + 3);
                ++count2;
            } else {
                ++count;
            }
            // @todo Retrograde drawing
//          if(!weekMode){
            if (Summary.size > 1) {
                y -= osg.getFont().getHeight();
            }
            drawImg(osg, Summary.imgPlanet, ev.planet0, x, y,
                    Graphics.BOTTOM | Graphics.LEFT);
            drawImg(osg, Summary.imgService, 1, x + 3, y + 3, Graphics.BOTTOM | Graphics.LEFT);
//          }
        }
        /* @todo Day # drawing in week mode */
        cur = new Date(Summary.firstGridDate.getTime());
        cnt = 0;
        for (int row = 0; row < owner.rowCount; row++) {
            for (int col = 0; col < owner.colCount; col++) {
                int xx = leftm;
                xx += col * colWidth;
                int yy = (row + 1) * rowHeight + top - 1;
                Astromaximum.calendar.setTime(cur);
                if (Astromaximum.calendar.get(Calendar.DAY_OF_WEEK) == Calendar.SUNDAY) {
                    osg.setColor(Astromaximum.RUBY_COLOR);
                } else {
                    osg.setColor(0);
                }
                if (!nodrawNums[cnt++]) {
                    osg.drawString(Integer.toString(Astromaximum.calendar.get(Calendar.DAY_OF_MONTH)),
                            xx + (colWidth / 2), yy,
                            (Graphics.BASELINE) | Graphics.HCENTER);
                }
                final long start = cur.getTime();
                cur.setTime(start + Astromaximum.MSECINDAY);
            }
        }
        if (Summary.size > 1) {
            for (int i = 0; i < 7; i++) {
                osg.setColor(i == 0 ? 0xb00000 : 0);
                osg.drawString(Astromaximum.getstr(20 + i),
                        leftm + colWidth * i + colWidth / 2, top, Graphics.BASELINE | Graphics.HCENTER);
            }
        }
    }catch(Exception ex){
        Astromaximum.log(ex.getMessage() + ": " + Integer.toString(Astromaximum.errCode));
    }
    }

    void altAction (int key) {
        if (type == Event.EV_TATTVAS) {
            selIndex = key;
            if (Astromaximum.interpreter.findText(this, true)) {
                Display.getDisplay(Astromaximum.instance).setCurrent(Astromaximum.interpreter);
            }
            selIndex = 0;
        }
    }
}

// # vi:et:ts=4:sw=4
