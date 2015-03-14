#!/usr/bin/python

from itertools import *

print "W R O N G     R I G H T"
for W,R,O,N,G in permutations('0123456789', 5):
   for I,H,T in permutations('0123456789', 3):
      if not(I==R or I==G or H==R or H==G or T==R or T==G):
         res = int(W)*20000 + int(O)*200  + int(N)*20 
         res-= int(R)*8000  + int(I)*1000 + int(G)*98 + int(H)*10 + int(T)
         if (not res):
            print W,R,O,N,G,"   ", R,I,G,H,T
