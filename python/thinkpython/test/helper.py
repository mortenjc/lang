from selenium import webdriver
from selenium.webdriver.common.keys import Keys
import unittest

class TestHelper():
   def __init__(self, driver=""):
      self.driver = webdriver.Firefox()

   def getUrl(self, url):
      self.driver.get(url)

   def getElement(name):
      self.driver.find_element_by_name(name)

   def close(self):
      self.driver.close()

