#include "servicemanager.h"

#include <cassert>

#include "utils/datacache.h"

#include "filter/servicefilter.h"
#include "filter/filtermanager.h"

#include "filter/filterfriendonline.h"
#include "filter/filterfriendgender.h"
#include "filter/filtermessagereaded.h"
#include "filter/filtermessagetype.h"

#include "datatypes/audio.h"
#include "datatypes/video.h"

#include <QApplication>
#include <QTimer>

#include <QPluginLoader>

#ifdef TRY_FUTURE
#define runInThread MussonConcurrent::run
#else
#define runInThread QtConcurrent::run
#endif

#if defined(Q_OS_SYMBIAN)
#    define LIB_SUFFIX "qtplugin"
#elif defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
#    define LIB_SUFFIX "dll"
#else
#    define LIB_SUFFIX "so"
#endif

ServiceMgr::ServiceMgr(QObject *parent) :
    QObject(parent), canUpdateFriends(false), canUpdateAlbums(false), canUpdateMessages(false), canUpdateFeeds(false),
    friendsUpdate(0), albumsUpdate(0), photosUpdate(0),
    audioUpdate(0), videoUpdate (0), feedsUpdate(0), messagesUpdate(0),
    serviceFilter_(0), filterManager_(0)
{
    this->isSkipFriendListUpdate = false;
#ifdef TIME_DEBUG
    this->timer.start();
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif

    this->loadWebauth();

    settings = new CoreSettings(this);

    loadAccounts();

#ifdef DEBUG
    qDebug()  << __PRETTY_FUNCTION__ << ":" << __LINE__ << "loaded " << this->accounts.length() << " accounts";
#endif

    // update network settings if settings changed
    connect(this, SIGNAL(settingsChanged()), this, SLOT(updateDriverSettings()));
    updateDriverSettings();

    qRegisterMetaType<FriendList>("FriendList");
    qRegisterMetaType<AlbumList>("AlbumList");
    qRegisterMetaType<PhotoList>("PhotoList");
    qRegisterMetaType<MessageList>("MessageList");
    qRegisterMetaType<AudioList>("AudioList");
    qRegisterMetaType<VideoList>("VideoList");
    qRegisterMetaType<Friend>("Friend");
    qRegisterMetaType<ErrorMessage>("ErrorMessage");
    qRegisterMetaType<PhotoCommentList>("PhotoCommentList");
    qRegisterMetaType<PlaceList>("PlaceList");
    qRegisterMetaType<CheckinList>("CheckinList");
    qRegisterMetaType<Driver::Action>("Driver::Action");
    qRegisterMetaType<QEventFeedList>("QEventFeedList");
    qRegisterMetaType<QEventFeed::FeedType>("QEventFeed::FeedType");
    qRegisterMetaType<QEventFeed>("QEventFeed");
    qRegisterMetaType<QAttachment>("QAttachment");
    qRegisterMetaType<QVariant>("QVariant");
    qRegisterMetaType<EventList>("EventList");

    for(int i = 0; i < this->accounts.length(); i++) {
        this->connectToTransport(this->accounts.at(i));
        // load profiles for each account if account is enabled
        if (!this->accounts.at(i)->isDisabled())
            this->accounts[i]->getProfile(false);
    }

    connect(this, SIGNAL(updateAccounts(QString,AccountList)), this, SLOT(storeAccounts(QString,AccountList)));

    createFilter();

    dataCache_ = new DataCache(this, this);
    dataCache_->setAccountList(QString(), accounts);
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::connectToTransport(Account *account)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    connect(account, SIGNAL(friendsReceived(QString, FriendList, bool)),
            SLOT(gotFriends(QString, FriendList, bool)));
    connect(account, SIGNAL(messagesReceived(QString,MessageList, bool)),
            SLOT(gotMessageList(QString,MessageList,bool)));
    connect(account, SIGNAL(albumsReceived(QString, QString, AlbumList, bool)),
            SLOT(gotAlbumList(QString, QString, AlbumList, bool)));
    connect(account, SIGNAL(photosReceived(QString, QString, QString, PhotoList, bool)),
            SLOT(gotPhotoList(QString, QString, QString, PhotoList, bool)));
    connect(account, SIGNAL(audioReceived(QString, QString, AudioList, bool)),
            SLOT(gotAudioList(QString, QString, AudioList, bool)));
    connect(account, SIGNAL(videoReceived(QString, QString, VideoList, bool)),
            SLOT(gotVideoList(QString, QString, VideoList, bool)));
    connect(account, SIGNAL(profileReceived(QString,QString,Friend)),
            SLOT(gotProfile(QString,QString,Friend)));
    connect(account, SIGNAL(errorOccurred(QString,ErrorMessage,Driver::Action)),
            SLOT(gotErrorMsg(QString,ErrorMessage,Driver::Action)));
    connect(account, SIGNAL(commentsReceived(QString,QString,QString,QString,PhotoCommentList)),
            SLOT(gotComments(QString,QString,QString,QString,PhotoCommentList)));
    connect(account, SIGNAL(photoUploaded(QString,QString,QString,QString)),
            SIGNAL(photoUploaded(QString,QString,QString,QString)));
    connect(account, SIGNAL(eventFeedReceived(QString,QEventFeedList,QEventFeed::FeedType,bool)),
            SLOT(gotEventFeed(QString,QEventFeedList,QEventFeed::FeedType,bool)));
    connect(account, SIGNAL(messageReaded(QString,QString)), this, SLOT(gotMessageReaded(QString,QString)));
    connect(account, SIGNAL(messageDeleted(QString,QString)), this, SLOT(gotMessageDeleted(QString,QString)));
    connect(account, SIGNAL(messageSended(QString,QString,QString,QString)),
            SLOT(gotMessageSent(QString,QString,QString,QString)));
    connect(account, SIGNAL(statsUpdated(QString)), SIGNAL(statsUpdated()));
    connect(account, SIGNAL(placesReceived(PlaceList)),
            SLOT(gotPlaces(PlaceList)));
    connect(account, SIGNAL(checkinsReceived(QString,CheckinList, int)),
            SLOT(gotCheckins(QString,CheckinList, int)));
    connect(account, SIGNAL(albumCreated(QString,QString)), this, SLOT(gotAlbumCreated(QString,QString)));
    connect(account, SIGNAL(eventsReceived(QString,QString,EventList,bool)), SLOT(gotEventsList(QString,QString,EventList,bool)));

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

ServiceMgr::ServiceMgr(const ServiceMgr &src) : QObject(src.parent())
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << src.timer.elapsed() << "msec";
#endif
    this->isSkipFriendListUpdate = src.isSkipFriendListUpdate;
    this->accounts = src.accounts;
    this->threads = src.threads;
    this->settings = src.settings;
    this->friendsUpdate = src.friendsUpdate;
    this->albumsUpdate = src.albumsUpdate;
    this->photosUpdate = src.photosUpdate;
    this->audioUpdate = src.audioUpdate;
    this->videoUpdate = src.videoUpdate;
    this->feedsUpdate = src.feedsUpdate;
    this->canUpdateAlbums = src.canUpdateAlbums;
    this->canUpdateFeeds = src.canUpdateFeeds;
    this->canUpdateFriends = src.canUpdateFriends;
    this->canUpdateMessages = src.canUpdateMessages;
    this->eventsUpdate = src.eventsUpdate;
    this->canUpdateEvents = src.canUpdateEvents;

#ifdef TIME_DEBUG
    this->timer = src.timer;
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << src.timer.elapsed() << "msec";
#endif
}

ServiceMgr::~ServiceMgr()
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    for (int i = 0; i < this->accounts.length(); i++)
        delete this->accounts.at(i);

    this->accounts.clear();

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::storeAccounts(QString accountId, AccountList)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    QDomDocument out("MyDoc");
    QDomElement profile = out.createElement(NODE_ACCOUNTS_ROOT);
    out.appendChild(profile);
    for (int i = 0; i < this->accounts.length(); i++) {
        Account *curAcc = this->accounts[i];
        if (curAcc->accountId().compare(accountId) == 0)
        {
            curAcc->saveAccount();
//            this->getFeed(curAcc, QEventFeed::photoFeed, true, true);
        }
        QDomElement anode = out.createElement(NODE_ACCOUNT);
        QDomText t = out.createTextNode(curAcc->accountId());
        anode.appendChild(t);
        anode.setAttribute("disabled", (curAcc->isDisabled() ? "true" : "false"));
        profile.appendChild(anode);
    }

    QFile file(CoreSettings::getHomeDir() + QString(FILE_ACCOUNTS_LIST));
