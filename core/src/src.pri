#include driver interface
include(../../drivers/common/driver.pri)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    datatypes/account.h \
    utils/coresettings.h \
    utils/qrefreshlist.h \
    manager/servicemanager.h \
    utils/mssnqmlplugin.h

HEADER_FILTERS = \
    $$PWD/filter/servicefilter.h \
    $$PWD/filter/filtermanager.h \
    $$PWD/filter/filter.h \
    $$PWD/filter/filterfriendonline.h \
    $$PWD/filter/filterfriendgender.h \
    $$PWD/filter/filtermessagereaded.h \
    $$PWD/filter/filtermessagetype.h \
    $$PWD/filter/filtermodel.h

HEADER_DATATYPES = \
    $$PWD/datatypes/photo.h \
    $$PWD/datatypes/friend.h \
    $$PWD/datatypes/album.h \
    $$PWD/datatypes/photocomments.h \
    $$PWD/datatypes/errormessage.h \
    $$PWD/datatypes/message.h \
    $$PWD/datatypes/qeventfeed.h \
    $$PWD/datatypes/audio.h \
    $$PWD/datatypes/video.h \
    $$PWD/datatypes/qattachment.h \
    $$PWD/datatypes/qstatinfo.h \
    $$PWD/datatypes/place.h \
    $$PWD/datatypes/checkin.h \
    $$PWD/datatypes/baseobject.h \
    $$PWD/datatypes/baseobjectprivate.h \
    $$PWD/datatypes/event.h


SOURCES += \
    utils/coresettings.cpp \
    manager/servicemanager.cpp \
    utils/mssnqmlplugin.cpp \
    $$PWD/datatypes/photo.cpp \
    $$PWD/datatypes/account.cpp \
    $$PWD/datatypes/friend.cpp \
    $$PWD/datatypes/album.cpp \
    $$PWD/datatypes/message.cpp \
    $$PWD/datatypes/photocomments.cpp \
    $$PWD/datatypes/qeventfeed.cpp \
    $$PWD/datatypes/qattachment.cpp \
    $$PWD/datatypes/audio.cpp \
    $$PWD/datatypes/video.cpp \
    $$PWD/datatypes/qstatinfo.cpp \
    $$PWD/datatypes/place.cpp \
    $$PWD/datatypes/checkin.cpp \
    $$PWD/datatypes/errormessage.cpp \
    $$PWD/datatypes/baseobject.cpp \
    $$PWD/datatypes/event.cpp \
    $$PWD/filter/servicefilter.cpp \
    $$PWD/filter/filtermanager.cpp \
    $$PWD/filter/filter.cpp \
    $$PWD/filter/filterfriendonline.cpp \
    $$PWD/filter/filterfriendgender.cpp \
    $$PWD/filter/filtermessagereaded.cpp \
    $$PWD/filter/filtermessagetype.cpp \
    $$PWD/filter/filtermodel.cpp

# components
SOURCES += \
    manager/networkmanager.cpp \
    manager/drivermanager.cpp \
    manager/accountmanager.cpp \
    widgets/profilebrowser.cpp \
    widgets/messagecomposer.cpp \
    datatypes/profileitem.cpp \
    datatypes/messageitem.cpp \
    datatypes/listmodel.cpp \
    datatypes/iconitem.cpp \
    datatypes/driveritem.cpp \
    datatypes/accountitem.cpp \
    datatypes/albumitem.cpp \
    datatypes/photoitem.cpp \
    widgets/albumsmanager.cpp \
    widgets/photosmanager.cpp \
    widgets/imageviewer.cpp \
    datatypes/commentitem.cpp \
    manager/programsettings.cpp \
    manager/mfiltersmanager.cpp \
    datatypes/mstringlistmodel.cpp \
    manager/notificationmanager.cpp \
    datatypes/statitem.cpp \
    datatypes/folderlistmodel.cpp \
    widgets/messagesmanager.cpp \
    widgets/profilesmanager.cpp \
    widgets/statisticsmanager.cpp \
    manager/feedmanager.cpp \
    datatypes/feeditem.cpp \
    datatypes/checkinitem.cpp \
    datatypes/placeitem.cpp \
    widgets/geomanager.cpp \
    widgets/eventmanager.cpp \
    datatypes/eventitem.cpp \
    datatypes/audioitem.cpp \
    widgets/audiomanager.cpp

HEADER_MANAGER = manager/networkmanager.h \
    manager/drivermanager.h \
    manager/accountmanager.h  \
    manager/programsettings.h \
    manager/mfiltersmanager.h \
    manager/notificationmanager.h \
    manager/feedmanager.h

HEADER_WIDGETS = widgets/profilesmanager.h \
    widgets/profilebrowser.h \
    widgets/messagesmanager.h \
    widgets/messagecomposer.h \
    widgets/albumsmanager.h \
    widgets/photosmanager.h \
    widgets/imageviewer.h \
    widgets/statisticsmanager.h \
    widgets/geomanager.h \
    widgets/eventmanager.h \
    widgets/audiomanager.h

HEADER_TYPES = datatypes/profileitem.h \
    datatypes/messageitem.h \
    datatypes/listmodel.h \
    datatypes/iconitem.h \
    datatypes/driveritem.h \
    datatypes/accountitem.h \
    datatypes/albumitem.h \
    datatypes/photoitem.h \
    datatypes/commentitem.h  \
    datatypes/mstringlistmodel.h \
    datatypes/statitem.h \
    datatypes/folderlistmodel.h \
    datatypes/feeditem.h \
    datatypes/checkinitem.h \
    datatypes/placeitem.h \
    datatypes/eventitem.h \
    datatypes/audioitem.h

HEADERS += \
    $$HEADER_MANAGER \
    $$HEADER_WIDGETS \
    $$HEADER_TYPES \
    $$HEADER_FILTERS \
    $$HEADER_DATATYPES

include(qxmlrpc/qxmlrpc.pri)