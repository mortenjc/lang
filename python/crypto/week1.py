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

m1="attack at dawn"
c1="6c73d5240a948c86981bc294814d"
m2="attack at dusk"

a = c1.decode('hex')
m = strxor(m1,a)
c = strxor(m, m2)
print c.encode('hex')  