#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << file.fileName();
#endif
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
#ifdef DEBUG
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "can't open file: " + file.errorString();
#endif
    }
    file.write(out.toByteArray());
    file.close();
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::loadAccounts()
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif

    QStringList accs;

    // step 1. load account list from file if it exists.
    QDomDocument doc;
    QFile file(CoreSettings::getHomeDir() + QString(FILE_ACCOUNTS_LIST));
    if (file.open(QIODevice::ReadOnly) != false) {
        doc.setContent(&file);
        file.close();

        QDomElement doc_elem = doc.documentElement();
        QDomNodeList lst = doc_elem.elementsByTagName(NODE_ACCOUNT_ROOT);

    #ifdef DEBUG
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "accounts:" << lst.count();
    #endif
        for (int i = 0; i < lst.count(); i++) {
            QString accId = lst.at(i).firstChild().nodeValue();
            accs.append(accId);
            bool isDisabled = lst.at(i).toElement().attribute("disabled", "false").compare("true") == 0;
    #ifdef DEBUG
            qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Load account:" << accId << "disabled=" << (isDisabled ? "true" : "false");
    #endif
            Account *prf = Account::loadAccount(accId, isDisabled);

            if (prf != NULL)
            {
                mutex.lock();
                this->accounts.append(prf);
                mutex.unlock();
            }
        }
    }

    // step 2. load other accounts from account dir
    QDir dir;
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
    dir.setPath(CoreSettings::getAccountDir(QString::null));
    QFileInfoList lst = dir.entryInfoList();

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Found " << lst.length() << " entries at " << dir.path();

    for (int i = 0; i < lst.length(); i++) {
        QFileInfo dir = lst.at(i);
        if (dir.isDir() && dir.fileName() != "." && dir.fileName() != "..") {
            if (!accs.contains(dir.fileName()))
            {
                accs.append(dir.fileName());
#ifdef DEBUG
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Load account:" << dir.fileName();
#endif
                Account *prf = Account::loadAccount(dir.fileName(), true);

                if (prf != NULL)
                {
                    mutex.lock();
                    this->accounts.append(prf);
                    mutex.unlock();
                }

            }
        }
    }

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::cleanThreads()
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker)
    QList<QString> keys;
    QHashIterator<QString, MSSFuture<void> > i(this->threads);
    while (i.hasNext()) {
        i.next();
        if (i.value().isFinished())
            keys.append(i.key());
    }

    for(int i = 0; i < keys.length(); i++)
        this->threads.remove(keys[i]);
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

QList<QString> ServiceMgr::activeRequests()
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    QList <QString> ret;

    QMutexLocker locker(&mutex);
    Q_UNUSED(locker)
    QHashIterator<QString, MSSFuture<void> > i(this->threads);
    while (i.hasNext()) {
        i.next();
        if (!i.value().isFinished())
            ret.append(i.key());
    }

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return ret;
}

void ServiceMgr::updateSettings()
{
    emit settingsChanged();
}

void ServiceMgr::audioFinish()
{
    emit audioStop();
}

void ServiceMgr::updateDriverSettings()
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    QString curProxyHost = QString("");
    int curProxyPort = 0;

    ProxyType proxy = (ProxyType)settings->value(SETTINGS_PROXY).toInt();

    switch (proxy) {
    case ServiceMgr::NoProxy:
        QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::NoProxy));
        break;
    case ServiceMgr::UserProxy:
        curProxyHost = settings->value(SETTINGS_PROXYHOST).toString();
        curProxyPort = settings->value(SETTINGS_PROXYPORT).toInt();
        if (!curProxyHost.isEmpty() && curProxyPort != 0)
            QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::HttpProxy,curProxyHost,curProxyPort));
        break;
    case ServiceMgr::SystemProxy: {
        QNetworkProxyFactory::setUseSystemConfiguration(true);
        QNetworkProxyQuery npq(QUrl(QLatin1String("http://maemo.org")));
        QList<QNetworkProxy> proxyList = QNetworkProxyFactory::systemProxyForQuery(npq);
        if (proxyList.length() > 0 && proxyList.at(0) != QNetworkProxy::NoProxy) {
            curProxyHost = proxyList.at(0).hostName();
            curProxyPort = proxyList.at(0).port();
        } else {
            QString var(getenv("http_proxy"));
            if (var.isNull())
                var = getenv("all_proxy");

            QRegExp regex("(http://)?(.*):(\\d*)/?");
            int pos = regex.indexIn(var);

            if(pos > -1) {
                curProxyHost = regex.cap(2);
                curProxyPort = regex.cap(3).toInt();
            }
        }
        if (curProxyHost.isEmpty())
            curProxyHost = QString::null;
        if (!curProxyHost.isEmpty() && curProxyPort != 0)
            QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::HttpProxy,curProxyHost,curProxyPort));
        break;
    }
    default:
        qWarning() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Unknown type of proxy";
    }

#ifdef DEBUG
    if (curProxyHost.isNull())
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "using system proxy";
    else
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "using proxy: \""<< curProxyHost << "\":" << curProxyPort;
#endif

    for (int i = 0; i < this->accounts.length(); i++)
        this->accounts.at(i)->setProxy(curProxyHost, curProxyPort);
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

FriendList ServiceMgr::getFriends(bool isNeedUpdate, bool useSignal)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() isNeedUpdate=" << (isNeedUpdate ? "true" : "false");
#endif

    bool startUpdate = isNeedUpdate;

    // checks that all friend lists was downloaded
    if (!startUpdate)
    {
        for (int i = 0; i < this->accounts.length(); i++)
        {
            if (this->accounts.at(i)->isDisabled()) continue;
            if (this->accounts.at(i)->isNeedUpdateFriends())
            {
                startUpdate = true;
                break;
            }
        }
    }

#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() startUpdate=" << (startUpdate ? "true" : "false");
#endif

    if (startUpdate)
    {
        // start update cache thread
        this->cleanThreads();
        if (!this->threads.contains("updateCacheFriends"))
        {
            MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheFriends, (isNeedUpdate ? UpdateType::MUST_UPDATE : UpdateType::CAN_UPDATE));
            this->threads.insert("updateCacheFriends", status);
        } else {
            qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() cannot run thread!!!";
        }
    }

    if (useSignal && !startUpdate)
        emit this->updateFriends(this->cachedFriendList, this->friendsUpdate > 0 ? false: true);

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return this->cachedFriendList;
}

void ServiceMgr::createCacheFriends(UpdateType::UpdateType isUpdate)
{
#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() start thread; isUpdate=" << (isUpdate == UpdateType::NOT_UPDATE ? "NOT_UPDATE" : (isUpdate == UpdateType::CAN_UPDATE ? "CAN_UPDATE" : "MUST_UPDATE"));
#endif
    mutexFriends.lock();
    this->cachedFriendList.clear();
    mutexFriends.unlock();
    this->cleanThreads();

    for (int i = 0; i < this->accounts.length(); i++)
    {
        if (this->accounts.at(i)->isDisabled()) continue;

        Account* acc = accounts.at(i);
        FriendList curList = acc->getFriendList();

        if (curList.timeCreate().isValid())
            acc->setFriendListCached();
#ifdef DEBUG
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "before merging" << this->cachedFriendList.length() << curList.length();
#endif
        mutexFriends.lock();
        this->cachedFriendList = Friend::mergeLists(this->cachedFriendList, curList);
        mutexFriends.unlock();
#ifdef DEBUG
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "after merging" << this->cachedFriendList.length();
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "curList.isEmpty()=" << (curList.isEmpty() ? "true" : "false") <<
                    "curList.timeCreate().isValid()=" << (curList.timeCreate().isValid() ? "true" : "false");
#endif

        if (isUpdate != UpdateType::NOT_UPDATE  &&
                (isUpdate == UpdateType::MUST_UPDATE ||
                (curList.isEmpty() && !curList.timeCreate().isValid()))) {
            QString threadId = QString("getFriends") + this->accounts[i]->accountId();

            // check that thread shouldn't already running
            if (!this->threads.contains(threadId) && this->accounts.at(i)->isNetworkEnabled()) {
                // add thread to list
                MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getFriends, true);
                this->threads.insert(threadId, status);
                mutexFriends.lock();
                this->friendsUpdate++;
                mutexFriends.unlock();
            }
        }
    }
    mutexFriends.lock();
    qSort(this->cachedFriendList.begin(), this->cachedFriendList.end());
    mutexFriends.unlock();

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() stop. List count=" << this->cachedFriendList.length() << "friendsUpdate=" << this->friendsUpdate;
    emit this->updateFriends(this->cachedFriendList, this->friendsUpdate > 0 ? false: true);
}

void ServiceMgr::gotFriends(QString accountid, FriendList list, bool isLastUpdate)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif

    // skip update if it is not last
    if (!isLastUpdate && this->isSkipFriendListUpdate)
        return;

    // cache Friend list
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (this->accounts[i]->accountId() == accountid) {
            this->accounts[i]->setFriendList(list);
        }
    }

    if (isLastUpdate)
    {
        mutexFriends.lock();
        this->friendsUpdate--;
        mutexFriends.unlock();
    }
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "islastUpdate=" << isLastUpdate << ", friendsUpdate=" << friendsUpdate;

    // start update cache thread
    this->cleanThreads();
    if (!this->threads.contains("updateCacheFriends"))
    {
        MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheFriends, UpdateType::NOT_UPDATE);
        this->threads.insert("updateCacheFriends", status);
    }

    if (!isLastUpdate)
    {
        mutexFriends.lock();
        this->isSkipFriendListUpdate = true;
        mutexFriends.unlock();
        QTimer::singleShot(1000, this, SLOT(disableSkipFriendListUpdate()));
    }

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::disableSkipFriendListUpdate()
{
    mutexFriends.lock();
    this->isSkipFriendListUpdate = false;
    mutexFriends.unlock();
}

AlbumList ServiceMgr::getAlbums(bool isNeedUpdate, bool useSignal)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    AlbumList ret;

    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        ret.append(this->getAlbums(this->accounts.at(i)->getProfile(false), isNeedUpdate, false));
    }

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): this->albumsUpdate=" << this->albumsUpdate;

    if (useSignal)
    {
        if (this->accounts.length() > 0)
            emit this->updateAlbumList(this->accounts.at(0)->getProfile(false), ret, this->albumsUpdate > 0 ? false: true);
        else
            emit this->updateAlbumList(Friend(), ret, this->albumsUpdate > 0 ? false: true);
    }

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return ret;
}

