#include "account.h"

void Account::setDisabled(bool isDisabled)
{
    if (this->disabled != isDisabled)
        emit accountUpdated(this->accountId());
    this->disabled = isDisabled;
}
