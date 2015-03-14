from selenium import webdriver
import unittest
import logging
from selenium.webdriver.support.ui import Select
import time

import utilsGuiAttr

class SailorTestUtils:
   #---------- Constructor and WebDriver initialization
   def __init__ (self, ip="192.168.0.1", user="admin", pwd="1234"):
      self.ipaddr = ip
      self.user   = user
      self.pwd    = pwd
      self.LANG   = "english"
      self.BASE_PATH = "C:\Users\Bganuttest\Desktop\Auto"
      self.FF_PROFILE_PATH = self.BASE_PATH + "\Python\Firefox Profile"
      self.log    = logging.getLogger('testlogger')
      self.flog   = logging.getLogger('logger_cor')
      self.ui     = utilsGuiAttr.SailorGui()

   def getFireFoxDriver(self):
      ''' Instantiate a firefox web driver or fail the test and close browser'''
      try:
         fp = webdriver.FirefoxProfile(self.FF_PROFILE_PATH)
         fp.set_preference("extensions.lastAppVersion","10.0.2")
         fp.set_preference("extensions.checkCompatibility.10.0.2","false")
         fp.set_preference("extensions.showMisMatchUI","false")
         fp.set_preference("network.proxy.type",0)
         self.driver = webdriver.Firefox(firefox_profile=fp)
         self.driver.implicitly_wait(10)
      except:
         self.testAssert(1==0, "Could not initialize Firefox driver - check/disable proxy settings")
         driver.quit()
      return self.driver


   #---------- Assert Functions -- basic functionality
   def testAssert(self, cond, errormessage):
      ''' Used to assert a logical condition and write error messages to log files
          also uses assert to fail or pass the teststep '''
      if not cond:
         self.log.error("   NOT OK: " + errormessage)
         self.flog.error(errormessage)
      else:
         self.log.error("   OK:")
      assert cond, errormessage


   def assertFalse(self, errormessage):
      ''' Makes an assertion that fails, log errormessage'''
      self.testAssert(1 == 0, errormessage)


   def assertTextPresent(self, text, errormessage):
      ''' Assert that text[] is present somewhere in the body of a page'''
      bodytext = self.driver.find_element_by_tag_name('body').text
      for txt in text:
         self.testStep("check for text: { " + txt + " } present")
         self.testAssert(txt in bodytext, errormessage)


   def assertTerminalReachable(self, ip, text):
      ''' Tests that driver works and that we have connectivity to the 
          Device Under Test
      '''
      self.driver.get("http://" + ip)
      title = self.driver.title
      self.testAssert( text in title, "Could not connect to DUT on " + ip)


   #-------------------------------------------------------------
   #---------- Logging functions -- basic functionality ---------
   def testStep(self, description):
      ''' Used to log a test step description to both log files '''
      self.log.info("TESTSTEP "+description)        
      self.flog.info("TESTSTEP "+description)


   #-------------------------------------------------------------
   #---------- Terminal specific helper functions ---------------

   def makeURL(self, page, options=""):
      ''' Construct a valid Sailor URL from the pageID and options
          ip address and language settings are taken from constructor, and inserted
      '''
      return "http://" + self.ipaddr + "/index.lua?pageID=" + page + "&langID=" + self.LANG + options


   def loginAsAdmin(self):
      ''' Log in as administrator, use ip address username and password from constructor
      '''
      self.driver.find_element_by_name("usernameAdm").clear()
      self.driver.find_element_by_name("usernameAdm").send_keys(self.user)
      self.driver.find_element_by_name("passwordAdm").clear()
      self.driver.find_element_by_name("passwordAdm").send_keys(self.pwd)
      self.driver.find_element_by_css_selector("input[type=\"submit\"]").click()
      self.assertTextPresent(["Please remember to log off "], "Couldn't login - check username and password")


   def logOut(self):
      self.driver.get(self.makeURL("Administration","&adminLogOff=logoff"))
      self.assertTextPresent(["Please enter administrator username"], "Login page not found (or UI changed)")


   def getIdentifiers(self):
      ''' this utility function shoud scrape serial number, IMEI, IP address and sw version
          from the UT and log these identifiers to the log files.
      '''
      self.driver.get(self.makeURL("Home"))
      text = ["IMEI number", "(No active data sessions)", "Software version"]
      self.assertTextPresent(text,"Dashboard UI Changes?")

      # Add scraping functions below
      # Possibly just dump a screenshot

      txt = self.driver.find_element_by_xpath("//tr[7]/td[3]/p").text
      self.testStep("Unit serial number: " + txt)
      txt = self.driver.find_element_by_xpath("//tr[8]/td[3]/p").text
      self.testStep("Software version  : " + txt)
      txt = self.driver.find_element_by_xpath("//tr[10]/td[3]/p").text
      self.testStep("IMEI number       : " + txt)
      # Note the ugly //tr[]... syntax above. There is no easy way to make 
      # this reliable given the existing HTML code. For the future remember to
      # name ALL fields containing data.


   def selectElementByName(self, name, text=""):
      ''' '''
      try:
         elt = self.driver.find_element_by_name(name)
      except:
         self.assertFalse("Could not find element '" + name + "'")
      if text != "":
         obj = Select(elt).select_by_visible_text(text)
      else:
         obj = Select(elt)
      return obj


   def clickElementByName(self, name):
      '''  Find an element by name, and if successful click() it'''
      try:
         elt = self.driver.find_element_by_name(name)
      except:
         self.assertFalse("Could not find element by name'" + name + "'")
      try:
         elt.click()
      except:
         self.assertFalse("Element '" + name + "' could not be clicked()") 
      return elt

   def clickElementByXpath(self, xpath):
      '''  Find an element by name, and if successful click() it'''
      try:
         elt = self.driver.find_element_by_xpath(xpath)
      except:
         self.assertFalse("Could not find element by xpath '" + xpath + "'")
      try:
         elt.click()
      except:
         self.assertFalse("Element '" + name + "' could not be clicked()") 
      return elt


   def alertBoxOk(self):
      ''' Attempt to click accept in popup box'''
      try:
         alert = self.driver.switch_to_alert()
         alert.accept()
      except:
         self.assertFalse("Failed clicking accept in alertbox")


   def dhcpEnable(self):
      ''' Enable DHCP Server - validate correct page is found'''
      self.driver.get(self.makeURL("LAN"))
      self.assertTextPresent(["DHCP status","Local IP address"], "Could not access LAN page")
      self.driver.find_element_by_name("radio_onoff_dhcp").click()
      self.driver.find_element_by_name("submit").click()

      elts = self.driver.find_elements_by_name("radio_onoff_dhcp")
      for elt in elts:
         val = elt.get_attribute("value")
         self.testStep("Checking Button: " + val)
         if "enabled" in val:
            self.testAssert(elt.is_selected() == True, "Could not select DHCP - enabled button not selected")
         else:
            self.testAssert(elt.is_selected() == False, "Could not select DHCP - disabled button not deselected")


   def autoActivationDisable(self):
      '''  '''
      ui = self.ui
      self.testStep("Goto edit user group (10)")
      self.driver.get(self.makeURL("Network%20user%20groups", "&oper=edit&idx=10"))

      self.clickElementByXpath(ui.elements["DIS_AUTO"])
      self.clickElementByName(ui.elements["APPLY_UG"])
      # done


   #----------------------------------------------------------------------------------------
   #---------- Compound commands used in test steps - makes use of the above utilities -----

   def deselectAllDataLimits(self):
      ''' Deselect any checked data limit boxes. Log those who are toggled.
      '''
      self.driver.get(self.makeURL("Data%20limits"))
      text = ["Estimated max charge","your actual bill"]
      self.assertTextPresent(text, "Unable to reach Data limits page (UI change?)")
      
      checkBoxes = ["ChkStdData","ChkStream8","ChkStream16","ChkStream32","ChkStream64","ChkStream128"]

      for boxName in checkBoxes:
         element = self.driver.find_element_by_name(boxName)
         if element.is_selected() == True:
            self.testStep("Deselcting " + boxName)
            element.click()

      self.driver.find_element_by_name("submit").click()

      for boxName in checkBoxes:
         element = self.driver.find_element_by_name(boxName)
         self.testStep("Asserting " + boxName + " is deselected")
         self.testAssert(element.is_selected() == False, "Failure to deselect data limit on " + boxName)

   

   def selectStandardDataOnly(self):
      ''' Select only the Standard Data connection to be available'''
      driver = self.driver
      ui = self.ui
      self.dhcpEnable()

      self.testStep("Navigate to user group 10 and edit")
      driver.get(self.makeURL("Network%20user%20groups", "&oper=edit&idx=10"))
      asserttext = ["Changes to Status and Internet","If using secondary profiles"]
      self.assertTextPresent(asserttext, "Unable to reach 'Network User Group edit page'")

      time.sleep(2)

      primConn=self.selectElementByName("primaryconnectionclass")
      primConn.deselect_all()
      primConn.select_by_visible_text("Standard")

      secConn = self.selectElementByName("secondaryconnectionclass")
      secConn.deselect_all()

      self.clickElementByName(ui.elements["APPLY_UG"])
      # done






