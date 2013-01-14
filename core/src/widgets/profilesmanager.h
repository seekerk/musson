#ifndef PROFILESMANAGER_H
#define PROFILESMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>
#include "datatypes/listmodel.h"
#include "datatypes/profileitem.h"
#include <datatypes/friend.h>
#include <datatypes/driverinfo.h>
#include <filter/filtermodel.h>
#include "musson_global.h"

class musson_EXPORT ProfilesManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(FilterModel *model READ model NOTIFY modelChanged)
    Q_PROPERTY(QString selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(QString nameFilter READ nameFilter WRITE setNameFilter NOTIFY nameFilterChanged)
    Q_PROPERTY(int onlineCount READ onlineCount NOTIFY onlineCountChanged)
    Q_PROPERTY(bool isFriendsUpdate READ isFriendsUpdate NOTIFY isFriendsUpdateChanged)
public:
    explicit ProfilesManager(QObject *parent = 0);
    ~ProfilesManager();

    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);
    FilterModel* model();

    const QString& selected() const;
    void setSelected(const QString& selectedId);

    const QString& nameFilter() const;
    void setNameFilter(const QString& filter);

    bool isFriendsUpdate() const;
    int onlineCount() const;

signals:
    void modelChanged();
    void selectedChanged();
    void nameFilterChanged();
    void isFriendsUpdateChanged();
    void onlineCountChanged();

public slots:
    void updateAccounts(QString accountName, AccountList list);
    void updateFriends(FriendList list, bool isLastUpdate);

    void refreshListFriends();
    void refreshListFriends(bool isNeedUpdate);

    void initProfiles();
    bool getOnlineStatus(QVariant profile);

private:
    FriendList m_friendList;
    QString m_selectedId;
    QString m_nameFilter;
    ListModel* m_model;
    ServiceManager *m_sm;
    int m_onlineCount;
    FilterModel *m_filterModel;

    bool m_isFriendsUpdate;
};

#endif // PROFILESMANAGER_H
