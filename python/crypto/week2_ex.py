#!/Library/Frameworks/Python.framework/Versions/2.7/bin/python
# !/usr/bin/python

import os
from Crypto.Cipher import AES

def strxor(a, b):     # xor two strings of different lengths
    if len(a) > len(b):
        return "".join([chr(ord(x) ^ ord(y)) for (x, y) in zip(a[:len(b)], b)])
    else:
        return "".join([chr(ord(x) ^ ord(y)) for (x, y) in zip(a, b[:len(a)])])

q1key="140b41b22a29beb4061bda66b6747e14"
q1ct="4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81"

q2key="140b41b22a29beb4061bda66b6747e14"
q2ct="5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253"

q3key="36f18357be4dbd77f050515c73fcf9f2"
q3ct="69dda8455c7dd4254bf353b773304eec0ec7702330098ce7f7520d1cbbb20fc388d1b0adb5054dbd7370849dbf0b88d393f252e764f1f5f7ad97ef79d59ce29f5f51eeca32eabedd9afa9329"

q4key="36f18357be4dbd77f050515c73fcf9f2"
q4ct="770b80259ec33beb2561358a9f2dc617e46218c0a53cbeca695ae45faa8952aa0e311bde9d4e01726d3184c34451"

SIZE = 16

def pad(s):
  lpad = (SIZE - len(s) % SIZE)
  return s + lpad*chr(lpad)

def unpad(s):
  return s[:-ord(s[-1])]

def aes_cbc_decrypt(k,c):
   cipher = AES.new(k.decode('hex'))
   cb = c.decode('hex')     # convert to bin string
   iv = cb[0:16]            # get IV
   cb = cb[16:]             # get ciphertext 
   blocks = len(cb)/16
   pt=""
   xor = iv
   for i in range(blocks):
      ctb=cb[i*SIZE:i*SIZE + 16]
      pt= pt+strxor(cipher.decrypt(ctb), xor)
      xor = ctb
   return unpad(pt)


def aes_cbc_encrypt(k,m):
   cipher = AES.new(k.decode('hex'))
   message = m.decode('ascii')     # convert to bin string
   message = pad(message)

   iv             =  os.urandom(SIZE)
   cipher_block=iv
   blocks = len(message)/16
   cipher_text=""
   for i in range(blocks):
      message_block=message[i*SIZE:i*SIZE + 16]
      cipher_block = cipher.encrypt(strxor(message_block, cipher_block))
      cipher_text= cipher_text + cipher_block 
   return iv + cipher_text 


def aes_ctr_encrypt(k,m):
   cipher_text=""
   cipher = AES.new(k.decode('hex'))
   text_bin_string = m.decode('ascii')      
   print "message length ", len(text_bin_string)

   nonce             =  os.urandom(SIZE)
   blocks = len(text_bin_string)/16 +1

   counter = int(nonce.encode('hex'),16) 
   for i in range(blocks):
      text_block = text_bin_string[i*SIZE:i*SIZE+16]
      hexcounter = hex(counter)[2:-1]
      cipher_text  += strxor(cipher.encrypt(hexcounter.decode('hex')), text_block)
      counter +=1
   return nonce + cipher_text 


def aes_ctr_decrypt(k,c):
   plain_text=""
   cipher = AES.new(k.decode('hex'))
   cipher_bin_string = c.decode('hex')      
   print "ciphertext length ", len(cipher_bin_string)

   nonce             = cipher_bin_string[0:16]  # get nonce 
   cipher_bin_string = cipher_bin_string[16:]   # rest contains ciphertext 
   blocks = len(cipher_bin_string)/16 +1

   counter = int(nonce.encode('hex'),16) 
   for i in range(blocks):
      cipher_block = cipher_bin_string[i*SIZE:i*SIZE+16]
      hexcounter = hex(counter)[2:-1]
      plain_text  += strxor(cipher.encrypt(hexcounter.decode('hex')), cipher_block)
      counter +=1 
   return plain_text 
   

print "-----problem 1------"
pt = aes_cbc_decrypt(q1key,q1ct)
print "message:", pt

print "-----problem 2------"
pt = aes_cbc_decrypt(q2key,q2ct)
print "message:", pt

print "-----problem 3------"
pt = aes_ctr_decrypt(q3key,q3ct)
print "message:", pt

print "-----problem 4------"
pt = aes_ctr_decrypt(q4key,q4ct)
print "message:", pt

print "\n-----Test encryption CTR mode------\n"
ct = aes_ctr_encrypt(q4key,"Testing encryption by AES CTR mode")
pt = aes_ctr_decrypt(q4key,ct.encode('hex'))
print "message: ", pt

print "\n-----Test encryption CBC mode------\n"
ct = aes_cbc_encrypt(q4key,"Testing encryption by AES CBC mode")
pt = aes_cbc_decrypt(q4key,ct.encode('hex'))
print "message: ", pt
