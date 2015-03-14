#!/usr/bin/python

import random

def bit(n, a):
   byte = int((n-1) / 8) 
   nb = n - byte * 8
   return (a[byte] >> (nb-1)) & 1

def hashing(a):
   res=0
   bi=""
   for of in range(6):
     h =   bit( 1+of,a)^bit( 7+of,a)^bit(13+of,a)^bit(19+of,a)
     h = h^bit(25+of,a)^bit(31+of,a)^bit(37+of,a)^bit(43+of,a)
     res+= h<<of
     bi= str(h)+ " "  + bi
   print "Hash indexes ", bi , "  Value: ", res
   return res

random.seed(020567)

a = []
for i in range(32):
  res = hashing([1,0,0x54,0,0,i])
  if not (res in a):
     a.append(res)
print "XOR hashing: Sequential", len(a) 
a = []
for i in range(32):
  res = hashing([1,0,0x54,random.getrandbits(8),random.getrandbits(8),random.getrandbits(8)])
  if not (res in a):
     a.append(res)
print "XOR hashing: Random", len(a) 


