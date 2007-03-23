/*
 * FrameAnimator.java
 *
 * Created on 15 January 2007, 19:37
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

import java.util.*;
import javax.microedition.lcdui.*;

/**
 *
 * @author Administrator
 */
class FrameAnimator extends Canvas implements Runnable{
  protected Timer timer;
  private final int DELAY=200;
  private int progress;
  protected boolean goon;
  private Image img;
  private final String moonFile;
  private int moonX; private int moonY;
  private final int frameCount;
  
  /** Creates a new instance of FrameAnimator
   * @param file
   * @param frames*/
  FrameAnimator(String file, int frames, int progr) {
    //    setFullScreenMode(true);
    moonFile = file;
    progress = progr;
    frameCount = frames;
    img = Astromaximum.extractImg(0, file);
  }
  
  void setMoonXY(int x, int y, int flags){
    moonX=x; moonY=y;
    if((flags & Graphics.HCENTER) > 0) {
      moonX -= img.getWidth()>>1;
    }
    if((flags & Graphics.VCENTER) > 0) {
      moonY -= img.getHeight()>>1;
    }
    if((flags & Graphics.RIGHT) > 0) {
      moonX -= img.getWidth();
    }
    if((flags & Graphics.BOTTOM) > 0) {
      moonY -= img.getHeight();
    }
  }
  
  public void run(){
    goon=true;
    timer=new Timer();
    timer.schedule(new SummItem(0),DELAY,DELAY);
  }
  
  public void stop(){
    goon=false;
    if(timer!=null){
      timer.cancel();
    }
    progress=0;
  }
  
  protected void paint(Graphics osg){
    if(goon){
      final int x=osg.getClipX();
      final int y=osg.getClipY();
      final int w=osg.getClipWidth();
      final int h=osg.getClipHeight();
      osg.setClip(moonX,moonY,img.getWidth(),img.getHeight());
      osg.drawImage(img,moonX,moonY,Graphics.LEFT|Graphics.TOP);
      osg.setClip(x,y,w,h);
    }
  }
  
  protected void drawFrame(){
    if(goon){
      if(progress<frameCount/2){
        img=Astromaximum.extractImg(progress,moonFile);
//#debug debug        
        System.out.println("drawFrame");
        repaint();
        serviceRepaints();
        ++progress;
      }
    }
  }
}
