#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>
#include "datatypes/listmodel.h"
#include <datatypes/accountitem.h>
#include <datatypes/profileitem.h>
#include <datatypes/account.h>
#include <datatypes/friend.h>
#include <datatypes/driverinfo.h>
#include "musson_global.h"

class AccountItem;
class musson_EXPORT AccountManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel *model READ model NOTIFY modelChanged)
    Q_PROPERTY(ListModel *profilesModel READ profilesModel NOTIFY profilesModelChanged)
    Q_PROPERTY(QVariant myProfile READ myProfile)
    Q_PROPERTY(QString myName READ myProfileName)
    Q_PROPERTY(QString myIcon READ myProfileIcon)
    Q_PROPERTY(QString selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(bool isDisabled READ isDisabled WRITE setDisabled NOTIFY disabledChanged)
    Q_PROPERTY(bool hasAccounts READ hasAccounts NOTIFY modelChanged)

public:
    explicit AccountManager(QObject *parent = 0);
    ~AccountManager();

    ServiceManager* sm();
    void setServiceManager(ServiceManager* sm);
    ListModel* model();
    ListModel* profilesModel();

    QVariant myProfile();

    QString myProfileIcon();

    QString myProfileName();

    const QString& selected() const;
    void setSelected(const QString& selectedId);

    bool isDisabled() const;
    void setDisabled(bool disabled);

    bool hasAccounts() const;

signals:
    void modelChanged();
    void selectedChanged();
    void profilesModelChanged();
    void disabledChanged();

public slots:
    void updateAccounts(QString accountName, AccountList name);
    //void addAccount(QString accountName);
    void addAccount(QString accountName, DriverInfo* driver);
    void addAccount(DriverInfo* driver);
    void deleteAccount();
    void reconnect();
    void refreshProfile();
    void gotProfile(Friend);
    void toggleAccount();

private:
    QString m_selectedId;
    ListModel* m_model;
    ListModel* m_profilesModel;
    ServiceManager* m_sm;
    FriendList m_profilesList;
    bool m_isDisabled;
};

#endif // ACCOUNTMANAGER_H