AlbumList ServiceMgr::getAlbums(const Friend& curFriend, bool isNeedUpdate, const bool useSignal)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    AlbumList ret;

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): Load album list from cache for " << curFriend.ownerId()
            << " from " << curFriend.accountId() << " needUpdate=" << isNeedUpdate << "; this->albumsUpdate="
            << this->albumsUpdate;

    ret = curFriend.getAlbumList();

    this->cleanThreads();

    if (isNeedUpdate || ret.isEmpty()) {
        for (int i = 0; i < this->accounts.length(); i++) {
            if (this->accounts.at(i)->isDisabled()) continue;
            if ((curFriend.accountId() == this->accounts[i]->accountId()) &&
                this->accounts.at(i)->isNetworkEnabled()) {
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Download album list for " << curFriend.ownerId();

                if (this->threads.contains(QString("albums") + curFriend.ownerId()))
                    break;

                MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getAlbums, curFriend.ownerId(), true);
                this->threads.insert(QString("albums") + curFriend.ownerId(), status);
                this->albumsUpdate++;
                break;
            }
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): this->albumsUpdate=" << this->albumsUpdate;

    if (useSignal)
        emit this->updateAlbumList(curFriend, ret, this->albumsUpdate > 0 ? false: true);

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return ret;
}

AlbumList ServiceMgr::getAlbums(const Album& al, const bool isNeedUpdate, const bool useSignal)
{
    return this->getAlbums(this->getProfile(al.accountId(), al.ownerId(), false, false, false), isNeedUpdate, useSignal);
}

void ServiceMgr::gotAlbumList(QString accountId, QString owner, AlbumList list, bool isLastUpdate)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): accountId=" << accountId << "isLastUpdate=" << isLastUpdate;

    AlbumList ret;
    ret.append(list);

    // store album list to cache
    Friend::setAlbumList(list, accountId, owner);

    // check that it is not a my album list
    for (int i = 0; i < this->accounts.length(); i++)
    {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (this->accounts.at(i)->accountId() == accountId &&
            this->accounts.at(i)->getProfile(false).ownerId() == owner)
        {
            // if it is my album list then store, generate new full list and send it
            this->accounts[i]->setAlbumList(list);
            for (int j = 0; j < this->accounts.length(); j++)
            {
                if (this->accounts.at(j)->isDisabled()) continue;
                if (j != i)
                    ret.append(this->accounts[j]->getAlbumList());
            }

            break;
        }
    }

    if (isLastUpdate)
    {
        this->albumsUpdate--;
        if (this->albumsUpdate < 0)
            this->albumsUpdate = 0;
    }
    qDebug() << "islastUpdate=" << isLastUpdate << ", albumsUpdate=" << albumsUpdate << ", accountId=" << accountId << "owner=" << owner;

    emit updateAlbumList(this->getProfile(accountId, owner, false, false, false), ret, this->albumsUpdate > 0 ? false: true);
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

bool ServiceMgr::createAlbum(QString accountId, QString name, QString description, QString privacy)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (accountId == this->accounts.at(i)->accountId())
            return this->accounts.at(i)->createAlbum(name, description, privacy);
    }

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return false;
}

PhotoList ServiceMgr::getPhotos(const Album& album, const bool isNeedUpdate, const bool useSignal, const bool loadIcons)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    bool nUpdate = isNeedUpdate;
    PhotoList ret;

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Load photo list from cache for album " << album.albumId();

    ret = album.getPhotoList();
    this->cleanThreads();

    // if size of album not equal size of photo list then update oldest lists.
    if (album.size() + ret.length() > 0 && album.size() != ret.length())
    {
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() UNEQUAL!!! Album suze=" << album.size() << " photos=" << ret.length();
        if (!ret.timeCreate().isValid() || QDateTime::currentDateTime().secsTo(ret.timeCreate()) > 60)
            nUpdate = true;

        AlbumList lst = this->getAlbums(album, false, false);
        if (!lst.timeCreate().isValid() || QDateTime::currentDateTime().secsTo(lst.timeCreate()) > 60)
            this->getAlbums(album, true, false);

        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Update time: album=" << lst.timeCreate() << " photos=" << ret.timeCreate();
    }

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "nUpdate=" << nUpdate << " ret.length()=" << ret.length();
    if (nUpdate || ret.isEmpty()) {
        for (int i = 0; i < this->accounts.length(); i++) {
            if (this->accounts.at(i)->isDisabled()) continue;
            if (album.accountId().compare(this->accounts[i]->accountId()) == 0 &&
                this->accounts.at(i)->isNetworkEnabled()) {
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Download photo list for " << album.albumId();
                if (!this->threads.contains(Photo::getKey(album)))
                {
                    MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getPhotos, album.ownerId(), album.albumId(), loadIcons);
                    this->threads.insert(Photo::getKey(album), status);
                    this->photosUpdate++;
                    break;
                }
            }
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() Loaded " << ret.length() << " photos for album " << album.albumId();

    filterManager_->filterList(ret);

    if (useSignal)
    {
        emit this->updatePhotoList(album.accountId(), album.ownerId(), album.albumId(), ret, this->photosUpdate > 0? false: true);
    }

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return ret;
}

PhotoList ServiceMgr::getPhotos(const Photo &curPhoto, const bool isNeedUpdate, const bool useSignal, const bool loadIcons)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    PhotoList ret;

#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Load photo list from cache for " << curPhoto.albumId();
#endif

    ret = Photo::loadPhotoList(curPhoto.serviceId(), curPhoto.accountId(), curPhoto.ownerId(), curPhoto.albumId());
    this->cleanThreads();

    if (isNeedUpdate || ret.isEmpty()) {
        for (int i = 0; i < this->accounts.length(); i++) {
            if (this->accounts.at(i)->isDisabled()) continue;
            if (curPhoto.accountId() == this->accounts[i]->accountId() &&
                this->accounts[i]->isNetworkEnabled()) {
#ifdef DEBUG
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Download photo list for " << curPhoto.albumId();
#endif
                if (this->threads.contains(Photo::getKey(curPhoto)) || !this->accounts.at(i)->isNetworkEnabled()())
                    return ret;
                MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getPhotos, curPhoto.ownerId(), curPhoto.albumId(), loadIcons);
                this->threads.insert(Photo::getKey(curPhoto), status);
                this->photosUpdate++;
            }
        }
    }

    filterManager_->filterList(ret);

    if (useSignal)
    {
        emit this->updatePhotoList(curPhoto.accountId(), curPhoto.ownerId(), curPhoto.albumId(), ret, this->photosUpdate > 0? false: true);
    }

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return ret;
}

void ServiceMgr::gotPhotoList(QString accountId, QString friendId, QString albumId, PhotoList list, bool isLastUpdate)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    Album::setPhotoList(list, accountId, friendId, albumId);

    if (isLastUpdate)
        this->photosUpdate--;
    if (this->photosUpdate < 0)
        this->photosUpdate = 0;
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ <<"() islastUpdate=" << isLastUpdate << ", photosUpdate=" << photosUpdate;

    filterManager_->filterList(list);

    emit updatePhotoList(accountId, friendId, albumId, list, photosUpdate > 0 ? false : true);
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

AudioList ServiceMgr::getAudio(bool isNeedUpdate, bool useSignal)
{
    AudioList ret;

    for (int i = 0; i < this->accounts.length(); i++) {
        ret.append(this->getAudio(this->accounts.at(i)->getProfile(false), isNeedUpdate, false));
    }

    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): this->audioUpdate=" << this->audioUpdate;

    filterManager_->filterList(ret);

    if (useSignal && this->accounts.length() > 0)
        emit this->updateAudioList(this->accounts.at(0)->getProfile(false), ret, this->audioUpdate > 0 ? false: true);

    return ret;
}

AudioList ServiceMgr::getAudio(const Friend& curFriend, bool isNeedUpdate, const bool useSignal)
{
    AudioList ret;

    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): Load audio list from cache for " << curFriend.ownerId()
            << " from " << curFriend.accountId() << " needUpdate=" << isNeedUpdate << "; this->audioUpdate="
            << this->audioUpdate;

    ret = curFriend.getAudioList();

    this->cleanThreads();

    if (isNeedUpdate || ret.isEmpty()) {
        for (int i = 0; i < this->accounts.length(); i++) {
            if (this->accounts.at(i)->isDisabled()) continue;
            if ((curFriend.accountId() == this->accounts[i]->accountId()) &&
                this->accounts.at(i)->isNetworkEnabled()) {
                qDebug() << "Download audio list for " << curFriend.ownerId();

                if (this->threads.contains(QString("audio") + curFriend.ownerId()))
                    break;

                MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getAudio, curFriend.ownerId());
                this->threads.insert(QString("audio") + curFriend.ownerId(), status);
                this->audioUpdate++;
                break;
            }
        }
    }

    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): this->audioUpdate=" << this->audioUpdate;

    filterManager_->filterList(ret);

    if (useSignal)
        emit this->updateAudioList(curFriend, ret, this->audioUpdate > 0 ? false: true);

    return ret;
}

AudioList ServiceMgr::getAudio(const Audio& aud, const bool isNeedUpdate, const bool useSignal)
{
    return this->getAudio(this->getProfile(aud.accountId(), aud.ownerId(), false, false, false), isNeedUpdate, useSignal);
}

void ServiceMgr::gotAudioList(QString accountId, QString friendId, AudioList list, bool isLastUpdate)
{

    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): isLastUpdate=" << isLastUpdate;

    AudioList ret;
    ret.append(list);

    // store audio list to cache
    Friend::setAudioList(list, accountId, friendId);

    // check that it is not a my audio list
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->accountId() == accountId &&
            this->accounts.at(i)->getProfile(false).ownerId() == friendId)
        {
            // if it is my audio list then store, generate new full list and send it
            this->accounts[i]->setAudioList(list);
            for (int j = 0; j < this->accounts.length(); j++)
            {
                if (j != i)
                    ret.append(this->accounts[j]->getAudioList());
            }

            break;
        }
    }

    if (isLastUpdate)
    {
        this->audioUpdate--;
        if (this->audioUpdate < 0)
            this->audioUpdate = 0;
    }
    qDebug() << "islastUpdate=" << isLastUpdate << ", audioUpdate=" << audioUpdate << ", accountId=" << accountId << "owner=" << friendId;

    filterManager_->filterList(ret);
    emit updateAudioList(this->getProfile(accountId, friendId, false, false, false), ret, this->audioUpdate > 0 ? false: true);
}

