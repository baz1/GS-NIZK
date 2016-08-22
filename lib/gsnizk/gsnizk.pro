#-------------------------------------------------
#
# Project created by QtCreator 2016-06-30T02:16:33
#
#-------------------------------------------------

QT       -= core gui

TARGET = gsnizk
TEMPLATE = lib

#CONFIG += config_lib
CONFIG += config_miracl
#CONFIG += config_pbc
DEFINES += AES_SECURITY=128
#DEFINES += AES_SECURITY=192
DEFINES += GSNIZK_IOSTREAM_NOTHREADS
DEFINES += DEBUG

DEFINES += LIB_COMPILATION

config_lib {
    CONFIG += staticlib
    unix {
        target.path = /usr/lib
        INSTALLS += target
    }
} else {
    CONFIG += console
    CONFIG -= app_bundle
    TEMPLATE = app
    SOURCES += main.cpp
}

CONFIG += c++11

SOURCES += gsnizk.cpp \
    pairings.cpp \
    test.cpp \
    maps.cpp \
    bigendian.cpp

HEADERS += gsnizk.h \
    pairings.h \
    maps.h \
    bigendian.h

config_miracl {
    include(BN.pri)
    DEFINES += USE_MIRACL
} else {
    config_pbc {
        # ...
    } else {
        error("QMake configuration error: neither MIRACL nor PBC selected")
    }
}

DISTFILES += \
    mainpage.dox
