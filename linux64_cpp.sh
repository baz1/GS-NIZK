#!/bin/bash

rm miracl.a
cp mirdef.hpp mirdef.h
cp mrmuldv.g64 mrmuldv.c
g++ -c -m64 -O2 mr*.c
g++ -c -m64 -O2 big.cpp
g++ -c -m64 -O2 zzn.cpp
g++ -c -m64 -O2 ecn.cpp
g++ -c -m64 -O2 ec2.cpp
g++ -c -m64 -O2 flash.cpp
g++ -c -m64 -O2 crt.cpp
ar rc miracl.a *.o
rm mr*.o
g++ -m64 -O2 bmark.c miracl.a -o bmark
g++ -m64 -O2 fact.c miracl.a -o fact
g++ -m64 -O2 mersenne.cpp miracl.a -o mersenne
g++ -m64 -O2 brent.cpp miracl.a -o brent
g++ -m64 -O2 sample.cpp miracl.a -o sample
g++ -m64 -O2 ecsgen.cpp miracl.a -o ecsgen
g++ -m64 -O2 ecsign.cpp miracl.a -o ecsign
g++ -m64 -O2 ecsver.cpp miracl.a -o ecsver
g++ -m64 -O2 pk-demo.cpp miracl.a -o pk-demo
g++ -c -m64 -O2 polymod.cpp
g++ -c -m64 -O2 poly.cpp
g++ -m64 -O2 schoof.cpp polymod.o poly.o miracl.a -o schoof