VideoList ServiceMgr::getVideo(bool isNeedUpdate, bool useSignal)
{
    Friend fd = this->getMyProfile(false);

    VideoList ret = this->getVideo(fd, isNeedUpdate, useSignal);

    if (fd.isMetaProfile())
    {
        FriendList fdl = fd.getProfileList();

        foreach (Friend curfd, fdl)
        {
            ret.append(this->getVideo(curfd, isNeedUpdate, useSignal));
        }
    }

    return ret;
}

VideoList ServiceMgr::getVideo(const Friend& curFriend, bool isNeedUpdate, const bool useSignal)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    VideoList ret;

#ifdef DEBUG
    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): Load video list from cache for " << curFriend.ownerId()
            << " from " << curFriend.accountId() << " needUpdate=" << isNeedUpdate << "; this->videoUpdate="
            << this->videoUpdate;
#endif

    ret = curFriend.getVideoList();

    this->cleanThreads();

    if (isNeedUpdate || ret.isEmpty()) {
        for (int i = 0; i < this->accounts.length(); i++) {
            if ((curFriend.accountId() == this->accounts.at(i)->accountId()) &&
                this->accounts.at(i)->isNetworkEnabled() &&
                this->accounts.at(i)->checkFunction(CLASS_VIDEO, GET_LIST_VIDEO)) {
                if (this->threads.contains(QString("video") + curFriend.ownerId()))
                    break;

                MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getVideo, curFriend.ownerId(), true);
                this->threads.insert(QString("video") + curFriend.ownerId(), status);
                this->videoUpdate++;
                break;
            }
        }
    }

#ifdef DEBUG
    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): this->videoUpdate=" << this->videoUpdate;
#endif

    if (useSignal)
        emit this->updateVideoList(curFriend, ret, this->videoUpdate > 0 ? false: true);

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return ret;
}

VideoList ServiceMgr::getVideo(const Video& vid, const bool isNeedUpdate, const bool useSignal)
{
    return this->getVideo(this->getProfile(vid.accountId(), vid.ownerId(), false, false, false), isNeedUpdate, useSignal);
}

void ServiceMgr::gotVideoList(QString accountId, QString friendId, VideoList list, bool isLastUpdate)
{

    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): isLastUpdate=" << isLastUpdate;

    VideoList ret;
    ret.append(list);

    // store video list to cache
    Friend::setVideoList(list, accountId, friendId);

    // check that it is not a my video list
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->accountId() == accountId &&
            this->accounts.at(i)->getProfile(false).ownerId() == friendId)
        {
            // if it is my video list then store, generate new full list and send it
            this->accounts[i]->setVideoList(list);
            for (int j = 0; j < this->accounts.length(); j++)
            {
                if (j != i)
                    ret.append(this->accounts[j]->getVideoList());
            }

            break;
        }
    }

    if (isLastUpdate)
    {
        this->videoUpdate--;
        if (this->videoUpdate < 0)
            this->videoUpdate = 0;
    }
    qDebug() << "islastUpdate=" << isLastUpdate << ", videoUpdate=" << videoUpdate << ", accountId=" << accountId << "owner=" << friendId;

    filterManager_->filterList(ret);
    emit updateVideoList(this->getProfile(accountId, friendId, false, false, false), ret, this->videoUpdate > 0 ? false: true);
}

MessageList ServiceMgr::getMessages(bool isNeedUpdate, bool useSignal)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() isNeedUpdate=" << (isNeedUpdate ? "true" : "false") << "useSignal=" << (useSignal ? "true" : "false");

    QMutexLocker lock(&mutexMessage);
    Q_UNUSED(lock);

    // true if we need to start update cache thread
    bool startUpdate = isNeedUpdate;

    // checks that all message lists was downloaded
    if (!startUpdate)
    {
        for (int i = 0; i < this->accounts.length(); i++)
        {
            if (this->accounts.at(i)->isDisabled()) continue;
            if (this->accounts.at(i)->isNeedUpdateMessages())
            {
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() accountId=" << this->accounts.at(i)->accountId();
                startUpdate = true;
                break;
            }
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() startUpdate=" << (startUpdate ? "true" : "false");
    // meessages not changed - return cache
    if (startUpdate)
    {
        // start update cache thread
        this->cleanThreads();
        if (!this->threads.contains("updateCacheMessages"))
        {
            MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheMessages, (isNeedUpdate ? UpdateType::MUST_UPDATE : UpdateType::CAN_UPDATE));
            this->threads.insert("updateCacheMessages", status);
            qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() run thread";
        } else {
            qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() cannot run thread!!!";
        }
    }

    if (useSignal && !startUpdate)
        emit this->updateMessageList(this->cachedMessageList, this->messagesUpdate > 0 ? false: true);

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() List count=" << this->cachedMessageList.length() << "messagesUpdate=" << this->messagesUpdate;
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return this->cachedMessageList;
}

void ServiceMgr::createCacheMessages(UpdateType::UpdateType isUpdate)
{
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() start thread";
    mutexMessage.lock();
    cachedMessageList.clear();
    mutexMessage.unlock();

    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;

        this->accounts.at(i)->setMessageListCached();
        MessageList curList = this->accounts.at(i)->getMessageList();
        mutexMessage.lock();
        cachedMessageList.append(curList);
        mutexMessage.unlock();

        // drafts
        MessageList draftList = this->accounts.at(i)->getDraftMessages();
        mutexMessage.lock();
        cachedMessageList.append(draftList);
        mutexMessage.unlock();

        if (isUpdate != UpdateType::NOT_UPDATE  &&
                (isUpdate == UpdateType::MUST_UPDATE ||
                (curList.isEmpty() && !curList.timeCreate().isValid()))) {
            QString threadId = QString("getMessages") + this->accounts[i]->accountId();

            // check that thread shouldn't already running
            if (!this->threads.contains(threadId) && this->accounts.at(i)->isNetworkEnabled()) {
                // add thread to list
                MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getMessages);
                this->threads.insert(threadId, status);
                this->messagesUpdate++;
            }
        }
    }
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() stop. List count=" << this->cachedMessageList.length() << "messagesUpdate=" << this->messagesUpdate;

    emit messageLoadingStateChanged(messagesUpdate > 0);

    emit this->updateMessageList(this->cachedMessageList, this->messagesUpdate > 0 ? false: true);
}

void ServiceMgr::gotMessageList(QString accountId, MessageList list, bool isLastUpdate)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": list.length()=" << list.length();

    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;

        if (this->accounts[i]->accountId() == accountId) {
            this->accounts[i]->setMessageList(list);
        }
    }

    if (isLastUpdate)
        this->messagesUpdate--;
    qDebug() << "islastUpdate=" << isLastUpdate << ", messagesUpdate=" << messagesUpdate;

    // start update cache thread
    this->cleanThreads();
    if (!this->threads.contains("updateCacheMessages"))
    {
        MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheMessages, UpdateType::NOT_UPDATE);
        this->threads.insert("updateCacheMessages", status);
    }

    emit messageLoadingStateChanged(messagesUpdate > 0);

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::deleteMessage(const Message& msg)
{
    this->deleteMessage(msg.accountId(), msg.messageId());
}

void ServiceMgr::deleteMessage(const QString &accountId, const QString &messageId)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): accountId=" << accountId << " messageId=" << messageId;

    this->cleanThreads();

    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (accountId == this->accounts[i]->accountId())
        {
            if (this->threads.contains(Message::deleteMessageKey(accountId, messageId)) || !this->accounts.at(i)->isNetworkEnabled())
                return;
            MSSFuture<void> status = runInThread(this->accounts[i], &Driver::deleteMessage, messageId);
            this->threads.insert(Message::deleteMessageKey(accountId, messageId), status);
            return;
        }
    }
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::gotMessageDeleted(const QString &accountId, const QString &messageId)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): accountId=" << accountId << "; messageId=" << messageId;

    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (this->accounts[i]->accountId() == accountId)
        {
            MessageList lst = this->accounts[i]->getMessageList();
            for (int j = 0; j < lst.length(); j++)
            {
                if (lst.at(j).messageId().compare(messageId) == 0)
                {
                    lst.removeAt(j);
                    this->messagesUpdate++;
                    this->gotMessageList(accountId, lst, true);
                    break;
                }
            }
            break;
        }
    }

    emit this->messageDeleted(accountId, messageId);
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::gotMessageSent(QString accountId, QString ownerId, QString title, QString text)
{
    emit messageSent(accountId, ownerId, title, text);
}

void ServiceMgr::gotAlbumCreated(const QString &accountId, const QString &albumId)
{
    // TODO: add album to account
    emit albumCreated(accountId, albumId);
}

bool ServiceMgr::canSendMessage(const QString &accountId)
{
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (accountId == this->accounts.at(i)->accountId())
            return this->accounts.at(i)->checkFunction(CLASS_MESSAGES, SEND_MESSAGE);
    }
    return false;
}

bool ServiceMgr::sendMessage(const QString &accountId, const QString &ownerId, const QString &title, const QString &message)
{
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "accountId=" << accountId << " to=" << ownerId;

    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (accountId == this->accounts.at(i)->accountId())
            return this->accounts.at(i)->sendMessage(ownerId, title, message);
    }
    return false;
}

