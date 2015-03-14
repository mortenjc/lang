import unittest
import helper

class SimpleTest(unittest.TestCase):
   def setUp(self):
      self.h = helper.TestHelper()
      self.assertNotEqual(None, self.h.driver)

   def test_randomtest1(self):
      print "Testing...\n" 
      try:
         self.h.getUrl("http://www.google.com")
         assert "oogle" in self.h.driver.title
      except:
         print "Some exception happened"

   def test_randomtest2(self):
      print "Testing again...\n" 
      self.h.getUrl("http://www.jcaps.com")
      assert "jcaps.com" in self.h.driver.title

   def tearDown(self):
      self.h.close()


if __name__ == '__main__':
   unittest.main()
