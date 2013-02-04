#include "datacache.h"

#include "servicemgr.h"

DataCache::DataCache(ServiceMgr* mgr, QObject *parent) :
    QObject(parent), m_serviceMgr(mgr)
{
    connectSignals();
}

void DataCache::setFriendList(const FriendList& friends)
{
    QWriteLocker lock(&m_lock);
    Q_UNUSED(lock);

    m_friendsCache.clear();
    foreach (const Friend& fr, friends) {
        m_friendsCache.insert(fr.ownerId(), fr);
    }
}

void DataCache::setAccountList(QString, const AccountList& accounts)
{
    QWriteLocker lock(&m_lock);
    Q_UNUSED(lock);

    m_localFriendsCache.clear();
    foreach (Account* account, accounts) {
        Friend localFriend = account->getProfile(false);
        m_localFriendsCache.insert(localFriend.ownerId(), localFriend);
    }
}

void DataCache::connectSignals()
{
    connect(m_serviceMgr, SIGNAL(updateFriends(FriendList,bool)), SLOT(setFriendList(FriendList)));
    connect(m_serviceMgr, SIGNAL(updateAccounts(QString,AccountList)), SLOT(setAccountList(QString,AccountList)));
}

Friend DataCache::getFriend(const QString& id) const
{   
    QReadLocker lock(&m_lock);
    Q_UNUSED(lock);

    QHash<QString, Friend>::const_iterator iter;
    if ((iter = m_friendsCache.find(id)) != m_friendsCache.constEnd())
        return *iter;
    else if ((iter = m_localFriendsCache.find(id)) != m_localFriendsCache.constEnd())
        return *iter;
    else
        return Friend();
}


