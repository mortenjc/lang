

def divby02():
   4/0
   print "   End of function"


def divby0():
   try:
      4/0
   except:
      print "      A Division by zero error"
   print "   End of function"


try:
   divby0()
except:
   print "   Didnt work"
print "End of test"

try:
   divby02()
except:
   print "   Didnt work"
print "End of test2"