void ServiceMgr::readMessage(const QString &accountId, const QString &messageId)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): accountId=" << accountId << " messageId=" << messageId;

    this->cleanThreads();

    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (accountId == this->accounts.at(i)->accountId())
        {
            if (this->threads.contains(Message::readMessageKey(accountId, messageId)) || !this->accounts.at(i)->isNetworkEnabled())
                break;
            MSSFuture<void> status = runInThread(this->accounts[i], &Driver::readMessage, messageId);
            this->threads.insert(Message::readMessageKey(accountId, messageId), status);
            break;
        }
    }
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::gotMessageReaded(const QString &accountId, const QString &messageId)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): accountId=" << accountId << "; messageId=" << messageId;

    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (this->accounts[i]->accountId() == accountId)
        {
            MessageList lst = this->accounts[i]->getMessageList();
            for (int j = 0; j < lst.length(); j++)
            {
                if (lst.at(j).messageId().compare(messageId) == 0)
                {
                    Message msg = lst[j];
                    msg.setReaded(true);
                    lst.replace(j, msg);
                    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): isReaded=" << (lst.at(j).isReaded() ? "true" : "false");
                    this->messagesUpdate++;
                    this->gotMessageList(accountId, lst, true);
                    break;
                }
            }
            break;
        }
    }

    emit this->messageReaded(accountId, messageId);
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

PhotoCommentList ServiceMgr::getComments(const Photo& curPhoto, const bool isNeedUpdate, const bool useSignal)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    PhotoCommentList ret;

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "load comments for " << curPhoto.photoId();

    ret = curPhoto.getPhotoComments();
    this->cleanThreads();

    if (isNeedUpdate || ret.isEmpty()) {
        for (int i = 0; i < this->accounts.length(); i++) {
            if (this->accounts.at(i)->isDisabled()) continue;
            if (curPhoto.accountId() == this->accounts[i]->accountId()) {
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__<< "(): Download comments for " << curPhoto.photoId();
                if (this->threads.contains(CoreSettings::getPhotoCommentListKey(curPhoto)) || !this->accounts.at(i)->isNetworkEnabled())
                    return ret;
                MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getPhotoComments, curPhoto);
                this->threads.insert(CoreSettings::getPhotoCommentListKey(curPhoto), status);
            }
        }
    }

    filterManager_->filterList(ret);

    if (useSignal)
        emit this->updatePhotoCommentList(curPhoto.photoId(), ret);

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return ret;
}

void ServiceMgr::gotComments(QString accountId, QString ownerId, QString albumId, QString photoId, PhotoCommentList list)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): photoId=" << photoId << " has comments=" << list.length();
    Photo::setPhotoComments(list, accountId, ownerId, albumId, photoId);
    filterManager_->filterList(list);
    emit this->updatePhotoCommentList(photoId, list);
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

bool ServiceMgr::sendComment(const Photo& curPhoto, const QString& comment)
{
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (curPhoto.accountId() == this->accounts.at(i)->accountId())
            return this->accounts.at(i)->sendPhotoComment(curPhoto, comment);
    }
    return false;
}

void ServiceMgr::gotProfile(QString accountId, QString reqOwnerId, Friend profile)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    Friend ret = profile;
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): reqOwnerId=" << reqOwnerId;
    if (reqOwnerId.isEmpty()) {
        for (int i = 0; i < this->accounts.length(); i++) {
            if (this->accounts.at(i)->isDisabled()) continue;
            if (this->accounts.at(i)->accountId().compare(accountId) == 0 && !profile.isPrivate() && !profile.isEmpty()) {
                bool isNE = this->accounts.at(i)->isNetworkEnabled();
                this->accounts.at(i)->setProfile(profile);
                if (!isNE)
                    emit updateAccounts(this->accounts.at(i)->accountId(), this->accounts);
                ret = this->getMyProfile(false);
                break;
            }
        }
    }

    profile.storeData();
    emit this->updateProfile(profile);
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

AccountList ServiceMgr::getAccounts()
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    // check that all accounts has profiles
    for (int i = 0; i < this->accounts.length(); i++)
    {
        if (this->accounts.at(i)->isDisabled()) continue;
        this->accounts.at(i)->getProfile(false);
    }

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return this->accounts;
}

void ServiceMgr::addAccount(QString accountName, const DriverInfo& driver)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    Account *newAcc;
    if (accountName.isEmpty())
        newAcc = new Account(driver.library(), Account::generateNewName(driver.serviceName()), false);
    else
        newAcc = new Account(driver.library(), accountName, false);

    if (newAcc != NULL && newAcc->ready()) {
        this->connectToTransport(newAcc);
        this->accounts.append(newAcc);
        this->updateDriverSettings();

        emit this->updateAccounts(newAcc->accountId(), this->accounts);
        //this->cachedFriendList.clear();
        this->updateData();
    }
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::updateAccount(Account *acc)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    emit this->updateAccounts(acc->accountId(), this->accounts);
    //this->cachedFriendList.clear();
    this->updateData();
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

Account *ServiceMgr::account(QString accountId)
{
    for (int i = 0; i < this->accounts.length(); i++)
    {
        if (this->accounts.at(i)->accountId().compare(accountId) == 0)
            return this->accounts.at(i);
    }

    return NULL;
}

void ServiceMgr::deleteAccount(Account *oldAcc)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    int i = -1;
    for (int j = 0; j < this->accounts.length(); j++) {
        if (oldAcc->accountId() == this->accounts.at(j)->accountId())
            i = j;
    }

    if (i >= 0) {
        mutex.lock();
        this->accounts.removeAt(i);
        mutex.unlock();

        runInThread(this, &ServiceMgr::removeAccountData, oldAcc);

        //execute accounts update process
        emit this->updateAccounts(QString::null, this->accounts);
        //this->cachedFriendList.clear();
        this->updateData();
    }
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::removeAccountData(Account *oldAcc)
{
    QDir dir(CoreSettings::getAccountDir(oldAcc->accountId()));

    delete oldAcc;
    qDebug() << "Try to remove dir " << dir.path();
    CoreSettings::RemoveDirectory(dir);
}

QList<DriverInfo> ServiceMgr::getDrivers()
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    if (!cachedDriverList.isEmpty())
    {
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
        return this->cachedDriverList;
    }

    QDir dir;
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot); // Choose only dirs
    dir.setPath(CoreSettings::getDriversDir());            // Search dirs in driversPath
    QFileInfoList lst = dir.entryInfoList();        // get list dirs in driversPath

    /* Each folder from lst it's potential folder with plugin */
    qDebug() << "Found " << lst.length() << " entries at " << dir.path();

    for (int i = 0; i < lst.length(); i++) { /* for each plugin folder */
        QFileInfo dir = lst.at(i);
        if (dir.isDir() && dir.fileName() != "." && dir.fileName() != "..") { /* resave check */
            QDir files;
            files.setFilter(QDir::Files);
            files.setNameFilters(QStringList(QString("*."LIB_SUFFIX)));
            files.setPath(dir.filePath());
            QFileInfoList lstFiles = files.entryInfoList(); /* get potential library files */
            for (int j = 0; j < lstFiles.length(); j++) {
                QFileInfo file = lstFiles.at(j);
                if (file.isFile()) { /* resave check */
                    DriverInfo curDr = Account::getDriverInfo(file.absoluteFilePath());
                    if (!curDr.isEmpty()) {
                        qDebug() << "found driver " << curDr.name();
                        this->cachedDriverList.append(curDr);
                    }
                }
            }
        }
    }

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return this->cachedDriverList;
}

PhotoList ServiceMgr::getPhotosForAlbum(Photo curPhoto)
{
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (this->accounts.at(i)->accountId() == curPhoto.accountId()) {
            return this->getPhotosForAlbum(this->accounts.at(i), curPhoto);
        }
    }

    PhotoList ret;
    return ret;
}

PhotoList ServiceMgr::getPhotosForAlbum(Account *curAcc, Photo curPhoto)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    AlbumList albums;

    // check that it can be my album
    if (curAcc->getProfile(false).ownerId() == curPhoto.ownerId()) {
        albums = curAcc->getAlbumList();
    } else {
        // or it is one of friend album
        FriendList friends;
        friends = curAcc->getFriendList();

        for (int i = 0; i < friends.length(); i++) {
            if (friends.at(i).ownerId() == curPhoto.ownerId()) {
                albums = friends[i].getAlbumList();
                break;
            }
        }
    }

    // get content of album
    for (int i = 0; i < albums.length(); i++) {
        if (albums.at(i).albumId() == curPhoto.albumId()) {
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
            return albums[i].getPhotoList();
        }
    }

    // try to load album if it not found
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): Can't found cached photos for album " << curPhoto.albumId();
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return this->getPhotos(curPhoto, false, false, false);
}

void ServiceMgr::downloadPhotos(Photo startPhoto, int nearest) {
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    qDebug() << "Start to download photo images from " << startPhoto.photoId() << " for " << startPhoto.albumId();

    this->cleanThreads();

    Account *acc = account(startPhoto.accountId());
    if (!acc || !acc->isNetworkEnabled() || acc->isDisabled())
    {
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
        return;
    }

    PhotoList photos = this->getPhotosForAlbum(acc, startPhoto);
    if (photos.isEmpty())
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "got empty photo list";

    if (!this->threads.contains(Photo::getKey(startPhoto)))
    {
        MSSFuture<void> ret = runInThread(acc->transport, &Driver::downloadPhotoList, photos, startPhoto, nearest);
        this->threads.insert(Photo::getKey(startPhoto), ret);
        this->photosUpdate++;
    }
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

QString ServiceMgr::downloadPhoto(Photo curPhoto)
{
    Account *acc = account(curPhoto.accountId());
    if (!acc || !acc->isNetworkEnabled() || acc->isDisabled())
        return QString::null;

    QString photo_path = curPhoto.photo();
    QFile file(photo_path);
    if (!file.exists()) {
        if (!acc->downloadPhoto(curPhoto, curPhoto.photoUrl(), file.fileName()))
            return QString::null;
    }
    return photo_path;
}

bool ServiceMgr::canUploadPhoto(const QString &accountId)
{
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (accountId == this->accounts.at(i)->accountId())
            return this->accounts.at(i)->checkFunction(CLASS_PHOTOS, UPLOAD_PHOTO);
    }
    return false;
}

