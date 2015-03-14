# Framework imports
from selenium import webdriver
import unittest

# imports of Cobham developed files
from utilsCommon import *

class test1_MJC(unittest.TestCase):
  
   def setUp(self):
      self.helper  = SailorTestUtils("192.168.0.1", "admin", "1234") # from CommonUtils
      self.driver  = self.helper.getFireFoxDriver()                  # from CommonUtils        

      h = self.helper
      # Initial checks
      h.testStep("Checking connectivity to DUT")
      h.assertTerminalReachable(self.helper.ipaddr, "SAILOR 250")

      h.testStep("Getting DUT identifiers")
      h.getIdentifiers()

      h.testStep("Start test by logging out")
      h.logOut()

   def tearDown(self):
      self.driver.quit()

   # Testcase 1058_113
   def test_1058_113(self):
      h = self.helper

      h.testStep("Login as Admin")
      h.loginAsAdmin()

      # Actual test
      h.testStep("Deselect all data limits")
      h.deselectAllDataLimits()
      h.testStep("Select Standard Data only")
      h.selectStandardDataOnly()
    
      h.testStep("Disable autoactivation feature")
      h.autoActivationDisable()
      # not completed yet


   # Testcase 1058_114
   def test_1058_114(self):
      h = self.helper
 
      h.testStep("This is a dummy text")
      # Not completed yet


if __name__ == "__main__":
   unittest.main()
