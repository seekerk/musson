# include private developer settings
exists(../devel.pri):include(../devel.pri)

include(../common/common.pri)

VERSION = 0.0.1

TARGET = mssnvk

HEADERS += vkdriver.h

SOURCES += vkdriver.cpp
