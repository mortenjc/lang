import java.util.List;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.By;

public class WebCrawler extends Test {
  
  protected java.util.HashSet<String> alreadyListed;
  protected java.util.Queue<String> queue;
  protected String baseUrl = null;
  
  public WebCrawler(String base)
  {
    baseUrl = base;
    alreadyListed = new java.util.HashSet<String>();
    queue = new java.util.LinkedList<String>();
  }
  
  protected boolean isAddressValid(String href)
  {
    if (href == null)
      return false;
    return href.startsWith(baseUrl);
  }
  
  protected void processPage(String url)
  {
    driver.get(url);
    
    List<WebElement> links = driver.findElements(By.tagName("a"));
    
    for (WebElement elt : links) {
      String href = elt.getAttribute("href");
      
      if (isAddressValid(href))
      {
        if (!alreadyListed.contains(href))
        {
          alreadyListed.add(href);
          queue.add(href);
        }
      }
    }
  }
  
  
  public void crawl(String startingUrl)
  {
    queue.add(startingUrl);
    String newAddress;
    while ((newAddress = queue.poll())!=null)
    {
      processPage(newAddress);
      System.out.println(newAddress);
    }
  }
  
  public static void main(String [] args) {
    
    WebCrawler crawler = new WebCrawler("file:///Users/mjc/lang/java/seleniumtest");
    crawler.crawl("file:///Users/mjc/lang/java/seleniumtest/webpage/index.html");
    x
    WebCrawler crawler2 = new WebCrawler("http://10.0.0.71");
    crawler2.crawl("http://10.0.0.71");
  }
  
}