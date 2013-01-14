#ifndef FILTERMESSAGETYPE_H
#define FILTERMESSAGETYPE_H

#include <QMetaType>

#include <QSet>

#include "filter/filter.h"
#include "datatypes/account.h"
#include "musson_global.h"
#include "datatypes/message.h"

class ServiceManager;
class QAction;
class musson_EXPORT FilterMessageType : public Filter
{
    Q_OBJECT
    Q_ENUMS(MessageType)

public:
    enum MessageType {ALL, INBOX, OUTBOX, DRAFTS};

    explicit FilterMessageType(ServiceManager* mgr, QObject *parent = 0);

    int filterableType() const;
    bool isFiltered(QVariant value) const;
    QActionGroup* createActions();

private:
    MessageType mAllowedType;
    AccountList mAccountList;

    QSet<QString> mLocalAccountIds;
    QSet<QString> mCachedDraftIds; // TODO: it is unneeded if:
                                   // message is draft <=> message.isSended and message isReceived are false

    void updateLocalCache();

private slots:
    void setAllowedTypeAction(QAction* action);
    void updateAccountList(QString, AccountList);
    void updateDrafts(MessageList drafts);
};

Q_DECLARE_METATYPE(FilterMessageType::MessageType)

#endif // FILTERMESSAGETYPE_H
