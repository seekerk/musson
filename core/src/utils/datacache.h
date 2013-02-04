#ifndef DATACACHE_H
#define DATACACHE_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QReadWriteLock>

#include "datatypes/friend.h"
#include "datatypes/account.h"

class ServiceMgr;

//! Class that can be used to retrieve datatype by its idenfifier
class DataCache : public QObject
{
    Q_OBJECT

public:
    //! Constructor
    /*!
      \param mgr ServiceMgr
      \param parent
    */
    DataCache(ServiceMgr* mgr, QObject *parent = 0);

    Friend getFriend(const QString& id) const;

public slots:
    //! Refresh friend list
    void setFriendList(const FriendList& friends);

    //! Refresh account list and set local friends
    void setAccountList(QString, const AccountList& accounts);

private:
    void connectSignals();

private:
    ServiceMgr* m_serviceMgr;

    QHash<QString, Friend> m_friendsCache;
    QHash<QString, Friend> m_localFriendsCache;

    mutable QReadWriteLock m_lock;
};

#endif // DATACACHE_H
