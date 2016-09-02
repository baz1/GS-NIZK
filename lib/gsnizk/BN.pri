# GS-NIZK project by Remi Bazin
# Barreto-Naehrig curves implementation from the MIRACL library

MIRACL_DIR   = ../../miracl

SOURCES     += $${MIRACL_DIR}/bn_pair.cpp \
    $${MIRACL_DIR}/zzn12a.cpp \
    $${MIRACL_DIR}/ecn2.cpp \
    $${MIRACL_DIR}/zzn4.cpp \
    $${MIRACL_DIR}/zzn2.cpp \
    $${MIRACL_DIR}/big.cpp \
    $${MIRACL_DIR}/zzn.cpp \
    $${MIRACL_DIR}/ecn.cpp

LIBS        += $${MIRACL_DIR}/miracl.a

INCLUDEPATH += $${MIRACL_DIR}
