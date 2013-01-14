
# include private developer settings
exists(../devel.pri):include(../devel.pri)
exists(../../devel.pri):include(../../devel.pri)
exists(../../../devel.pri):include(../../../devel.pri)

QT += declarative xml network

CONFIG += thread

TARGET = musson-gallery

CONFIG (debug, debug|release) {
    OBJECTS_DIR = build/debug
} else {
    OBJECTS_DIR = build/release
}
MOC_DIR = build/moc

!symbian:DEFINES += LOCALDATADIR=\\\"/.musson/\\\"
unix {
    #VARIABLES
    isEmpty(PREFIX) {
        PREFIX = $$[QT_INSTALL_PREFIX]
    }
}
message(prefix is $$PREFIX)

# prefix for search shared files
DATAPREFIX=$$PREFIX/share
DEFINES += DATAPREFIX=\\\"$$DATAPREFIX\\\"
message(dataprefix is $$DATAPREFIX)

BINDIR = $$PREFIX/bin
DATADIR =$$PREFIX/share
LIBDIR = $$PREFIX/lib
!maemo5:!contains(MEEGO_EDITION,harmattan):contains(QMAKE_HOST.arch, x86_64)::LIBDIR = $$PREFIX/lib64

# define plugin prefix (Core and Components)
# PLUGINPREFIX=$$LIBDIR
# DEFINES += PLUGINPREFIX=\\\"$$PLUGINPREFIX\\\"
# message(plugin prefix is $$PLUGINPREFIX)

# Harmattan specific
contains(MEEGO_EDITION, harmattan) {
    CONFIG += SET_SPECIFIC_QML
    include(harmattan.pri)
}

# Symbian specific
symbian {
    CONFIG += SET_SPECIFIC_QML
    include(symbian.pri)
}

simulator {
    CONFIG += SET_SPECIFIC_QML
    include(symbian.pri)
}

!SET_SPECIFIC_QML {
    include(desktop.pri)
}

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

isEmpty(DRIVERSDIR): {
    win32 {
        DRIVERSDIR=$$PREFIX/drivers
    } else {
        DRIVERSDIR=$$PREFIX/share/musson/drivers
    }
}

DEFINES += DRIVERS_DIR=\\\"$$DRIVERSDIR\\\"
message("Drivers will be seek at $$DRIVERSDIR")

include(../../../core/src/src.pri)
