# include private developer settings
exists(../../devel.pri):include(../../devel.pri)
exists(../devel.pri):include(../devel.pri)

TEMPLATE = lib

VERSION = 1.2.0

TARGET_NAME = musson
unix::TARGET = $${TARGET_NAME}
else:TARGET = $$qtLibraryTarget($$TARGET_NAME)

QT += gui core network xml declarative

config += thread

contains(DEFINES, ENABLE_DEBUG)|!isEmpty(ENABLE_DEBUG) {
    message(Enabled debug)
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

# specify paths
!symbian:DEFINES += LOCALDATADIR=\\\"/.musson/\\\"
isEmpty(PREFIX):PREFIX="/usr" # $$[QT_INSTALL_PREFIX]
message(prefix is $$PREFIX)
DATAPREFIX = $$PREFIX/share
DEFINES += DATAPREFIX=\\\"$$DATAPREFIX\\\"
message(dataprefix is $$DATAPREFIX)
LIBDIR = $$PREFIX/lib
!maemo5:!contains(MEEGO_EDITION,harmattan):contains(QMAKE_HOST.arch, x86_64)::LIBDIR = $$PREFIX/lib64
INCLUDEDIR = $$PREFIX/include
DATADIR = $$PREFIX/share

isEmpty(DRIVERSDIR): {
    win32 {
	DRIVERSDIR=$$PREFIX/drivers
    } else {
	DRIVERSDIR=$$PREFIX/share/musson/drivers
    }
}

DEFINES += DRIVERS_DIR=\\\"$$DRIVERSDIR\\\"
message("Drivers will be seek at $$DRIVERSDIR")

# sources
include(src.pri)

#installs
target.path = $$LIBDIR
INSTALLS += target

unix:!symbian {
    system("sed -e 's:@prefix@:$$PREFIX:g;s:@libdir@:$$LIBDIR:g;s:@includedir@:$$INCLUDEDIR/musson:g;s:@VERSION@:$$VERSION:g' ../data/musson-core.pc.in >../data/musson-core.pc")
    pkgconfig.path = $$LIBDIR/pkgconfig
    pkgconfig.files = ../data/*.pc
    INSTALLS += pkgconfig
}

# import paths
importpath.path = $$DATADIR/musson
importpath.files = ../data/MussonCo*
INSTALLS += importpath
