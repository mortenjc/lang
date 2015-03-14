


def line1(size):
   for i in range(0, size*2+1):
      if (i % size) == 0:
        print "+", 
      else:
        print "-", 
   print
 
def line2(size):
   for i in range(0, size*2+1):
      if (i % size) == 0:
        print "|", 
      else:
        print " ", 
   print


def grid(size):
   for i in range(0, size*2+1):
      if (i % size) == 0:
         line1(size)
      else:
         line2(size) 


grid(1)
grid(2)
grid(3)
grid(4)
grid(5)
