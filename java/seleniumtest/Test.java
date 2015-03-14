import java.io.IOException;
import java.io.File;
import java.util.List;

import org.apache.commons.logging.impl.Jdk14Logger;
import org.apache.commons.logging.LogFactory;
import org.apache.commons.io.FileUtils;
import static org.junit.Assert.*;

import org.openqa.selenium.By;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.OutputType;
import org.openqa.selenium.TakesScreenshot;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;



public class Test {
  
  public WebDriver driver = null;
  
  Test() {
    System.setProperty("webdriver.chrome.driver", "/Users/mjc/bin/chromedriver");
    driver = new ChromeDriver();
    assertNotNull("WebDriver not NULL", driver);
  }
  
  public void navigateTo(String url, String linkid) throws InterruptedException
  {
    driver.get(url);
    Thread.sleep(200);  // TBD time?
    if (linkid != "") {
      driver.findElement(By.id(linkid)).click();
    }
  }
  
  public void screenDumpTo(String ofile) 
  {
    File scrFile = ((TakesScreenshot)driver).getScreenshotAs(OutputType.FILE);
    try {
      FileUtils.copyFile(scrFile, new File(ofile));
    } catch(IOException e) {
      System.out.println("Caught exception " + e);
    }
  }
  
  /** 
  */
  public void getAllLinks(String url) throws InterruptedException
  {
    navigateTo(url, "");
    
    List<WebElement> links = driver.findElements(By.tagName("a"));
    for (WebElement elt : links) {
      System.out.println("Link: " + elt.getAttribute("href"));
    }
    
  }
  
  public static void main(String[] args) throws InterruptedException
  {
    
    Jdk14Logger mylog = (Jdk14Logger)LogFactory.getLog("com.sshtools");
    mylog.getLogger().setLevel(java.util.logging.Level.SEVERE);
    
    String home = "file:///Users/mjc/lang/java/seleniumtest/webpage/index.html";
    
    System.out.println("\n\n---- Initializing test driver ----");
    Test mytest = new Test();
    
    System.out.println("---- Visiting main page");
    mytest.navigateTo(home, "");
    Thread.sleep(2000);  
    
    System.out.println("---- Visiting 1st link");
    mytest.navigateTo(home, "id1");
    Thread.sleep(2000);  
    
    System.out.println("---- Screendump to file");
    mytest.screenDumpTo("screenshots/gpl.png");
    
    System.out.println("---- Back to home page");
    mytest.navigateTo(home, "");
    Thread.sleep(500);  
    System.out.println("---- Visiting 2nd link");
    mytest.navigateTo(home, "id2");
    System.out.println("---- Screendump to file");
    mytest.screenDumpTo("screenshots/stat.png");
    Thread.sleep(2000);  
    
    System.out.println("---- Read table entries");
    WebElement table = mytest.driver.findElement(By.id("ifStats"));
    // Assert table not NULL
    List<WebElement> rows = table.findElements(By.tagName("tr")); 
    for (WebElement row : rows) {
      List<WebElement> cols = row.findElements(By.tagName("td"));
      for (WebElement col : cols) {
        System.out.print(col.getText()+ "   ");
      }
      System.out.println();
    }
    
    WebElement entry = mytest.driver.findElement(By.id("ifInPackets"));
    System.out.println("---- Value of ifInPackets: " + entry.getText());
    
    mytest.navigateTo("http://10.0.0.70", "");   
    mytest.screenDumpTo("screenshots/10.0.0.70.png");
    mytest.navigateTo("http://10.0.0.71", "");    
    mytest.screenDumpTo("screenshots/10.0.0.71.png");
    Thread.sleep(500);  
    
    mytest.getAllLinks(home);
    
    System.out.println("---- End of test");
    mytest.driver.quit();
  }
}
