# -*- coding: utf-8 -*-
import HTMLTestRunner
import unittest
import sys, os
import StringIO
import logging
import caselogger

import test1_MJC

# logDir and output_dir are used to create the path to logfiles and html results
logDir = []
output_dir = []

# This is the main test on HTMLTestRunner
class Test_HTMLTestRunner(unittest.TestCase):

   def test_main(self):
      # Create output dir for html and log files
      output_dir.append("output/" + logDir)
      if not os.path.exists(output_dir[0]):
         os.makedirs(output_dir[0])
 
      # Setup logging 
      caselogger.Logger(output_dir[0] + "/AutotestLog.txt").create_hierachy()

      testLogName = output_dir[0] + '/TestLog.txt'
      logger = logging.getLogger('testlogger')
      logger.addHandler(logging.FileHandler(testLogName))
      logger.setLevel(logging.DEBUG)
  
      #-------------------------------------------------------------
      #----ADD TEST CASE TO LIST OF TEST CASES BELOW--
      self.suite = unittest.TestSuite()

      for idx in range(1):
         self.suite.addTests([
            unittest.defaultTestLoader.loadTestsFromModule(test1_MJC)
         ])
      #------------------------------[  END  ]----------------------
      #-------------------------------------------------------------

      # Invoke TestRunner
      buf = StringIO.StringIO()
      fp = file(output_dir[0] +"/report.html", 'wb')
      runner = HTMLTestRunner.HTMLTestRunner(stream = fp,
                  title='Validation Test Results ',
                  description='Build Release(prototype) T&T Sailor 250.')
      runner.run(self.suite)


# Executing this module from the command line
if __name__ == "__main__":
   if len(sys.argv) > 2:
      print "Usage: python testrunner output_dir"
      sys.exit(0)
   else:
      if len(sys.argv) == 2:
         logDir = sys.argv[1]
         argv=['test_HTMLTestRunner.py', 'Test_HTMLTestRunner']             
      else:
         logDir = "default_output_dir"
         argv=['test_HTMLTestRunner.py', 'Test_HTMLTestRunner']

   unittest.main(argv=argv)

