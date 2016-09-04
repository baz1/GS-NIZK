# GS-NIZK project by Remi Bazin
# Barreto-Naehrig curves implementation from the MIRACL library

MIRACL_DIR   = ../../miracl

LIBS        += $${MIRACL_DIR}/mr_bn.a $${MIRACL_DIR}/miracl.a

INCLUDEPATH += $${MIRACL_DIR}
