#include "profilesmanager.h"

ProfilesManager::ProfilesManager(QObject *parent) :
    QObject(parent)
{
    m_model = new ListModel(new ProfileItem, this);
    m_isFriendsUpdate = false;
}

ProfilesManager::~ProfilesManager()
{
    delete m_model;
    delete m_filterModel;
}

void ProfilesManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;

    m_filterModel = new FilterModel(m_sm, this);
    m_filterModel->setSourceModel(m_model);
    m_filterModel->setFilterRole(Qt::UserRole + 1);

    connect(m_filterModel, SIGNAL(layoutAboutToBeChanged()), m_model, SLOT(slotBeginReset()));
    connect(m_sm, SIGNAL(updateAccounts(QString,AccountList)),
            this, SLOT(updateAccounts(QString,AccountList)));
    connect(m_sm, SIGNAL(updateFriends(FriendList,bool)),
            this, SLOT(updateFriends(FriendList,bool)));

    initProfiles();
}

void ProfilesManager::initProfiles()
{
    if (this->m_sm->getAccounts().length() != 0)
        this->updateFriends(m_sm->getFriends(false, false), true);
}

ServiceManager* ProfilesManager::sm()
{
    return m_sm;
}

FilterModel* ProfilesManager::model()
{
    return m_filterModel;
}

const QString& ProfilesManager::selected() const
{
    return m_selectedId;
}

void ProfilesManager::setSelected(const QString &selectedId)
{
    if (m_selectedId != selectedId) {
        m_selectedId = selectedId;
        emit selectedChanged();
    }
}

const QString& ProfilesManager::nameFilter() const
{
    return m_nameFilter;
}

void ProfilesManager::setNameFilter(const QString &filter)
{
    if (m_nameFilter != filter) {
        m_nameFilter = filter;

        m_model->clear();
        for (int i = 0; i < m_friendList.length(); i++) {
            if (m_friendList.at(i).name().indexOf(m_nameFilter, 0, Qt::CaseInsensitive) >= 0) {
                m_model->appendRow(new ProfileItem(m_friendList.at(i)));
            }
        }
        emit modelChanged();
        emit nameFilterChanged();
    }
}

int ProfilesManager::onlineCount() const
{
    return m_onlineCount;
}

void ProfilesManager::updateAccounts(QString, AccountList list)
{
    if (!list.isEmpty()) {
        this->refreshListFriends(false);
    }
}

void ProfilesManager::refreshListFriends()
{
    this->refreshListFriends(true);
}

void ProfilesManager::refreshListFriends(bool isNeedUpdate)
{
    if (!m_isFriendsUpdate) {
        m_isFriendsUpdate = true;
        emit isFriendsUpdateChanged();
        m_sm->getFriends(isNeedUpdate, true);
    }
}

bool ProfilesManager::isFriendsUpdate() const
{
    return m_isFriendsUpdate;
}

void ProfilesManager::updateFriends(FriendList list, bool isLastUpdate)
{
    if (isLastUpdate)
        m_isFriendsUpdate = false;
    else
        m_isFriendsUpdate = true;
    emit isFriendsUpdateChanged();

    m_friendList = list;

    m_model->clear();
    m_onlineCount = 0;
    for (int i = 0; i < list.length(); i++) {
        if (list.at(i).online())
            m_onlineCount++;
        m_model->appendRow(new ProfileItem(list.at(i)));
    }
    emit onlineCountChanged();
    emit modelChanged();
}

bool ProfilesManager::getOnlineStatus(QVariant profile)
{
    if (profile.canConvert<Friend>()) {
        Friend fd = profile.value<Friend>();
        ListItem* item = m_model->find(fd.ownerId());
        if (!item)
            return false;
        QVariant v = item->data(ProfileItem::DataRole);

        if (v.canConvert<Friend>()) {
            Friend fd2 = v.value<Friend>();
            return fd2.online();
        }
    }
    return false;
}
