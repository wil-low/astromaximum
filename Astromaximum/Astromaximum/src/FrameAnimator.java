/*
 * FrameAnimator.java
 *
 * Created on 15 January 2007, 19:37
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

/**
 *
 * @author Administrator
 */
//#define imgPhase

//#ifdef build.desktop
//# package com.sw_axis;
//# //import java.util.*;
//# class FrameAnimator{
//#else
import javax.microedition.lcdui.*;
import java.util.*;
class FrameAnimator extends Canvas implements Runnable{
  protected Timer timer;
  private final int DELAY=200;
  private int progress;
  protected boolean goon;
//#ifdef imgPhase 
  private Image img;
  private final String moonFile;
//#else
//#   static final int width=0;
//#endif
  private int moonX; private int moonY;
  private final int frameCount;
  
  /** Creates a new instance of FrameAnimator
   * @param file
   * @param frames*/
  FrameAnimator(String file, int frames, int progr) {
    //    setFullScreenMode(true);
    progress = progr;
    frameCount = frames;
//#ifdef imgPhase 
    moonFile = file;
    img = Astromaximum.extractImg(0, file);
//#endif    
  }
  
  void setMoonXY(int x, int y, int flags){
    moonX=x; moonY=y;
//#ifdef imgPhase 
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
//#else
//#     if((flags & Graphics.HCENTER) > 0) {
//#       moonX -= width>>1;
//#     }
//#     if((flags & Graphics.VCENTER) > 0) {
//#       moonY -= width>>1;
//#     }
//#     if((flags & Graphics.RIGHT) > 0) {
//#       moonX -= width;
//#     }
//#     if((flags & Graphics.BOTTOM) > 0) {
//#       moonY -= width;
//#     }
//#endif    
  }
  
  public void run(){
    goon=true;
    timer=new Timer();
    timer.schedule(new SummItem(0),DELAY,DELAY);
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
  
  protected void paint(Graphics osg){
    if(goon){
      final int x=osg.getClipX();
      final int y=osg.getClipY();
      final int w=osg.getClipWidth();
      final int h=osg.getClipHeight();
//#ifdef imgPhase
      osg.setClip(moonX,moonY,img.getWidth(),img.getHeight());
      osg.drawImage(img,moonX,moonY,Graphics.LEFT|Graphics.TOP);
//#else
//#       osg.setClip(moonX,moonY,width,width);
//#       osg.fillArc(moonX,moonY,moonX,moonY,-90,180);
//#endif      
      osg.setClip(x,y,w,h);
    }
  }
  
  protected void drawFrame(){
    if(goon){
      if(progress<frameCount/2){
//#ifdef imgPhase
        img=Astromaximum.extractImg(progress,moonFile);
//#endif
//#debug debug        
        System.out.println("drawFrame");
        repaint();
        serviceRepaints();
        ++progress;
      }
    }
  }
//#endif  
}
