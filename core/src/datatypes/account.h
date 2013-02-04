#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <QObject>

#include "friend.h"
#include "datatypes/driverinfo.h"
#include "datatypes/message.h"

class Account : public QObject
{
    Q_OBJECT

public:

    Account();

    Account(QObject *parent);

    Account(const Account& other);

    Account(QString libraryName, QString accountId, bool isDisabled);

    QString accountId() const {return this->mAccountId;}

    DriverInfo getDriverInfo() const;

    QString serviceName() const;

    bool isDisabled() const;

    void setDisabled(bool isDisabled);

    bool isNetworkEnabled() const;

    static QString generateNewName(QString basis);

    Friend getProfile(bool isNeedUpdate) const;

    void setDraftMessages(const MessageList&);

    MessageList getDraftMessages();

    Account& operator=(const Account& other);

    bool ready() const;

signals:
    void accountUpdated(QString accountId);

private:
    QString mAccountId;

    bool disabled;
};

typedef QList<Account*> AccountList;

Q_DECLARE_METATYPE(Account)

#endif /* _ACCOUNT_H_ */
