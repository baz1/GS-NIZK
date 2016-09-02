#-------------------------------------------------
#
# Project created by QtCreator 2016-06-30T02:16:33
#
#-------------------------------------------------

# GS-NIZK project by Remi Bazin

# ==========  BUILD MAIN CONFIGURATION  ==========

# Are we compiling the library?
# (else, just the test program is compiled)
#CONFIG      += config_lib

# Which configuration are we using? (MIRACL? PBC?)
CONFIG      += config_miracl
#CONFIG      += config_pbc

# Do we want to enable debug mode?
# (note: may be overwritten by Qt's NO_DEBUG macro)
DEFINES     += DEBUG

# ========== MIRACL ONLY CONFIGURATION  ==========

# What kind of security level do we want?
DEFINES     += AES_SECURITY=128
#DEFINES     += AES_SECURITY=192

# Do we want to optimize the iostream operations
# by assuming that the library will not be used on
# several different threads?
DEFINES     += GSNIZK_IOSTREAM_NOTHREADS

# ==========   PBC ONLY CONFIGURATION   ==========

# Are we using PBC as a static library?
# (note that for convenience and clarity purposes,
# the includes will still be looking for global
# headers first)
CONFIG      += pbc_static

# ==========  END OF THE CONFIGURATION  ==========

DEFINES     += LIB_COMPILATION

QT          -= core gui
TARGET       = gsnizk
CONFIG      += c++11

config_lib {
    TEMPLATE     = lib
    CONFIG      += staticlib
    unix {
        target.path  = /usr/lib
        INSTALLS    += target
    }
} else {
    TEMPLATE     = app
    CONFIG      += console
    CONFIG      -= app_bundle
    SOURCES     += main.cpp \
        test.cpp
}


SOURCES     += gsnizk.cpp \
    pairings.cpp \
    maps.cpp \
    bigendian.cpp

HEADERS     += gsnizk.h \
    pairings.h \
    maps.h \
    bigendian.h

config_miracl {
    include(BN.pri)
    DEFINES     += USE_MIRACL
} else {
    config_pbc {
        include(PBC.pri)
        DEFINES     += USE_PBC
    } else {
        error("QMake configuration error: neither MIRACL nor PBC selected")
    }
}

DISTFILES   += mainpage.dox
