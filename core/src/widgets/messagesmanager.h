#ifndef MESSAGESMANAGER_H
#define MESSAGESMANAGER_H

#include <QObject>
#include <QStringListModel>
#include <manager/servicemanager.h>
#include <datatypes/listmodel.h>
#include <datatypes/messageitem.h>
#include <datatypes/message.h>
#include <QAction>

#include "filter/servicefilter.h"
#include "filter/filtermanager.h"
#include "filter/filtermessagetype.h"
#include "filter/filtermodel.h"
#include "musson_global.h"

class musson_EXPORT MessagesManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(FilterModel *model READ model NOTIFY modelChanged)
    Q_PROPERTY(bool canSendMessages READ canSendMessages NOTIFY canSendMessagesChanged)
    Q_PROPERTY(QString curMsgId READ curMsgId WRITE setCurMsgId NOTIFY curMsgIdChanged)
    Q_PROPERTY(QString curAccountId READ curAccountId WRITE setCurAccountId NOTIFY curAccountIdChanged)
    Q_PROPERTY(QString nameFilter READ nameFilter WRITE setNameFilter NOTIFY nameFilterChanged)
    Q_PROPERTY(bool isMessagesUpdate READ isMessagesUpdate NOTIFY isMessagesUpdateChanged)
    Q_PROPERTY(bool isFriendMessages READ isFriendMessages WRITE setFriendMessages NOTIFY isFriendMessagesChanged)
    Q_PROPERTY(int folder READ folder WRITE setFolder NOTIFY folderChanged)
public:
    explicit MessagesManager(QObject *parent = 0);
    ~MessagesManager();

    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);
    FilterModel* model();

    const QString& curMsgId() const;
    void setCurMsgId(const QString& messageId);

    const QString& curAccountId() const;
    void setCurAccountId(const QString& accountId);

    const QString& nameFilter() const;
    void setNameFilter(const QString& filter);

    bool canSendMessages() const;
    bool isMessagesUpdate() const;

    bool isFriendMessages() const;
    void setFriendMessages(bool active);

    int folder() const;
    void setFolder(int folder);

    enum MessagesFolder {
        All,
        Inbox,
        Sent,
        Drafts,
        New
    };

signals:
    void modelChanged();
    void curMsgIdChanged();
    void curAccountIdChanged();
    void nameFilterChanged();
    void canSendMessagesChanged();
    void isMessagesUpdateChanged();
    void isFriendMessagesChanged();
    void folderChanged();

public slots:
    void updateAccounts(QString accountName, AccountList list);
    void updateMessages(MessageList list, bool isLastUpdate);

    void refreshListMessages();
    void refreshListMessages(bool isNeedUpdate);

    void showFriendMessages(QVariant fd);

    void deleteMessage();
    void initMessages();

private:
    MessageList m_messageList;
    QString m_selectedMessageId;
    QString m_selectedAccountId;
    QString m_nameFilter;
    ListModel* m_model;
    FilterModel *m_filterModel;
    ServiceManager *m_sm;
    int m_folder;
    QVariant m_owner;

    bool m_canSendMessages;
    bool m_isFriendMessages;
    bool m_isMessagesUpdate;

    AccountList mAccountList;

    const QString messageAuthor(Message msg) const;
    QString messageService(Message msg);

    QActionGroup* messageTypeActions;
    QActionGroup* messageReadedActions;

    bool isFriendMessage(Friend fd, Message msg);
};

#endif // MESSAGESMANAGER_H