void ServiceMgr::uploadPhoto(QString accountId, QString albumId,
                             QString file, QString description="")
{
    qDebug() << "Start to upload photo image: " << file;

    this->cleanThreads();

    const Account *acc = account(accountId);
    if (!acc || !acc->isNetworkEnabled() || acc->isDisabled())
        return;

    runInThread(acc->transport, &Driver::uploadPhoto, accountId, albumId, file, description);
}

void ServiceMgr::uploadPhoto(const Album album, QString file, QString description) {
    uploadPhoto(album.accountId(), album.albumId(), file, description);
}

void ServiceMgr::gotErrorMsg(QString accountId, ErrorMessage msg, Driver::Action acc)
{
    QString prefix(tr("During request "));
    QString comment;

    bool isMajor = true;

    // checks majority of error
    if (msg.code().compare(DRV_AUTH_CANCEL_CODE) == 0||
        msg.code().compare(DRV_UNKNOWN_METHOD_CALL) == 0 )
        isMajor = false;

    switch (acc) {
    case Driver::getListFriendsAction:
        mutexFriends.lock();
        this->friendsUpdate--;
        if (this->friendsUpdate < 0)
            this->friendsUpdate = 0;
        mutexFriends.unlock();
        prefix += tr("\"Update list of friends\"");
        break;
    case Driver::deleteFriendAction:
        prefix += tr("\"Delete friend from list\"");
        break;
    case Driver::getListAlbumsAction:
        this->albumsUpdate--;
        if (this->albumsUpdate < 0)
            this->albumsUpdate = 0;
        prefix += tr("\"Update list of albums\"");
        break;
    case Driver::getListPhotosAction:
        this->photosUpdate--;
        if (this->photosUpdate < 0)
            this->photosUpdate = 0;
        prefix += tr("\"Update list of images\"");
        break;
    case Driver::getListAudioAction:
        this->audioUpdate--;
        if (this->audioUpdate < 0)
            this->audioUpdate = 0;
        prefix += tr("\"Update list of audio\"");
        break;
    case Driver::getAudioByIdAction:
        prefix += tr("\"Get audio by id\"");
        break;
    case Driver::getListVideoAction:
        this->videoUpdate--;
        if (this->videoUpdate < 0)
            this->videoUpdate = 0;
        prefix += tr("\"Update list of video\"");
        break;
    case Driver::getProfileAction:
        prefix += tr("\"Update user profile\"");
        comment = tr("Account swiched to offline mode. Please check network settings and try again.");
        break;
    case Driver::getSettingsAction:
        prefix += tr("\"Get settings\"");
        break;
    case Driver::setSettingsAction:
        prefix += tr("\"Set settings\"");
        break;
    case Driver::getPhotoAction:
        this->photosUpdate--;
        if (this->photosUpdate < 0)
            this->photosUpdate = 0;
        prefix += tr("\"Download image\"");
        break;
    case Driver::uploadPhotoAction:
        prefix += tr("\"Upload image\"");
    case Driver::getCommentsAction:
        prefix += tr("\"Get comments\"");
        break;
    case Driver::sendCommentAction:
        prefix += tr("\"Send comment\"");
        break;
    case Driver::getMessagesAction:
        prefix += tr("\"Get list of messages\"");
        break;
    case Driver::sendMessageAction:
        prefix += tr("\"Send message\"");
        break;
    case Driver::readMessageAction:
        prefix += tr("\"Mark message as read\"");
        break;
    case Driver::deleteMessageAction:
        prefix += tr("\"Delete message\"");
        break;
    case Driver::getListMethodsAction:
        prefix += tr("\"Get list of registered methods\"");
        break;
    case Driver::createAlbumAction:
        prefix += tr("\"Create album\"");
        break;
    case Driver::getFeedAction:
        prefix += tr("\"Get feed\"");
        this->feedsUpdate--;
        if (this->feedsUpdate < 0)
            this->feedsUpdate = 0;
        break;
    case Driver::searchPlacesAction:
        prefix += tr("\"Search places\"");
        break;
    case Driver::getCheckinsAction:
        prefix += tr("\"Get checkins\"");
        break;
    case Driver::createPlaceAction:
        prefix += tr("\"Create place\"");
        break;
    case Driver::checkinAction:
        prefix += tr("\"Checkin\"");
        break;
    case Driver::getEventsAction:
        this->eventsUpdate--;
        if (this->eventsUpdate < 0)
            this->eventsUpdate = 0;
        prefix += tr("\"Get events\"");
        break;
    case Driver::createEventAction:
        prefix += tr("\"Create event\"");
        break;
    case Driver::deleteEventAction:
        prefix += tr("\"Delete event\"");
        break;
    }

    prefix += tr(" from account \"%1\" the error %2 was caused: ").arg(accountId).arg(msg.code()) + msg.text() + ".";
    if (!comment.isEmpty())
        prefix += " " + comment;

    emit this->errorOccured(prefix, acc, msg, isMajor);
}


void ServiceMgr::clearCache()
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    QString path;
    bool ret;
    for (int i = 0; i < this->accounts.length(); i++) {
        DriverInfo info = this->accounts.at(i)->getDriverInfo();
        // remove friend icons
        path = CoreSettings::getFriendsIconDir(info.name());
        QDir dir(path);
        ret = CoreSettings::RemoveFiles(dir, "*");
        qDebug() << "Remove " << path << " returned " << ret;

        // remove album icons
        path = CoreSettings::getAlbumsIconDir(info.name());
        dir = QDir(path);
        ret = CoreSettings::RemoveFiles(dir, "*");
        qDebug() << "Remove " << path << " returned " << ret;

        // remove friend list
        path = CoreSettings::getAccountDir(this->accounts.at(i)->accountId());
        dir = QDir(path);
        ret = CoreSettings::RemoveFiles(dir, FILE_FRIENDS_DATA);
        qDebug() << "Remove " << path << " + " << FILE_FRIENDS_DATA << " returned " << ret;
        ret = CoreSettings::RemoveFiles(dir, QString(FILE_ALBUMS_DATA).arg("*"));
        qDebug() << "Remove " << path << " + " << QString(FILE_ALBUMS_DATA).arg("*") << " returned " << ret;
        ret = CoreSettings::RemoveFiles(dir, QString(FILE_PHOTOS_DATA).arg("*", "*"));
        qDebug() << "Remove " << path << " + " << QString(FILE_PHOTOS_DATA).arg("*", "*") << " returned " << ret;
        ret = CoreSettings::RemoveFiles(dir, QString(FILE_AUDIO_DATA).arg("*"));
        qDebug() << "Remove " << path << " + " << QString(FILE_AUDIO_DATA).arg("*") << " returned " << ret;
        ret = CoreSettings::RemoveFiles(dir, QString(FILE_VIDEO_DATA).arg("*"));
        qDebug() << "Remove " << path << " + " << QString(FILE_VIDEO_DATA).arg("*") << " returned " << ret;

        path = CoreSettings::getPhotoDir(info.name());
        dir = QDir(path);
        ret = CoreSettings::RemoveFiles(dir, "*");
        qDebug() << "Remove " << path << " returned " << ret;

        path = CoreSettings::getAudioDir();
        dir = QDir(path);
        ret = CoreSettings::RemoveFiles(dir, "*");
        qDebug() << "Remove " << path << " returned " << ret;
    }
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

