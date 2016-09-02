# GS-NIZK project by Remi Bazin
# Barreto-Naehrig curves implementation from the MIRACL library

SOURCES     += miracl/bn_pair.cpp \
    miracl/zzn12a.cpp \
    miracl/ecn2.cpp \
    miracl/zzn4.cpp \
    miracl/zzn2.cpp \
    miracl/big.cpp \
    miracl/zzn.cpp \
    miracl/ecn.cpp

LIBS        += miracl/miracl.a

INCLUDEPATH += miracl
