#!/Library/Frameworks/Python.framework/Versions/2.7/bin/python

c="f20bdba6ff29eed7b046d1df9fb7000058b1ffb4210a580f748b4ac714c001bd4a61044426fb515dad3f21f18aa577c0bdf302936266926ff37dbf7035d5eeb4"

import urllib2
import sys

TARGET = 'http://crypto-class.appspot.com/po?er='
#--------------------------------------------------------------
# padding oracle
#--------------------------------------------------------------
class PaddingOracle(object):
    def query(self, q):
        target = TARGET + urllib2.quote(q)    # Create query URL
        req = urllib2.Request(target)         # Send HTTP request to server
        try:
            f = urllib2.urlopen(req)          # Wait for response
        except urllib2.HTTPError, e:          
            #print "We got: %d" % e.code       # Print response code
            if e.code == 404:
                return True # good padding
            return False # bad padding

def getBlock(i):
   return c[i*32:i*32+32]

po = PaddingOracle()
po.query(c)       # Issue HTTP query with the given argument

for i in range(len(c)/32):
   print "block", i, getBlock(i)


message= []

m0 = getBlock(0)
m1 = getBlock(1)
m3 = getBlock(3)
a = getBlock(2).decode('hex')
b = a[:-1]
for i in range(255):
  print i
  g = chr(ord(a[-1:]) ^ 0x01 ^ i)
  msg = m0 + m1 + (b + g).encode('hex') + m3
  if (po.query(msg)):
     print "<<<<", i 
     break

b = a[:-2]
for i in range(255):
  print i
  g = chr(ord(a[-2:-1]) ^ 0x02 ^ i)
  g2 = chr(ord(a[-1:]) ^ 0x02 ^ 9)
  msg = m0 + m1 + (b + g+g2).encode('hex') + m3
  if (po.query(msg)):
     print "<<<<", i 
     break

b = a[:-3]
for i in range(255):
  print i
  g0= chr(ord(a[-3:-2]) ^ 0x03 ^ i)
  g1= chr(ord(a[-2:-1]) ^ 0x03 ^ 9)
  g2 = chr(ord(a[-1:]) ^ 0x03 ^ 9)
  msg = m0 + m1 + (b + g0+g1+g2).encode('hex') + m3
  if (po.query(msg)):
     print "<<<<", i 
     break

b = a[:-4]
for i in range(255):
  print i
  g0= chr(ord(a[-4:-3]) ^ 0x04 ^ i)
  g1= chr(ord(a[-3:-2]) ^ 0x04 ^ 9)
  g2= chr(ord(a[-2:-1]) ^ 0x04 ^ 9)
  g3 = chr(ord(a[-1:]) ^ 0x04 ^ 9)
  msg = m0 + m1 + (b + g0+g1+g2+g3).encode('hex') + m3
  if (po.query(msg)):
     print "<<<<", i 
     break

b = a[:-5]
for i in range(255):
  print i
  g0= chr(ord(a[-5:-4]) ^ 0x05 ^ i)
  g1= chr(ord(a[-4:-3]) ^ 0x05 ^ 9)
  g2= chr(ord(a[-3:-2]) ^ 0x05 ^ 9)
  g3= chr(ord(a[-2:-1]) ^ 0x05 ^ 9)
  g4 = chr(ord(a[-1:]) ^ 0x05 ^ 9)
  msg = m0 + m1 + (b + g0+g1+g2+g3+g4).encode('hex') + m3
  if (po.query(msg)):
     print "<<<<", i 
     break
 

j=6 
b = a[:-j]
for i in range(255):
  print i
  g0= chr(ord(a[-j:-j+1]) ^ j ^ i)
  g1= chr(ord(a[-5:-4]) ^ 0x06 ^ 9)
  g2= chr(ord(a[-4:-3]) ^ 0x06 ^ 9)
  g3= chr(ord(a[-3:-2]) ^ 0x06 ^ 9)
  g4= chr(ord(a[-2:-1]) ^ 0x06 ^ 9)
  g5 = chr(ord(a[-1:]) ^ 0x06 ^ 9)
  msg = m0 + m1 + (b + g0+g1+g2+g3+g4+g5).encode('hex') + m3
  if (po.query(msg)):
     print "<<<<", i 
     break
  
b = a[:-7]
for i in range(255):
  print i
  g0= chr(ord(a[-j:-j+1]) ^ 0x07 ^ i)
  g1= chr(ord(a[-j:-j+1]) ^ 0x07 ^ 9)
  g2= chr(ord(a[-5:-4]) ^ 0x07 ^ 9)
  g3= chr(ord(a[-4:-3]) ^ 0x07 ^ 9)
  g4= chr(ord(a[-3:-2]) ^ 0x07 ^ 9)
  g5= chr(ord(a[-2:-1]) ^ 0x07 ^ 9)
  g6 = chr(ord(a[-1:]) ^ 0x07 ^ 9)
  msg = m0 + m1 + (b + g0+g1+g2+g3+g4+g5+g6).encode('hex') + m3
  if (po.query(msg)):
     print "<<<<", i 
     break

po.query(c)       # Issue HTTP query with the given argument
