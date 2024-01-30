#!/usr/bin/pypy3

import sys
import math
import random
import time
import itertools

if len(sys.argv) != 2:
  print("./svg.py edges")
  exit(1)

fn = sys.argv[1]

E = []
P = {}
with open(fn, 'r') as f:
  for line in f:
    E.append(tuple(int(x) for x in line.split()))
    for v in E[-1]:
      P[v] = (v // 100000, v % 100000)

imgsz = 1000.0
x0 = min([p[0] for p in P.values()])
y0 = min([p[1] for p in P.values()])
x1 = max([p[0] for p in P.values()])
y1 = max([p[1] for p in P.values()])
sz = max(x1-x0, y1-y0)
r2 = 10.0

fsvg = open(fn+".svg", "w")
fsvg.write('<?xml version="1.0" encoding="utf-8"?>\n')
fsvg.write('<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="'+str(imgsz+2*r2)+'" height="'+str(imgsz+2*r2)+'">\n')

for e in E:
  q1 = (r2 + (P[e[0]][1]-y0) * imgsz / sz, r2 + (x1-P[e[0]][0]) * imgsz / sz)
  q2 = (r2 + (P[e[1]][1]-y0) * imgsz / sz, r2 + (x1-P[e[1]][0]) * imgsz / sz)
  fsvg.write('<polygon stroke="black" stroke-width="1" fill="none" points="')
  fsvg.write(str(q1[0])+","+str(q1[1])+" "+str(q2[0])+","+str(q2[1]))
  fsvg.write('"/>\n')

fsvg.write('\n')

for i,p in P.items():
  q = (r2 + (p[1]-y0) * imgsz / sz, r2 + (x1-p[0]) * imgsz / sz)
  fsvg.write('<circle stroke="blue" stroke-width="2" cx="'+str(q[0])+'" cy="'+str(q[1])+'" r="1">\n')
  fsvg.write(f' <title>{i}</title>\n')
  fsvg.write('</circle>\n')

fsvg.write("</svg>\n")


