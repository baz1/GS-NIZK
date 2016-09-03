# GS-NIZK project by Remi Bazin
# Implementation from the PBC library

PBC_DIR      = ../../pbc/pbc-master

pbc_static {
    INCLUDEPATH += $${PBC_DIR}/include
    LIBS        += $${PBC_DIR}/libpbc.a
    DEFINES     += PBC_STATIC
} else {
    LIBS        += -lpbc
}

LIBS        += -lgmp

config_sha512 {
    HEADERS     += sha512.h
    SOURCES     += sha512.cpp
} else {
    HEADERS     += sha256.h
    SOURCES     += sha256.cpp
}
