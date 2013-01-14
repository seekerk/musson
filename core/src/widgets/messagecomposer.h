#ifndef MESSAGECOMPOSER_H
#define MESSAGECOMPOSER_H

#include <QObject>
#include <manager/servicemanager.h>
#include "datatypes/listmodel.h"
#include "datatypes/profileitem.h"
#include <datatypes/message.h>
#include "musson_global.h"

class musson_EXPORT MessageComposer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel *model READ model NOTIFY modelChanged)
    Q_PROPERTY(QString selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(QString nameFilter READ nameFilter WRITE setNameFilter NOTIFY nameFilterChanged)
    Q_PROPERTY(QVariant owner READ owner WRITE setOwner NOTIFY ownerChanged)
public:
    explicit MessageComposer(QObject *parent = 0);
    ~MessageComposer();

    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);
    ListModel* model();

    const QString& selected() const;
    void setSelected(const QString& selectedId);

    const QString& nameFilter() const;
    void setNameFilter(const QString& filter);

    void setOwner(QVariant owner);
    QVariant owner();

signals:
    void modelChanged();
    void selectedChanged();
    void nameFilterChanged();
    void ownerChanged();

public slots:
    void updateAccounts(QString accountName, AccountList list);
    void updateFriends(FriendList list, bool isLastUpdate);

    void refreshListFriends();
    void refreshListFriends(bool isNeedUpdate);

    void sendMessage(const QString& subject, const QString& body);

private:
    FriendList m_friendList;
    ListModel* m_model;
    ServiceManager *m_sm;
    QString m_nameFilter;
    QString m_selectedId;
    QVariant m_owner;
};

#endif // MESSAGECOMPOSER_H
