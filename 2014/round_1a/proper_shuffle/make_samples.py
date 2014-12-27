#! /usr/bin/python

import random, sys, io, collections
import math

def good_seq(n):
    lst = list(range(0,n))
    for i in range(0,n):
        r = random.randrange(i, n)
        lst[i], lst[r] = lst[r], lst[i]
    return lst

def bad_seq(n):
    lst = list(range(0,n))
    for i in range(0,n):
        r = random.randrange(0, n)
        lst[i], lst[r] = lst[r], lst[i]
    return lst

seqs = []

if len(sys.argv) < 2:
    print("Usage: %s [g|b]"%sys.argv[0])
    sys.exit(0)

if sys.argv[1] == "g":
    fn = "good.txt"
    good = 1
else:
    fn = "bad.txt"
    good = 0


n = 1000
t = 120
print(t)
for i in range(0,t):
    print(n)
    if good == 1:
        seqs.append(good_seq(n))
    else:
        seqs.append(bad_seq(n))
    for j in range(0,n):
        print(seqs[i][j],end=" ")
    print("")

