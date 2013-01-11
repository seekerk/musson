TEMPLATE = lib

QT += core network webkit

QT -= gui

CONFIG += plugin

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

DESTDIR = ../

contains(DEFINES, ENABLE_DEBUG)|!isEmpty(ENABLE_DEBUG) {
    CONFIG += debug
    DEFINES += DEBUG
    unix:!symbian:DEFINES += NO_RESOURCES
} else {
    CONFIG += release
    CONFIG -= debug
    DEFINES += QT_NO_DEBUG_OUTPUT
}

CONFIG (debug, debug|release) {
    OBJECTS_DIR = build/debug
} else {
    OBJECTS_DIR = build/release
}

CODECFORSRC = UTF-8

HEADERS += \
    $$PWD/drivertemplate.h \
    $$PWD/interface.h \
    $$PWD/json.h \
    $$PWD/task.h \
    $$PWD/webauth.h

SOURCES += \
    $$PWD/json.cpp \
    $$PWD/task.cpp \
    $$PWD/webauth.cpp \
    $$PWD/drivertemplate.cpp
