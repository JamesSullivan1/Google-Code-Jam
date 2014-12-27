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

def hist(o_dict):
    string = ""
    for (key, value) in o_dict.items():
        times = math.floor(value / 10)
        string += str(key)
        string += '\n'
        string += ('=' * times)
        string += '\n'
    print(string)

def delta_hist(o_dict, norm):
    string = ""
    for (key, value) in o_dict.items():
        times = math.floor((value - norm) / 10)
        neg = times < 0
        string += str(key)
        string += '\n'
        if neg:
            string += ('-' * abs(times))
        else:
            string += ('+' * abs(times))
        string += '\n'
    print(string)

seqs1 = []
seqs2 = []

freq1 = dict()
freq2 = dict()

n = 20
for i in range(0,10000):
    seqs1.append(good_seq(n))
    seqs2.append(bad_seq(n))

for i in range(0,10000):
    for j in range(0, n):
        if freq1.get((j, seqs1[i][j])) != None:
            freq1[(j,seqs1[i][j])]+= 1
        else:
            freq1[(j,seqs1[i][j])] =1

        if freq2.get((j, seqs2[i][j])) != None:
            freq2[(j,seqs2[i][j])]+= 1
        else:
            freq2[(j,seqs2[i][j])] =1

norm = 10000 / 20 # 20 choose 2 = 190
ofreq1 = collections.OrderedDict(sorted(freq1.items()))
ofreq2 = collections.OrderedDict(sorted(freq2.items()))
print("HISTOGRAM\n"
        "(Elem, Pos)\n(=) for each 10 occurrences\n"
        ,'=' * 72,sep="")
hist(ofreq2)
print("DELTA HISTOGRAM\n"
        "(Elem, Pos)\n(+/-) for each difference of 10 from norm\n"
        ,'=' * 72,sep="")
delta_hist(ofreq2, norm)

