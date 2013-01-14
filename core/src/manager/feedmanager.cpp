#include "feedmanager.h"

static uint qHash(const FeedKey& key)
{
    return qHash(key.ownerId) ^ qHash(QDate(0, 0, 0).daysTo(key.date.date()));
}

static bool operator==(const FeedKey& key1, const FeedKey& key2)
{
    return key1.date.date() == key2.date.date() && key1.ownerId == key2.ownerId;
}

FeedManager::FeedManager(QObject *parent) :
    QObject(parent)
{
    m_model = new ListModel(new FeedItem);
    m_filterModel = new MStringListModel;
    m_filterNames.append(tr("All news"));
}

FeedManager::~FeedManager()
{
    delete m_model;
    delete m_filterModel;
}

void FeedManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;

    connect(m_sm, SIGNAL(updateAccounts(QString,AccountList)),
            this, SLOT(updateAccounts(QString,AccountList)));
    connect(m_sm, SIGNAL(updateFeed(QEventFeedList,QEventFeed::FeedType,bool)),
            this, SLOT(updateFeed(QEventFeedList,QEventFeed::FeedType,bool)));
    connect(m_sm, SIGNAL(updateFriends(FriendList,bool)), this,
            SLOT(setFriends(FriendList)));
    connect(this, SIGNAL(newFeedType(QEventFeed::FeedType)),
            this, SLOT(addFeedType(QEventFeed::FeedType)));

    this->setFriends(m_sm->getFriends(false, false));
    this->setAccounts(QString(), m_sm->getAccounts());
}

ServiceManager* FeedManager::sm()
{
    return m_sm;
}

ListModel* FeedManager::model()
{
    return m_model;
}

void FeedManager::updateAccounts(QString, AccountList list)
{
    if (!list.isEmpty()) {
        this->refreshFeeds(false);
    }
}

const QString& FeedManager::selected() const
{
    return m_selectedId;
}

void FeedManager::setSelected(const QString &selectedId)
{
    if (m_selectedId != selectedId) {
        m_selectedId = selectedId;
        emit selectedChanged();
    }
}

void FeedManager::updateFeed(QEventFeedList list, QEventFeed::FeedType type, bool isLastUpdate)
{
    Q_UNUSED(isLastUpdate);
    if (list.empty())
        return;

    bool isNewType = false;
    QHash<QEventFeed::FeedType, FeedTypeState>::Iterator iter = mFeedByType.find(type);
    if (iter == mFeedByType.constEnd()) {
        iter = mFeedByType.insert(type, FeedTypeState());
        isNewType = true;
    }

    iter->list = list;
    updateFeedItemList();

    if (isNewType)
        emit newFeedType(type);
}

void FeedManager::updateFeedItemList(Friend fr)
{
    m_model->clear();
    QHash<FeedKey, QList<QEventFeed> > hash;
    foreach (QEventFeed::FeedType type, mFeedByType.keys()) {
        FeedTypeState feedTypeState = mFeedByType.value(type);
        if (feedTypeState.active) {
            const QEventFeedList& list = feedTypeState.list;
            foreach (const QEventFeed& feed, list) {
                if (!fr.isEmpty() && (fr.ownerId() != feed.ownerId() || fr.accountId() != feed.accountId()))
                    continue;

                FeedKey key = getFeedKey(feed);
                hash[key].append(feed);

                Friend fr = findFriend(feed.ownerId());
                m_model->appendRow(new FeedItem(feed, type, fr));
            }
        }
    }

    m_model->sort(0, Qt::DescendingOrder);
    emit modelChanged();
}


void FeedManager::refreshFeeds()
{
    this->refreshFeeds(true);
}

void FeedManager::refreshFeeds(bool isNeedUpdate)
{
    m_model->clear();
    m_feedList.clear();
    m_sm->getFeed(QEventFeed::messageFeed, isNeedUpdate, true);
    m_sm->getFeed(QEventFeed::photoFeed, isNeedUpdate, true);
}

void FeedManager::setFriends(FriendList friendList)
{
    m_friends.clear();
    m_friends = friendList;
}

void FeedManager::setAccounts(QString, AccountList accountList)
{
    m_accounts.clear();
    m_accounts = accountList;
}

Friend FeedManager::findFriend(const QString& id) const
{
    for (int i = 0; i < m_friends.length(); i++) {
        if (m_friends.at(i).ownerId() == id)
            return m_friends.at(i);
    }

    for (int j = 0; j < m_accounts.length(); j++) {
        if (m_accounts.at(j).accountId() == id)
            return m_accounts.at(j).getProfile(false);
    }

    return Friend();
}

FeedKey FeedManager::getFeedKey(QEventFeed feed)
{
    FeedKey key;
    key.date = feed.created();
    key.ownerId = feed.ownerId();
    return key;
}

void FeedManager::gotProfile(Friend profile)
{
    Q_UNUSED(profile);
}


FeedManager::FeedTypeState::FeedTypeState()
    : active(true)
{
}

void FeedManager::setFeedTypeState(QEventFeed::FeedType feedType, bool state)
{
    QHash<QEventFeed::FeedType, FeedTypeState>::Iterator iter = mFeedByType.find(feedType);
    if (iter != mFeedByType.constEnd()) {
        if (iter->active != state) {
            iter->active = state;
            this->updateFeedItemList();
        }
    }
}

bool FeedManager::feedTypeState(QEventFeed::FeedType feedType) const
{
    return mFeedByType.value(feedType).active;
}

void FeedManager::addFeedType(QEventFeed::FeedType type)
{
    switch (type) {
    case QEventFeed::photoFeed:
        m_filterNames.append(tr("Photos"));
        m_feedTypes[m_filterNames.last()] = QEventFeed::photoFeed;
        break;
    case QEventFeed::messageFeed:
        m_filterNames.append(tr("Messages"));
        m_feedTypes[m_filterNames.last()] = QEventFeed::messageFeed;
        break;
    default:
        ;
    }

    m_filterModel->setStringList(m_filterNames);
    emit filterModelChanged();
}

MStringListModel* FeedManager::filterModel()
{
    return m_filterModel;
}

void FeedManager::setFeedType(QString type)
{
    if (type == tr("All news")) {
        foreach (QEventFeed::FeedType feedType, mFeedByType.keys()) {
            setFeedTypeState(feedType, true);
        }
        return;
    }

    foreach (QEventFeed::FeedType feedType, m_feedTypes.values()) {
        if (m_feedTypes[type] == feedType)
            setFeedTypeState(feedType,  true);
        else
            setFeedTypeState(feedType, false);
    }
}


bool FeedManager::isPhoto(QVariant image)
{
    return image.canConvert<Photo>();
}
