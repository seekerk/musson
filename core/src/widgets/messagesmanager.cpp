#include "messagesmanager.h"
#include <QChar>

MessagesManager::MessagesManager(QObject *parent) :
    QObject(parent)
{
    m_model = new ListModel(new MessageItem, this);
    m_canSendMessages = false;
    m_isMessagesUpdate = false;
    m_isFriendMessages = false;
    m_folder = Inbox;
}

MessagesManager::~MessagesManager()
{
    delete m_model;
    delete m_filterModel;
}

void MessagesManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;

    m_filterModel = new FilterModel(m_sm, this);
    m_filterModel->setSourceModel(m_model);
    m_filterModel->setFilterRole(Qt::UserRole + 1);

    connect(m_filterModel, SIGNAL(layoutAboutToBeChanged()), m_model, SLOT(slotBeginReset()));
    connect(m_sm, SIGNAL(updateAccounts(QString,AccountList)),
            this, SLOT(updateAccounts(QString,AccountList)));
    connect(m_sm, SIGNAL(updateMessageList(MessageList,bool)),
            this, SLOT(updateMessages(MessageList,bool)));
}

void MessagesManager::initMessages()
{
    // Filters
    QList<QActionGroup*> actionsList = m_sm->filterManager()->createActions<Message>();
    messageReadedActions = actionsList.at(0);
    messageTypeActions = actionsList.at(1);

    AccountList acc = m_sm->getAccounts();
    this->updateAccounts(QString::null, acc);

    if (this->m_sm->getAccounts().length() != 0)
        this->updateMessages(m_sm->getMessages(false, true), true);
}

ServiceManager* MessagesManager::sm()
{
    return m_sm;
}

FilterModel* MessagesManager::model()
{
    return m_filterModel;
}

const QString& MessagesManager::curMsgId() const
{
    return m_selectedMessageId;
}

void MessagesManager::setCurMsgId(const QString &messageId)
{
    if (m_selectedMessageId != messageId) {
        m_selectedMessageId = messageId;
        emit curMsgIdChanged();
    }
}

const QString& MessagesManager::curAccountId() const
{
    return m_selectedAccountId;
}

void MessagesManager::setCurAccountId(const QString &accountId)
{
    if (m_selectedAccountId != accountId) {
        m_selectedAccountId = accountId;
        emit curAccountIdChanged();
    }
}

const QString& MessagesManager::nameFilter() const
{
    return m_nameFilter;
}

void MessagesManager::setNameFilter(const QString &)
{

}

int MessagesManager::folder() const
{
    return m_folder;
}

void MessagesManager::setFolder(int folder)
{
    if (m_folder != folder) {
        m_folder = folder;

        if (m_folder < 3) {
            if (messageReadedActions->actions().at(0)->isChecked())
                messageReadedActions->actions().at(0)->toggle();
            messageTypeActions->actions().at(m_folder)->trigger();
        } else {
            // Enable All and New filters
            messageTypeActions->actions().at(0)->trigger();
            messageReadedActions->actions().at(0)->toggle();
        }
        emit folderChanged();
    }
}

void MessagesManager::updateAccounts(QString, AccountList list)
{
    mAccountList = list;
    if (!list.isEmpty())
        this->refreshListMessages(false);
}

bool MessagesManager::canSendMessages() const
{
    return m_canSendMessages;
}

bool MessagesManager::isMessagesUpdate() const
{
    return m_isMessagesUpdate;
}

bool MessagesManager::isFriendMessages() const
{
    return m_isFriendMessages;
}

void MessagesManager::setFriendMessages(bool active)
{
    if (m_isFriendMessages != active) {
        m_isFriendMessages = active;

        if (!m_isFriendMessages) {
            updateMessages(m_messageList, true);
        }
        emit isFriendMessagesChanged();
    }
}

void MessagesManager::refreshListMessages()
{
    this->refreshListMessages(true);
}

void MessagesManager::refreshListMessages(bool isNeedUpdate)
{
    if (!m_isMessagesUpdate) {
        m_isMessagesUpdate = true;
        emit isMessagesUpdateChanged();
        m_sm->getMessages(isNeedUpdate, true);
    }
}

void MessagesManager::updateMessages(MessageList list, bool isLastUpdate)
{
    if (isLastUpdate)
        m_isMessagesUpdate = false;
    else
        m_isMessagesUpdate = true;
    emit isMessagesUpdateChanged();

    m_messageList = list;

    m_model->clear();
    for (int i = 0; i < list.size(); i++) {
        m_model->appendRow(new MessageItem(list.at(i),
                                           messageAuthor(list.at(i)),
                                           messageService(list.at(i))));
    }
    m_model->sort(0, Qt::DescendingOrder);

    if (!m_isFriendMessages)
        emit modelChanged();
    else
        showFriendMessages(m_owner);
}

const QString MessagesManager::messageAuthor(Message msg) const
{
    QString author;

    if (msg.isSended() && !msg.recepientListConst().isEmpty())
        author = msg.recepientListConst().first().name;

    if (msg.isReceived())
        author = msg.senderName();

    if (author.isEmpty())
        author = msg.senderName();
    return author;
}

QString MessagesManager::messageService(Message msg)
{
    QString service;
    for (int i = 0; i < mAccountList.length(); i++) {
        if (mAccountList.at(i).accountId() == msg.accountId()) {
            DriverInfo info = mAccountList.at(i).getDriverInfo();
            service = info.name();
            break;
        }
    }
    return service;
}

void MessagesManager::deleteMessage()
{
    m_sm->deleteMessage(curAccountId(), curMsgId());
}

void MessagesManager::showFriendMessages(QVariant fd)
{
    m_owner = fd;
    if (fd.canConvert<Friend>()) {
        Friend fr = fd.value<Friend>();

        m_model->clear();
        for (int i = 0; i < m_messageList.length(); i++) {
            if (isFriendMessage(fr, m_messageList.at(i)))
                m_model->appendRow(new MessageItem(m_messageList.at(i),
                                                   messageAuthor(m_messageList.at(i)),
                                                   messageService(m_messageList.at(i))));
        }
        emit modelChanged();
    }
}

bool MessagesManager::isFriendMessage(Friend fd, Message msg)
{
    foreach (const Recepient& recepient, msg.recepientListConst())
        if (fd.ownerId() == recepient.id)
            return true;

    if (fd.ownerId() == msg.senderId())
        return true;
    return false;
}
