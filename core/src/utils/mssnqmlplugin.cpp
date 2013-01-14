#include "mssnqmlplugin.h"
#include <manager/servicemanager.h>
#include <manager/networkmanager.h>
#include <manager/accountmanager.h>
#include <manager/drivermanager.h>
#include <manager/programsettings.h>
#include <manager/mfiltersmanager.h>
#include <manager/notificationmanager.h>
#include <manager/feedmanager.h>
#include <filter/filtermodel.h>

#include <widgets/profilesmanager.h>
#include <widgets/profilebrowser.h>

#include <widgets/messagecomposer.h>
#include <widgets/messagesmanager.h>

#include <widgets/albumsmanager.h>
#include <widgets/photosmanager.h>
#include <widgets/imageviewer.h>
#include <widgets/geomanager.h>
#include <widgets/eventmanager.h>
#include <widgets/audiomanager.h>

#include <widgets/statisticsmanager.h>
#include <datatypes/folderlistmodel.h>

#include <datatypes/listmodel.h>
#include <datatypes/iconitem.h>
#include <datatypes/eventitem.h>
#include <datatypes/audioitem.h>

#include <QtDeclarative/qdeclarative.h>

void MssnQmlPlugin::registerTypes(const char *uri){

    Q_UNUSED(uri)
    qmlRegisterType<ServiceManager>("MussonCore", 1, 2, "ServiceManager");
    qmlRegisterType<NetworkManager>("MussonCore", 1, 2, "NetworkManager");
    qmlRegisterType<AccountManager>("MussonCore", 1, 2, "AccountManager");
    qmlRegisterType<DriverManager>("MussonCore", 1, 2, "DriverManager");
    qmlRegisterType<ProgramSettings>("MussonCore", 1, 2, "ProgramManager");
    qmlRegisterType<MFiltersManager>("MussonCore", 1, 2, "FilterManager");
    qmlRegisterType<NotificationManager>("MussonCore", 1, 2, "NotificationManager");
    qmlRegisterType<StatisticsManager>("MussonCore", 1, 2, "StatManager");

    qmlRegisterType<ProfilesManager>("MussonCore", 1, 2, "ProfileManager");
    qmlRegisterType<ProfileBrowser>("MussonCore", 1, 2, "ProfileBrowser");
    qmlRegisterType<MessagesManager>("MussonCore", 1, 2, "MessageManager");
    qmlRegisterType<MessageComposer>("MussonCore", 1, 2, "MessageComposer");
    qmlRegisterType<AlbumsManager>("MussonCore", 1, 2, "AlbumsManager");
    qmlRegisterType<PhotosManager>("MussonCore", 1, 2, "PhotosManager");
    qmlRegisterType<ImageViewer>("MussonCore", 1, 2, "ImageViewer");
    qmlRegisterType<FeedManager>("MussonCore", 1, 2, "FeedManager");
    qmlRegisterType<GeoManager>("MussonCore", 1, 2, "GeoManager");
    qmlRegisterType<EventManager>("MussonCore", 1, 2, "EventManager");

    qmlRegisterType<IconItem>("MussonCore", 1, 2, "Icon");
    qmlRegisterType<DriverInfo>("MussonCore", 1, 2, "DriverInfo");
    qmlRegisterType<ListModel>("MussonCore", 1, 2, "MListModel");
    qmlRegisterType<MStringListModel>("MussonCore", 1, 2, "MStringListModel");
    qmlRegisterType<QDeclarativeFolderListModel>("MussonCore", 1, 2, "MFolderListModel");
    qmlRegisterType<FilterModel>("MussonCore", 1, 2, "FilterModel");
    qmlRegisterType<AudioManager>("MussonCore", 1, 2, "AudioManager");
    qmlRegisterType<AudioItem>("MussonCore", 1, 2, "AudioItem");
}

Q_EXPORT_PLUGIN2(musson, MssnQmlPlugin)
