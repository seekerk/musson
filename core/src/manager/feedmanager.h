#ifndef FEEDMANAGER_H
#define FEEDMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>
#include <QDateTime>
#include "datatypes/listmodel.h"
#include <datatypes/mstringlistmodel.h>
#include "datatypes/feeditem.h"
#include "datatypes/qattachment.h"
#include "datatypes/qeventfeed.h"
#include "musson_global.h"

struct musson_EXPORT FeedKey {
    QString ownerId;
    QDateTime date;
};

class musson_EXPORT FeedManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel *model READ model NOTIFY modelChanged)
    Q_PROPERTY(MStringListModel* filterModel READ filterModel NOTIFY filterModelChanged)
public:
    explicit FeedManager(QObject *parent = 0);
    ~FeedManager();

    ServiceManager* sm();
    void setServiceManager(ServiceManager* sm);

    ListModel* model();

    const QString& selected() const;
    void setSelected(const QString& selectedId);

    void setAccounts(QString, AccountList accountList);
    Friend findFriend(const QString& id) const;
    FeedKey getFeedKey(QEventFeed feed);

    //! Is type enabled
    bool feedTypeState(QEventFeed::FeedType feedType) const;

    MStringListModel* filterModel();

public slots:
    void updateFeed(QEventFeedList list, QEventFeed::FeedType type, bool isLastUpdate);
    void updateAccounts(QString accountName, AccountList list);
    void gotProfile(Friend profile);

    void refreshFeeds();
    void refreshFeeds(bool isNeedUpdate);
    void setFriends(FriendList friendList);

    //! Enable or disable displaying of specific feed type
    void setFeedTypeState(QEventFeed::FeedType, bool isActive);

    void setFeedType(QString type);

    void addFeedType(QEventFeed::FeedType);
    bool isPhoto(QVariant image);

signals:
    void modelChanged();
    void selectedChanged();
    void filterModelChanged();

    //! Emited when new feed type appears in FeedList
    void newFeedType(QEventFeed::FeedType type);

private:
    struct FeedTypeState
    {
        FeedTypeState();
        QEventFeedList list;
        bool active;
    };

    ListModel* m_model;
    MStringListModel *m_filterModel;
    QStringList m_filterNames;
    ServiceManager* m_sm;
    QString m_selectedId;
    FriendList m_friends;
    AccountList m_accounts;
    QList<FeedKey> m_feedList;
    QHash<QString,QEventFeed::FeedType> m_feedTypes;
    QHash<QEventFeed::FeedType, FeedTypeState> mFeedByType;
    void updateFeedItemList(const Friend fr = Friend());
};

#endif // FEEDMANAGER_H
