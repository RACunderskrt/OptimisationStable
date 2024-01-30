#!/usr/bin/python3

import sys
import math
import random
import time
import itertools

if len(sys.argv) != 2:
  print("./tester.py n")
  print("n = 30, 100, 300, 1000, 3000, 10000, 30000")
  exit(1)

n = int(sys.argv[1])

fnh = {
  30: '00030_000109_random_udg',
  100: '00100_001155_qatar_udg',
  300: '00300_001484_euronight_8nn',
  1000: '01000_002980_greece_del',
  3000: '03000_010884_paris_6nn',
  10000: '10000_024304_italy_4nn',
  20000: '20000_107511_jupiter_9nn',
  30000: '30000_089977_usnight_del',
  40000: '40000_119965_protein_del',
  50000: '50000_149963_sun_del'
}

def readEdges(fn):
  E = []
  with open(fn) as f:
    for line in f:
      if line:
        E.append(tuple(int(x) for x in line.split()))
  return E

def buildGraph(E):
  G = {}
  for u,v in E:
    if u in G:
      G[u].add(v)
    else:
      G[u] = {v}
    if v in G:
      G[v].add(u)
    else:
      G[v] = {u}
      
  return G

def readDom(fn):
  with open(fn) as f:
    D = {int(line) for line in f if line}
  return D

def testDom(G,D):
  undom = set()
  for v in G.keys():
    if v not in D and not(G[v] & D):
      print(f"Erreur : Sommet {v} n'est pas domine")
      undom.add(v)
  return undom

def readPoints(fn):
  P = []
  with open(fn) as f:
    for line in f:
      if line:
        P.append(tuple(float(x) for x in line.split()))
  return P

def writeSVG(fn, P, E, D, undom):
  imgsz = 1000.0
  x0 = min([p[0] for p in P])
  y0 = min([p[1] for p in P])
  x1 = max([p[0] for p in P])
  y1 = max([p[1] for p in P])
  sz = max(x1-x0, y1-y0)
  r2 = 10.0

  with open(fn, "w") as fsvg:
    fsvg.write('<?xml version="1.0" encoding="utf-8"?>\n')
    fsvg.write(f'<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="{imgsz+2*r2}" height="{imgsz+2*r2}">\n')
    
    fsvg.write(f' <text x="0" y="20">|D|={len(D)}</text>\n')

    for e in E:
      q1 = (r2 + (P[e[0]][1]-y0) * imgsz / sz, r2 + (x1-P[e[0]][0]) * imgsz / sz)
      q2 = (r2 + (P[e[1]][1]-y0) * imgsz / sz, r2 + (x1-P[e[1]][0]) * imgsz / sz)
      fsvg.write(f' <polygon stroke="black" stroke-width="1" fill="none" points="{q1[0]},{q1[1]} {q2[0]},{q2[1]}"/>\n')

    for i in range(len(P)):
      p = P[i]
      q = (r2 + (p[1]-y0) * imgsz / sz, r2 + (x1-p[0]) * imgsz / sz)
      if i in D:
        fsvg.write(f' <circle stroke="darkgreen" fill="lightgreen" stroke-width="2" cx="{q[0]}" cy="{q[1]}" r="5">\n')
        fsvg.write(f'  <title>{i} D</title>\n')
        fsvg.write(' </circle>\n')
      elif i in undom:
        fsvg.write(f' <circle stroke="black" fill="red" stroke-width="2" cx="{q[0]}" cy="{q[1]}" r="6">\n')
        fsvg.write(f'  <title>{i} !</title>\n')
        fsvg.write(' </circle>\n')
      else:
        fsvg.write(f' <circle stroke="darkblue" stroke-width="2" cx="{q[0]}" cy="{q[1]}" r="3">\n')
        fsvg.write(f'  <title>{i}</title>\n')
        fsvg.write(' </circle>\n')

    fsvg.write("</svg>\n")

E = readEdges(fnh[n]+".edges")
G = buildGraph(E)
assert(max(G.keys()) < n)
D = readDom(fnh[n]+".dom")
print(f"La taille de D est {len(D)}")

undom = testDom(G,D)
if undom:
  print(f"Il y {len(undom)} erreurs")
else:
  print("Pas d'erreur")

P = readPoints(fnh[n]+".points")
assert(len(P) == n)
writeSVG(fnh[n]+".dom.svg", P, E, D, undom)
print("Fichier SVG cree")
