import jmunit.framework.cldc10.*;

public class InterpreterTest extends TestCase {

  /**
   * Test of findText method, of class Interpreter.
   */
  public void testfindText() throws AssertionFailedException {
    System.out.println("findText");
    SummItem si = new SummItem(Event.EV_MOON_SIGN_LARGE);
    si.events=new Event[1];
    si.events[0]=new Event(System.currentTimeMillis(),Event.SE_MOON);
    si.events[0].date1=System.currentTimeMillis()+3600*1000;
    Interpreter instance = new Interpreter();
    for(int h=0; h<11; h++){
      System.out.println(h);
      instance.topic=h;
      for(int i=0; i<12; i++){
        si.events[0].degree=(short)i;
        boolean result = instance.findText(si);
      }
    }
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  /**
   * Test of sizeChanged method, of class Interpreter.
   */
  public void testsizeChanged() throws AssertionFailedException {
    System.out.println("sizeChanged");
    int w = 0;
    int h = 0;
    Interpreter instance = new Interpreter();
    instance.sizeChanged(w,h);
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  /**
   * Test of commandAction method, of class Interpreter.
   */
  public void testcommandAction() throws AssertionFailedException {
    System.out.println("commandAction");
    javax.microedition.lcdui.Command c = null;
    javax.microedition.lcdui.Displayable d = null;
    Interpreter instance = new Interpreter();
    instance.commandAction(c,d);
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  /**
   * Test of paint method, of class Interpreter.
   */
  public void testpaint() throws AssertionFailedException {
    System.out.println("paint");
    javax.microedition.lcdui.Graphics graphics = null;
    Interpreter instance = new Interpreter();
    instance.paint(graphics);
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  /**
   * Test of keyReleased method, of class Interpreter.
   */
  public void testkeyReleased() throws AssertionFailedException {
    System.out.println("keyReleased");
    int keyCode = 0;
    Interpreter instance = new Interpreter();
    instance.keyReleased(keyCode);
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  public InterpreterTest() {
    super(5,"InterpreterTest");
  }

  public void setUp() {
  }

  public void tearDown() {
  }

  public void test(int testNumber) throws Throwable {
    switch(testNumber) {
      case 0:testfindText();break;
//      case 1:testsizeChanged();break;
//      case 2:testcommandAction();break;
//      case 3:testpaint();break;
//      case 4:testkeyReleased();break;
      default: break;
    }
  }
}
