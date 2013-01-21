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
MOC_DIR = build/moc

CODECFORSRC = UTF-8

HEADERS += \
    $$PWD/drivertemplate.h \
    $$PWD/json.h \
    $$PWD/task.h \
    $$PWD/webauth.h

SOURCES += \
    $$PWD/json.cpp \
    $$PWD/task.cpp \
    $$PWD/webauth.cpp \
    $$PWD/drivertemplate.cpp

include(driver.pri)

isEmpty(PREFIX):PREFIX = /usr
message("Prefix is $$PREFIX")

isEmpty(DRIVERSDIR): {
    win32 {
	DRIVERSDIR=$$PREFIX/drivers
    } else {
	DRIVERSDIR=$$PREFIX/share/musson/drivers
    }
} else {
    message("You can specify DRIVERSDIR to define install path")
}
message("Install path is $$DRIVERSDIR")


