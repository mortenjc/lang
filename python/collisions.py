
import math

def coll(w, k):
   N = 2**w
   probUnique = 1.0
   for i in range(1, k):
      probUnique = probUnique * (N - (i - 1)) / N
   print("({:3d}b, {:6d}) {:14.10f}, {:14.10f}".format(w, i, 1 - probUnique, 1 - math.exp(-0.5 * i * (i - 1) / N)))


coll(32,  int(77163/2)) 
coll(32,  77163) 
coll(32,  77163*2) 
print("")
coll(32,   500) 
coll(32,  1000) 
coll(32,  2000) 
coll(48,   500) 
coll(48,  1000) 
coll(48,  2000) 
