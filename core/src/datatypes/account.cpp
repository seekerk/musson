#include "account.h"

Account::Account()
{
}

Account::Account(const Account &other)
    : QObject(other.parent())
{
}


bool Account::isDisabled() const
{
    return this->isDisabled();
}

void Account::setDisabled(bool isDisabled)
{
    if (this->disabled != isDisabled)
        emit accountUpdated(this->accountId());
    this->disabled = isDisabled;
}

QString Account::serviceName() const
{
    return "Account.ServiceName()";
}


Friend Account::getProfile(bool isNeedUpdate) const
{
    return Friend();
}


MessageList Account::getDraftMessages()
{
    return MessageList();
}

