#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
from pickle import dump
import time as timess
from random import choice,random
def shuffle(n):
    ori = list(range(n))
    for i in range(n):
        index = choice(range(n))
        ori[i],ori[index] = ori[index],ori[i]
    return ori
def heap_sort(a):
    for i in range(len(a)-1,-1,-1):
        ls = i<<1;rs = ls+1
        while ls<len(a):
            if rs == len(a) :
                if a[ls]>a[i]:a[ls],a[i]=a[i],a[ls]
                break
            elif a[rs]>max(a[ls],a[i]): a[rs],a[i]=a[i],a[rs];i = rs
            elif a[ls]>max(a[rs],a[i]): a[ls],a[i]=a[i],a[ls];i = ls
            else:break
            ls = i<<1
            rs = ls+1
    for tail in range(len(a)-1,0,-1):
        a[0],a[tail] = a[tail],a[0]
        temp = 0
        s1 = 0
        s2 = 1
        while(s1<tail):
            if s2 == tail :
                if a[s1]>a[i]:a[s1],a[temp] = a[temp],a[s1]
                break
            if a[s1] >max(a[s2],a[temp]):
                a[s1],a[temp] = a[temp],a[s1]
                temp = s1
            elif a[s2] > max(a[s1],a[temp]):
                a[s2],a[temp]=a[temp],a[s2]
                temp = s2
            else: break
            s1 = temp*2
            s2 = (temp*2)+1
    return a
def insertion_sort(a):
    result = []
    for i in a:
        if len(result)==0:result.append(i);continue
        if len(result)==1:result.insert(0 if i<result[0] else 1,i);continue
        if len(result)==2:result.insert(0 if i<result[0] else(1 if i<result[1] else 2),i);continue
        ll = 0
        rl = len(result)
        while rl-ll>1:
            can = (ll+rl)//2
            if result[can] >i :
                rl = can
            elif result[can]<i:
                ll = can
            else:
                ll = can-1
                break
        result.insert(rl,i)
    return result
time = dict()
time['insert'] = dict()
time['heap'] = dict()
time['py_built_in'] = dict()
for i in range(0,10000,100):
    print(i)
    for j in range(100):
        seq = tuple(shuffle(i))
        seq1=list(seq)
        seq2=list(seq)
        seq3=list(seq)
        a = timess.time()
        insertion_sort(seq1)
        b = timess.time()
        heap_sort(seq2)
        c = timess.time()
        seq3.sort()
        d = timess.time()
        time['insert'][i] = time['insert'].get(i,0)+b-a
        time['heap'][i] = time['heap'].get(i,0)+c-b
        time['py_built_in'][i] = time['py_built_in'].get(i,0)+d-c
    time['insert'][i]/=100
    time['heap'][i]/=100
    time['py_built_in'][i]/=100
with open('result','wb') as target:
    dump(time,target)

