import java.util.Date;
import jmunit.framework.cldc10.*;

public class DataFileTest extends TestCase {

  /**
   * Test of fillCache method, of class DataFile.
   */
  public void testfillCache() throws AssertionFailedException {
    System.out.println("fillCache");
    DataFile instance = new DataFile();
    instance.fillCache();
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  /**
   * Test of getEventsOnPeriod method, of class DataFile.
   */
  public void testgetEventsOnPeriod() throws AssertionFailedException {
    System.out.println("getEventsOnPeriod");
//    fail("The test case is a prototype.");
    java.util.Vector v = null;
    int evtype = 0;
    int planet = 0;
    boolean special = true;
    long dayStart = 0L;
    long dayEnd = 0L;
    int value = 0;
    DataFile instance = new DataFile();
    instance.getEventsOnPeriod(v,evtype,planet,special,dayStart,dayEnd,value);
    
    //TODO review the generated test code and remove the default call to fail.
  }

  /**
   * Test of getAspectsOnPeriod method, of class DataFile.
   */
  public void testgetAspectsOnPeriod() throws AssertionFailedException {
    System.out.println("getAspectsOnPeriod");
//    fail("The test case is a prototype.");
    java.util.Vector v = null;
    int planet = 0;
    long dayStart = 0L;
    long dayEnd = 0L;
    DataFile instance = new DataFile();
    instance.getAspectsOnPeriod(v,planet,dayStart,dayEnd);
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  /**
   * Test of getEvents method, of class DataFile.
   */
  public void testgetEvents() throws AssertionFailedException {
    System.out.println("getEvents");
//    fail("The test case is a prototype.");
    int evtype = 0;
    int planet = 0;
    long dayStart = 0L;
    long dayEnd = 0L;
    DataFile instance = new DataFile();
    java.util.Vector expectedResult = null;
    java.util.Vector result = instance.getEvents(evtype,planet,dayStart,dayEnd);
    assertEquals(expectedResult, result);
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  /**
   * Test of getEventOnPeriod method, of class DataFile.
   */
  public void testgetEventOnPeriod() throws AssertionFailedException {
    System.out.println("getEventOnPeriod");
//    fail("The test case is a prototype.");
    int evtype = 0;
    int planet = 0;
    boolean special = true;
    long dayStart = 0L;
    long dayEnd = 0L;
    DataFile instance = new DataFile();
    Event expectedResult = null;
    Event result = instance.getEventOnPeriod(evtype,planet,special,dayStart,dayEnd);
    assertEquals(expectedResult, result);
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  /**
   * Test of isDateAvailable method, of class DataFile.
   */
  public void testisDateAvailable() throws AssertionFailedException {
    System.out.println("isDateAvailable");
//    fail("The test case is a prototype.");
    long date = 0L;
    DataFile instance = new DataFile();
    boolean expectedResult = true;
    boolean result = instance.isDateAvailable(date);
    assertTrue(expectedResult==result);
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  /**
   * Test of todayEclipse method, of class DataFile.
   */
  public void testtodayEclipse() throws AssertionFailedException {
    System.out.println("todayEclipse");
//    fail("The test case is a prototype.");
    long today = 0L;
    int delta = 0;
    DataFile instance = new DataFile();
    Event expectedResult = null;
    Event result = instance.todayEclipse(today,delta);
    assertEquals(expectedResult, result);
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  /**
   * Test of cacheData method, of class DataFile.
   */
  public void testcacheData() throws AssertionFailedException {
    System.out.println("cacheData");
//    fail("The test case is a prototype.");
    int event = 0;
    int planet = 0;
    DataFile instance = new DataFile();
    instance.cacheData(event,planet);
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }

  public void setUp() {
  }

  public void tearDown() {
  }


  /**
   * Test of readSubData method, of class DataFile.
   */
  public void testreadSubData() throws AssertionFailedException {
    System.out.println("readSubData");
    byte[] buf = null;
    int evtype = Event.EV_TITHI;
    int planet = Event.SE_MOON;
    boolean isCommon = true;
    DataFile instance = new DataFile();
    long dayStart = instance.startJD;
    long dayEnd = instance.finalJD;
    dayStart = System.currentTimeMillis();
    dayEnd = dayStart+Astromaximum.MSECINDAY;
    buf=instance.commonData;
    int expectedResult = 2;
    System.out.println("============");
    System.out.println("dayStart="+new Date(dayStart).toString());
    System.out.println("dayEnd="+new Date(dayEnd).toString());
    java.util.Vector result = instance.readSubData(buf,evtype,planet,isCommon,dayStart,dayEnd);
    assertTrue(result.size()>0);
    Astromaximum.evDump(result);
    System.out.println("============");
    
    //TODO review the generated test code and remove the default call to fail.
//    fail("The test case is a prototype.");
  }


  public void test(int testNumber) throws Throwable {
    switch(testNumber) {
//      case 0:testfillCache();break;
//      case 1:testgetEventsOnPeriod();break;
//      case 2:testgetAspectsOnPeriod();break;
//      case 3:testgetEvents();break;
//      case 4:testgetEventOnPeriod();break;
//      case 5:testisDateAvailable();break;
//      case 6:testtodayEclipse();break;
//      case 7:testcacheData();break;
//      case 8:testreadSubData();break;
      default: break;
    }
  }

  public DataFileTest() {
    super(9,"DataFileTest");
  }
}
