#!/bin/bash
g++ -Wall -c -I/opt/ibm/ILOG/CPLEX_Studio2211/cplex/include -I /opt/ibm/ILOG/CPLEX_Studio2211/concert/include tp3.cpp

g++ -o tp3 -L/opt/ibm/ILOG/CPLEX_Studio2211/cplex/lib/x86-64_linux/static_pic -L /opt/ibm/ILOG/CPLEX_Studio2211/concert/lib/x86-64_linux/static_pic tp3.o -lilocplex -lconcert -lcplex -lpthread -ldl

rm tp3.o

./tp3  30000_089977_usnight_del

./tp3  40000_119960_protein_del
