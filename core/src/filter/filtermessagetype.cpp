#include "filtermessagetype.h"

#include <QMetaType>
#include <QActionGroup>
#include <QAction>
#include <QIcon>
#include <QSignalMapper>

#include "manager/servicemanager.h"
#include "datatypes/message.h"

FilterMessageType::FilterMessageType(ServiceManager* mgr, QObject *parent) :
    Filter(parent), mAllowedType(INBOX)
{
    mAccountList = mgr->getAccounts();
    updateLocalCache();
}

int FilterMessageType::filterableType() const
{
    return qMetaTypeId<Message>();
}

bool FilterMessageType::isFiltered(QVariant value) const
{
    const Message& message = value.value<Message>();

    switch (mAllowedType) {
    case ALL:
        return false;

    case INBOX:
        if (message.isSended())
            return true;
        if (message.isReceived())
            return false;

        break;

    case OUTBOX:
        if (message.isSended())
            return false;

        break;

    case DRAFTS:
        if (!message.isReceived() && !message.isSended())
            return false;

        if (mCachedDraftIds.contains(message.messageId()))
            return false;
    }

    return true;
}

QActionGroup* FilterMessageType::createActions()
{
    QActionGroup* actions = new QActionGroup(this);
    actions->setExclusive(true);

    QAction* allAction = new QAction(QIcon(":/res/msg_all.png"), tr("All messages"), actions);
    allAction->setCheckable(true);
    allAction->setData(QVariant::fromValue(ALL));

    QAction* inboxAction = new QAction(QIcon(":/res/msg_inbox.png"), tr("Inbox"), actions);
    inboxAction->setCheckable(true);
    inboxAction->setChecked(true);
    inboxAction->setData(QVariant::fromValue(INBOX));

    QAction* outboxAction = new QAction(QIcon(":/res/msg_outbox.png"), tr("Sent"), actions);
    outboxAction->setCheckable(true);
    outboxAction->setData(QVariant::fromValue(OUTBOX));

    QAction* draftAction = new QAction(QIcon(":/res/drafts.png"), tr("Drafts"), actions);
    draftAction->setCheckable(true);
    draftAction->setData(QVariant::fromValue(DRAFTS));

    connect(actions, SIGNAL(triggered(QAction*)), SLOT(setAllowedTypeAction(QAction*)));

    return actions;
}

void FilterMessageType::setAllowedTypeAction(QAction* action)
{
    mAllowedType = action->data().value<MessageType>();
    emitStateChanged();
}

void FilterMessageType::updateAccountList(QString id, AccountList accountList)
{
    Q_UNUSED(id);
    mAccountList = accountList;
    updateLocalCache();
    // TODO: emitStateChanged(); ???
}

void FilterMessageType::updateLocalCache()
{
    mLocalAccountIds.clear();
    mCachedDraftIds.clear();

    foreach (Account account, mAccountList) {
        const QString& id = account.getProfile(false).ownerId();
        mLocalAccountIds.insert(id);
        foreach (const Message& message, account.getDraftMessages()) {
            mCachedDraftIds.insert(message.messageId());
        }

        connect(&account, SIGNAL(draftsUpdated(MessageList)), SLOT(updateDrafts(MessageList)), Qt::UniqueConnection);
    }
}

void FilterMessageType::updateDrafts(MessageList drafts)
{
    foreach (const Message& message, drafts) {
        mCachedDraftIds.insert(message.messageId());
    }
}
