#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      amatthee
#
# Created:     17/01/2013
# Copyright:   (c) amatthee 2013
# Licence:     <your licence>
#-------------------------------------------------------------------------------

from array import *
import unittest, time, re,os
from time import gmtime, strftime
import HTMLTestRunner
import sys
import string
import warnings
import types
import traceback
import atexit
import sys
import logging



class Logger:
    def __init__(self,log_filename):
        self.log_filename = log_filename
    def create_hierachy(self):
        # establish logger hierarchy to enable independent log scenarios:
        # (A) log only to file
        # (B) Log to file and console
        #  At least two nodes (excluding root) of 'deriveration' are required.
        #  Assumptions: 1)only one handler to be installed on each logger
        #                2) normal propogation
        #  -------------------------------------------------------

        logging.getLogger("logger_cor")
        logging.getLogger("logger_cor.logger_coc")

        # install file handler on highest logger and do formatting
        # install console handler on lowest Logger and do formatting
        fileH = logging.FileHandler(self.log_filename)

        fileformat = logging.Formatter('%(asctime)s: %(name)-22s: %(levelname)-10s: IN MODULE: %(module)-15s: METHOD NAME: %(funcName)-25s: IN LINE: %(lineno)-5d: %(message)-15s')
        fileH.setFormatter(fileformat)
        logging.getLogger("logger_cor").addHandler(fileH)

        # ensure that the logger itself also knows about the LVL
        logging.getLogger("logger_cor").setLevel(logging.DEBUG)

        logging.getLogger("logger_cor").propagate=False

        consoleH = logging.StreamHandler(HTMLTestRunner.stdout_redirector)
        #consoleH = logging.StreamHandler()
        consoleformat = logging.Formatter('%(asctime)s: %(name)-20s: %(levelname)-10s: IN MODULE: %(module)-15s: METHOD NAME: %(funcName)-25s: IN LINE: %(lineno)-5d: %(message)-15s')
        consoleH.setFormatter(consoleformat)
        consoleH.setLevel(logging.DEBUG)
        logging.getLogger("logger_cor.logger_coc").addHandler(consoleH)
        # ensure that the coc logger itself also knows about the LVL
        logging.getLogger("logger_cor.logger_coc").setLevel(logging.ERROR)


