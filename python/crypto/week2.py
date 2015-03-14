#!/usr/bin/python

from string import ascii_letters, digits, punctuation

def strxor(a, b):     # xor two strings of different lengths
    if len(a) > len(b):
        return "".join([chr(ord(x) ^ ord(y)) for (x, y) in zip(a[:len(b)], b)])
    else:
        return "".join([chr(ord(x) ^ ord(y)) for (x, y) in zip(a, b[:len(a)])])

def bufferToAscii(buffer, start, count):
    accumulator = ''
    for item in range(count):
        char = chr(ord(buffer[start + item]))
        if char in ascii_letters or \
           char in digits or \
           char in punctuation or \
           char == ' ':
            accumulator += char
        else:
            accumulator += '.'
    return accumulator


a0="4af532671351e2e1"
a1="87a40cfa8dd39154"
b0="9d1a4f78cb28d863"
b1="75e5e3ea773ec3e6"
c0="2d1cfa42c0b1d266"
c1="eea6e3ddb2146dd0"
d0="290b6e3a39155d6f"
d1="d6f491c5b645c008"

def hexor(s1,s2):
  a = strxor(s1.decode('hex'),s2.decode('hex'))
  print a.encode('hex')
  return

hexor(a0,a1)
hexor(b0,b1)
hexor(c0,c1)
hexor(d0,d1)

print
print "---------"
print "Question 8"
print "---------"
print

s1='If qualified opinions incline to believe in the exponential conjecture, then I think we cannot afford not to make use of it.'
s2='In this letter I make some remarks on a general principle relevant to enciphering in general and my machine.'
s3='To consider the resistance of an enciphering process to being broken we should assume that at same times the enemy knows everything but the key being used and to break it needs only discover the key from this information.'
s4='We see immediately that one needs little information to begin to break down the process.'

print "s1 length", len(s1)
print "s2 length", len(s2)
print "s3 length", len(s3)
print "s4 length", len(s4)
