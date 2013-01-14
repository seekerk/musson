#include "messagecomposer.h"

MessageComposer::MessageComposer(QObject *parent) :
    QObject(parent)
{
    m_model = new ListModel(new ProfileItem, this);
}

MessageComposer::~MessageComposer()
{
    delete m_model;
}

void MessageComposer::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;
    AccountList acc = m_sm->getAccounts();
    this->updateAccounts(QString::null, acc);

    connect(m_sm, SIGNAL(updateAccounts(QString,AccountList)),
            this, SLOT(updateAccounts(QString,AccountList)));
    connect(m_sm, SIGNAL(updateFriends(FriendList,bool)),
            this, SLOT(updateFriends(FriendList,bool)));
}

ServiceManager *MessageComposer::sm()
{
    return m_sm;
}

ListModel* MessageComposer::model()
{
    return m_model;
}

const QString& MessageComposer::nameFilter() const
{
    return m_nameFilter;
}

void MessageComposer::setNameFilter(const QString &filter)
{
    if (m_nameFilter != filter) {
        m_nameFilter = filter;

        m_model->clear();
        for (int i = 0; i < m_friendList.length(); i++) {
            if ((m_friendList.at(i).name().indexOf(m_nameFilter, 0, Qt::CaseInsensitive) >= 0)
                    && m_sm->canSendMessage(m_friendList.at(i).accountId()))
                m_model->appendRow(new ProfileItem(m_friendList.at(i)));
        }
        emit nameFilterChanged();
    }
}

const QString& MessageComposer::selected() const
{
    return m_selectedId;
}

void MessageComposer::setSelected(const QString &selectedId)
{
    if (m_selectedId != selectedId) {
        m_selectedId = selectedId;
        emit selectedChanged();
    }
}

void MessageComposer::updateAccounts(QString, AccountList list)
{
    if (!list.isEmpty())
        this->refreshListFriends(false);
}

void MessageComposer::refreshListFriends()
{
    this->refreshListFriends(true);
}

void MessageComposer::refreshListFriends(bool isNeedUpdate)
{
    m_sm->getFriends(isNeedUpdate, true);
}

void MessageComposer::setOwner(QVariant owner)
{
    if (owner.canConvert<Friend>()) {
        Friend fd = owner.value<Friend>();
        m_selectedId = fd.ownerId();
        emit selectedChanged();
        emit ownerChanged();
    }
}

QVariant MessageComposer::owner()
{
    return m_owner;
}

void MessageComposer::updateFriends(FriendList list, bool )
{
    m_friendList = list;

    m_model->clear();
    for (int i = 0; i < list.size(); i++) {
        if (m_sm->canSendMessage(list.at(i).accountId()))
            m_model->appendRow(new ProfileItem(list.at(i)));
    }
    emit modelChanged();
}

void MessageComposer::sendMessage(const QString &subject, const QString &body)
{
    Friend fd;
    for (int i = 0; i < m_friendList.length(); i++) {
        if (m_friendList.at(i).ownerId() == m_selectedId) {
            fd = m_friendList.at(i);
            break;
        }
    }

    if (!fd.isEmpty())
        m_sm->sendMessage(fd.accountId(), fd.ownerId(), subject, body);
}