Friend ServiceMgr::getProfile(const QString& accountId, const QString& ownerId, const bool isNeedUpdate, const bool useSignal, const bool isFullProfile)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "accountId=" << accountId << "; isNeedUpdate=" << isNeedUpdate << "; isFullProfile=" << isFullProfile;
    Account *acc = this->account(accountId);
    assert(acc);
    QString service = this->account(accountId)->serviceName();
    bool isFound = false;
    bool hasFullProfile = false;
    Friend ret;

    if (!acc || acc->isDisabled())
    {
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
        return Friend();
    }

    // step 1. check downloaded full profiles from all accounts
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->serviceName().compare(service) != 0)
            continue;

        Friend nonFr = Friend::loadData(service, this->accounts.at(i)->accountId(), ownerId);
        if (nonFr.ownerId().compare(ownerId) == 0)
        {
            ret = nonFr;
            isFound = true;
            hasFullProfile = true;
            break;
        }
    }
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "step1 isFound" << isFound;

    // step 2. check friends from all accounts
    if (!isFound)
    {
        for (int i = 0; i < this->accounts.length(); i++)
        {
            if (this->accounts.at(i)->serviceName().compare(service) != 0)
                continue;

            // check owner id
            if (this->accounts.at(i)->getProfile(false).ownerId() == ownerId)
            {
                ret = this->accounts.at(i)->getProfile(false);
                isFound = true;
                break;
            }

            // check friends of owner
            FriendList frl = this->accounts.at(i)->getFriendList();
            for (int j = 0; j < frl.length(); j++)
            {
                if (frl.at(j).ownerId().compare(ownerId) == 0)
                {
                    ret = frl.at(j);
                    isFound = true;
                    break;
                }
            }

            if (isFound)
                break;
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "step2 isFound" << isFound;

    // step 3. download full profile if it isn't found or need update
    if (!isFound || isNeedUpdate || (isFullProfile && !hasFullProfile))
    {
        this->cleanThreads();

        if (!this->threads.contains(CoreSettings::getFriendKey(accountId, ownerId)))
        {
            MSSFuture<void> ret = runInThread(this->account(accountId)->transport,
                                                  &Driver::getProfile, ownerId);
            this->threads.insert(CoreSettings::getFriendKey(accountId, ownerId), ret);
        }
    }

    // return cached value
    if (useSignal && isFound)
        emit this->updateProfile(ret);

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return ret;
}

Friend ServiceMgr::getProfile(const PhotoComment& comment, const bool isNeedUpdate, const bool useSignal)
{
    return this->getProfile(comment.accountId(), comment.senderId(), isNeedUpdate, useSignal, false);
}

Friend ServiceMgr::getMyProfile(bool isNeedUpdate)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "isNeedUpdate=" << (isNeedUpdate ? "true" : "false") << "accounts.length()=" << accounts.length();
    Friend fd;
    if (this->accounts.length() > 0)
    {
        int start = 0;

        for (; start <this->accounts.length(); start++)
            if(!this->accounts.at(start)->isDisabled())
                break;

        if (start >= this->accounts.length())
            return fd;

        fd = this->accounts.at(start)->getProfile(isNeedUpdate);

        if (start + 1 < this->accounts.length())
        {
            for (int i = start + 1; i < this->accounts.length(); i++)
            {
                if (this->accounts.at(i)->isDisabled()) continue;
                fd.addProfile(this->accounts.at(i)->getProfile(isNeedUpdate), false);
            }
        }
    }
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return fd;
}

QEventFeedList ServiceMgr::getFeed(QEventFeed::FeedType type, bool isNeedUpdate, bool useSignal)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): isNeedUpdate="<< isNeedUpdate << "feedsUpdate=" << this->feedsUpdate << "feedType=" << QEventFeed::typeToString(type);
#endif

    bool startUpdate = isNeedUpdate;

    if (!startUpdate)
    {
        for (int i = 0; i < this->accounts.length(); i++)
        {
            if (this->accounts.at(i)->isDisabled()) continue;
            if (this->accounts.at(i)->isNeedUpdateNews(type))
            {
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() accountId=" << this->accounts.at(i)->accountId();
                startUpdate = true;
                break;
            }
        }
    }

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() startUpdate=" << (startUpdate ? "true" : "false");

    // if need rebuild cache then do it
    if (startUpdate)
    {
        // start update cache thread
        this->cleanThreads();
        if (!this->threads.contains("updateCacheNews" + QEventFeed::getKey("", type)))
        {
            MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheNews, (isNeedUpdate ? UpdateType::MUST_UPDATE : UpdateType::CAN_UPDATE), type);
            this->threads.insert("updateCacheNews" + QEventFeed::getKey("", type), status);
            qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() run thread";
        } else {
            qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() cannot run thread!!!";
        }
    }


    if (useSignal)
        emit this->updateFeed(this->cachedNewsList[type], type, this->feedsUpdate > 0 ? false : true);

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return this->cachedNewsList[type];
}

void ServiceMgr::createCacheNews(UpdateType::UpdateType isUpdate, QEventFeed::FeedType type)
{
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): start. feedType=" << QEventFeed::typeToString(type);
    mutex.lock();
    this->cachedNewsList.remove(type);
    mutex.unlock();

    QEventFeedList ret;
    this->cleanThreads();

    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;

        this->accounts.at(i)->setNewsListCached(type);
        QEventFeedList curList = this->accounts.at(i)->getNewsList(type);
        qDebug() << "before merging" << ret.length() << curList.length();
        ret.append(curList);

        if (isUpdate != UpdateType::NOT_UPDATE  &&
                (isUpdate == UpdateType::MUST_UPDATE ||
                (curList.isEmpty() && !curList.timeCreate().isValid()))) {
            QString threadId = QEventFeed::getKey(this->accounts[i]->accountId(), type);

            // check that thread shouldn't already running
            if (!this->threads.contains(threadId) && this->accounts.at(i)->isNetworkEnabled()) {
                // add thread to list
                qDebug() << "Try to download feed for " << this->accounts.at(i)->accountId();
                MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getFeed, type);
                this->threads.insert(threadId, status);
                this->feedsUpdate++;
            }
        }
    }

    this->cachedNewsList[type] = ret;

    emit this->updateFeed(ret, type, this->feedsUpdate > 0 ? false : true);
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): stop. feedType=" << QEventFeed::typeToString(type);
}

void ServiceMgr::gotEventFeed(QString accountId, QEventFeedList list, QEventFeed::FeedType type, bool isLastUpdate)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "(): accountId=" << accountId << " has news=" << list.length() << "feedType=" << QEventFeed::typeToString(type);

    for (int i = 0; i < this->accounts.length(); i++)
    {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (this->accounts.at(i)->accountId().compare(accountId) == 0)
            this->accounts.at(i)->setNewsList(type, list);
    }

    if (isLastUpdate)
        this->feedsUpdate--;
    if (this->feedsUpdate < 0)
        this->feedsUpdate = 0;

    // start update cache thread
    this->cleanThreads();
    if (!this->threads.contains("updateCacheNews" + QEventFeed::getKey("", type)))
    {
        MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheNews, UpdateType::NOT_UPDATE, type);
        this->threads.insert("updateCacheNews" + QEventFeed::getKey("", type), status);
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() run thread";
    } else {
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() cannot run thread!!!";
    }
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::updateData(int type)
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    bool all = (type == -1);

    if (this->canUpdateFriends && (all || type == qMetaTypeId<Friend>())) {
        // start update cache thread
        this->cleanThreads();
        if (!this->threads.contains("updateCacheFriends"))
        {
            MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheFriends, UpdateType::CAN_UPDATE);
            this->threads.insert("updateCacheFriends", status);
        } else {
            qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() cannot run thread!!!";
        }
    }

    if (this->canUpdateMessages && (all || type == qMetaTypeId<Message>())) {
        // start update cache thread
        this->cleanThreads();
        if (!this->threads.contains("updateCacheMessages"))
        {
            MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheMessages, UpdateType::CAN_UPDATE);
            this->threads.insert("updateCacheMessages", status);
            qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() run thread";
        } else {
            qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() cannot run thread!!!";
        }
    }

    if (this->canUpdateAlbums && (all || type == qMetaTypeId<Album>())) {
        getAlbums(false, true);
    }

    if (all && this->canUpdateFeeds)
    {
        if (this->canUpdateFriends)
        {
            QEventFeed::FeedType type = QEventFeed::friendFeed;
            // start update cache thread
            this->cleanThreads();
            if (!this->threads.contains("updateCacheNews" + QEventFeed::getKey("", type)))
            {
                MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheNews, UpdateType::CAN_UPDATE, type);
                this->threads.insert("updateCacheNews" + QEventFeed::getKey("", type), status);
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() run thread";
            } else {
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() cannot run thread!!!";
            }
        }

        if (this->canUpdateAlbums)
        {
            QEventFeed::FeedType type = QEventFeed::photoFeed;
            // start update cache thread
            this->cleanThreads();
            if (!this->threads.contains("updateCacheNews" + QEventFeed::getKey("", type)))
            {
                MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheNews, UpdateType::CAN_UPDATE, type);
                this->threads.insert("updateCacheNews" + QEventFeed::getKey("", type), status);
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() run thread";
            } else {
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() cannot run thread!!!";
            }
        }

        if (this->canUpdateMessages)
        {
            QEventFeed::FeedType type = QEventFeed::messageFeed;
            // start update cache thread
            this->cleanThreads();
            if (!this->threads.contains("updateCacheNews" + QEventFeed::getKey("", type)))
            {
                MSSFuture<void> status = runInThread(this, &ServiceMgr::createCacheNews, UpdateType::CAN_UPDATE, type);
                this->threads.insert("updateCacheNews" + QEventFeed::getKey("", type), status);
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() run thread";
            } else {
                qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "() cannot run thread!!!";
            }
        }
    }
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

void ServiceMgr::createFilter()
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    serviceFilter_ = new ServiceFilter(this, false, this);
    serviceFilter_->setObjectName("Main service filter");

    filterManager_ = new FilterManager(this);
    filterManager_->setObjectName("Main filter manager");

    filterManager_->registerFilterableType<Friend>();
    filterManager_->registerFilterableType<Album>();
    filterManager_->registerFilterableType<Message>();
    filterManager_->registerFilterableType<Photo>();
    filterManager_->registerFilterableType<Audio>();
    filterManager_->registerFilterableType<Video>();

    filterManager_->addFilter(new FilterFriendOnline(filterManager_));
    filterManager_->addFilter(new FilterFriendGender(filterManager_));
    filterManager_->addFilter(new FilterMessageReaded(filterManager_));
    filterManager_->addFilter(new FilterMessageType(this, filterManager_));
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
}

FilterManager* ServiceMgr::filterManager() const
{
    return filterManager_;
}

ServiceFilter* ServiceMgr::serviceFilter() const
{
    return serviceFilter_;
}

QList<QStatInfo> ServiceMgr::getStats() const
{
#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "start at" << this->timer.elapsed() << "msec";
#endif
    QList<QStatInfo> ret;
    for (int i = 0; i < this->accounts.length(); i++)
    {
        if (this->accounts.at(i)->isDisabled()) continue;
        if (this->accounts.at(i)->transport == NULL) continue;
        ret.append(this->accounts.at(i)->getStats());
    }

#ifdef TIME_DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "finish at" << this->timer.elapsed() << "msec";
#endif
    return ret;
}

