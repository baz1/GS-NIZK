# GS-NIZK project by Remi Bazin
# Implementation from the PBC library

PBC_DIR      = ../../pbc/pbc-master

LIBS        += -lgmp

pbc_static {
    INCLUDEPATH += $${PBC_DIR}/include
    LIBS        += $${PBC_DIR}/libpbc.a
    DEFINES     += PBC_STATIC
} else {
    LIBS        += -lpbc
}
