#!/usr/bin/python

from Crypto.Hash import SHA256

def get_bytes_from_file(filename):
    return open(filename, "rb").read()

file = get_bytes_from_file("week3.mp4")

BLKSZ  =1024

blocks = len(file)/BLKSZ

chunk  = file[blocks*BLKSZ:]
sha    = SHA256.new(chunk).digest()

for i in range(blocks):
   offset = (blocks-i-1)*BLKSZ
   chunk  = file[offset:offset+BLKSZ] + sha
   sha    = SHA256.new(chunk).digest()

print sha.encode('hex')