PlaceList ServiceMgr::searchPlaces(QString text, QString latitude, QString longitude, QString radius)
{
    PlaceList ret;

    this->cleanThreads();


    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()){
            qDebug() << accounts.at(i)->serviceName() << "disabled";
            continue;
        }
        if (this->accounts.at(i)->isNetworkEnabled()) {
                MSSFuture<void> status = runInThread(this->accounts[i], &Driver::searchPlaces,
                                                         text, latitude, longitude, radius);
                break;
        }
        else
           qDebug() << accounts.at(i)->serviceName() << "disabled network";
    }

    return ret;
}

void ServiceMgr::gotPlaces(PlaceList list)
{
    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "count of place=" << list.length();
    //Photo::setPhotoComments(list, accountId, ownerId, albumId, photoId);
    //filterManager_->filterList(list);
    emit this->updatePlaceList(list);
}

CheckinList ServiceMgr::getCheckins(QString placeId, QString latitude, QString longitude, QString ownerId, const int type,const bool useSignal)
{
    CheckinList ret;

    this->cleanThreads();

    //ret = curPlace.getCheckinList();

    if (ret.isEmpty()) {
        for (int i = 0; i < this->accounts.length(); i++) {
            if (this->accounts.at(i)->isDisabled()) continue;
            if (this->accounts.at(i)->isNetworkEnabled()) {
                    MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getCheckins, placeId, latitude, longitude, ownerId, type);
                    break;
            }
        }
    }


    //filterManager_->filterList(ret);
    if(useSignal){;}
    return ret;
}

void ServiceMgr::gotCheckins(QString placeId, CheckinList list, const int type)
{
    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "count of checkins=" << list.length();
    //Place::setCheckinList(list, accountId, placeId);
    //filterManager_->filterList(list);
    emit this->updateCheckinList(placeId, list, type);
}

bool ServiceMgr::createPlace(QString text, QString latitude, QString longitude, QString type, QString address, QString comment, bool isCheckin)
{
    QString placeId;
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
            placeId = this->accounts.at(i)->createPlace(text, latitude, longitude, type, address);
    }
    if(isCheckin){
        for (int i = 0; i < this->accounts.length(); i++) {
            if (this->accounts.at(i)->isDisabled()) continue;
                return this->accounts.at(i)->checkIn(placeId, comment);
        }
    }
    if(placeId != "")
        return true;
    else return false;

}

bool ServiceMgr::checkIn(QString placeId, QString comment)
{
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
            return this->accounts.at(i)->checkIn(placeId, comment);
    }
    return false;
}



bool ServiceMgr::isLocalFriend(const QString& friendId)
{
    foreach (Account* account, accounts) {
        if (account->getProfile(false).ownerId() == friendId)
            return true;
    }
    return false;
}

DataCache* ServiceMgr::dataCache() const
{
    return dataCache_;
}

void ServiceMgr::allowDownloadingOldMessages(const QString& accountId, const QString& friendId)
{
    foreach (Account* acc, accounts) {
        if (accountId.isEmpty() || acc->accountId() == accountId) {
            acc->setFetchData(true, friendId);
            break;
        }
    }
}

void ServiceMgr::stopMessageLoading()
{
    foreach (Account* acc, accounts)
        acc->stopMessageLoading();
}

EventList ServiceMgr::getEvents(bool isNeedUpdate, bool useSignal)
{
    EventList ret;

    for (int i = 0; i < this->accounts.length(); i++) {
        ret.append(this->getEvents(this->accounts.at(i)->getProfile(false), isNeedUpdate, false));
    }

    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): this->eventUpdate=" << this->eventsUpdate;

    filterManager_->filterList(ret);

    if (useSignal && this->accounts.length() > 0)
        emit this->updateEventList(this->accounts.at(0)->getProfile(false), ret, this->eventsUpdate > 0 ? false: true);

    return ret;
}

EventList ServiceMgr::getEvents(const Friend& curFriend, bool isNeedUpdate, const bool useSignal)
{
    EventList ret;

    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): Load events list from cache for " << curFriend.ownerId()
            << " from " << curFriend.accountId() << " needUpdate=" << isNeedUpdate << "; this->eventUpdate="
            << this->eventsUpdate;

    ret = curFriend.getEventsList();

    qDebug() << "event list: " << endl;
    for (int t = 0; t < ret.length(); t++)
        qDebug() << ret.at(t).title() << endl;

    this->cleanThreads();
    QString own;

    if (isNeedUpdate || ret.isEmpty()) {
        for (int i = 0; i < this->accounts.length(); i++) {
            if (this->accounts.at(i)->isDisabled()) continue;
            if ((curFriend.accountId() == this->accounts[i]->accountId()) &&
                this->accounts.at(i)->isNetworkEnabled()) {
                qDebug() << "Download event list for " << curFriend.ownerId();

                if (this->threads.contains(QString("events") + curFriend.ownerId())) {
                    qDebug() << __FILE__ << ":" << __LINE__ << ": this->threads" << endl;
                    break;
                }

                own = curFriend.ownerId();
                MSSFuture<void> status = runInThread(this->accounts[i], &Driver::getEvents, own.remove(0,2), true);
                this->threads.insert(QString("events") + curFriend.ownerId(), status);
                this->eventsUpdate++;
                break;
            }
        }
    }

    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): this->eventsUpdate=" << this->eventsUpdate;

    filterManager_->filterList(ret);

    if (useSignal)
        emit this->updateEventList(curFriend, ret, this->eventsUpdate > 0 ? false: true);

    return ret;
}

EventList ServiceMgr::getEvents(const Event& ev, const bool isNeedUpdate, const bool useSignal)
{
    return this->getEvents(this->getProfile(ev.accountId(), ev.ownerId(), false, false, false), isNeedUpdate, useSignal);
}

void ServiceMgr::gotEventsList(QString accountId, QString friendId, EventList list, bool isLastUpdate)
{

    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): isLastUpdate=" << isLastUpdate;

    EventList ret;
    ret.append(list);

    // store events list to cache
    Friend::setEventsList(list, accountId, friendId);

    // check that it is not a my events list
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->accountId() == accountId &&
            this->accounts.at(i)->getProfile(false).ownerId() == friendId)
        {
            // if it is my events list then store, generate new full list and send it
            this->accounts[i]->setEventsList(list);
            for (int j = 0; j < this->accounts.length(); j++)
            {
                if (j != i)
                    ret.append(this->accounts[j]->getEventsList());
            }

            break;
        }
    }

    if (isLastUpdate)
    {
        this->eventsUpdate--;
        if (this->eventsUpdate < 0)
            this->eventsUpdate = 0;
    }
    qDebug() << "islastUpdate=" << isLastUpdate << ", eventsUpdate=" << eventsUpdate << ", accountId=" << accountId << "owner=" << friendId;

    filterManager_->filterList(ret);
    emit updateEventList(this->getProfile(accountId, friendId, false, false, false), ret, this->eventsUpdate > 0 ? false: true);
}



bool ServiceMgr::createEvent(QString title, QString description, QString startTime, QString endTime, QString location, QString privacy)
{
    QString eventId;
    for (int i = 0; i < this->accounts.length(); i++) {
        if (this->accounts.at(i)->isDisabled()) continue;
            eventId = this->accounts.at(i)->createEvent(title, description, startTime, endTime, location, privacy);
    }
    if(eventId != "")
        return true;
    else
        return false;
}

void ServiceMgr::loadWebauth() {
    bool webauthDetected = false;
#ifdef PLUGINPREFIX
    QCoreApplication::addLibraryPath(QString(PLUGINPREFIX));
#endif
    QStringList paths = QCoreApplication::libraryPaths();
    for (int i = 0; i < paths.count(); ++i) {
        const QString &pluginDir = paths.at(i);
        QDir files;
        files.setFilter(QDir::NoSymLinks | QDir::Files | QDir::Readable | QDir::CaseSensitive | QDir::NoDotAndDotDot);
        files.setNameFilters(QStringList(QChar('*')+QString(MSSN_WEBAUTH_LIBRARY_NAME)+QChar('*')));
        files.setPath(pluginDir);
        QFileInfoList lstFiles = files.entryInfoList(); /* get potential library files */
        for (int j = 0; j < lstFiles.length(); ++j) {
            QFileInfo file = lstFiles.at(j);
            qDebug() << "Found " << file.fileName() << " in " << pluginDir;
            if (file.isFile() && QLibrary::isLibrary(file.fileName())) {        
                
                QPluginLoader* driver = new QPluginLoader(file.absoluteFilePath());
                if (driver == 0) {
                    qWarning("Proplem with allocating memory for plugin");
                    continue;
                }     
                driver->setLoadHints(QLibrary::ResolveAllSymbolsHint); // RTLD_LAZY. TODO: Need?
                /* Don't try to delete the root component.
                \* Instead rely on that unload() will automatically delete it when needed. */
                QObject *plugin = driver->instance();
                if (plugin == 0) {
                    qWarning("Problems with initiation one of the plugin instances");
                    qDebug() << driver->errorString();
                    //driver->unload();
                    continue;
                }
                WebauthInterface *interface = qobject_cast<WebauthInterface *>(plugin);
                if (interface == 0) {
                    qWarning("This plugin not support WebauthInterface");
                    driver->unload();
                    continue;
                }
                
                webauth = interface->init();
                
                if (webauth) {
                    qWarning("Webauth detected!");
                    webauthDetected = true;
                    break;
                } else {
                    driver->unload();
                    continue;
                }
            }
        }
        if (webauthDetected)
            break;
    }
    if (!webauth)
        qWarning("Webauth not found!");
}
