#include "filtermessagereaded.h"

#include <QActionGroup>
#include <QtDebug>

#include "datatypes/message.h"

FilterMessageReaded::FilterMessageReaded(QObject* parent) :
    Filter(parent), enabled(false)
{
}

int FilterMessageReaded::filterableType() const
{
    return qMetaTypeId<Message>();
}

bool FilterMessageReaded::isFiltered(QVariant value) const
{
    if (enabled)
    {
        Message msg = value.value<Message>();
        for (int i = 0; i < notReaded.length(); i++)
        {
            if (notReaded.at(i).accountId.compare(msg.accountId()) == 0 &&
                    notReaded.at(i).messageId.compare(msg.messageId()) == 0)
                return false;

        }

        if (msg.isReaded())
            return true;

        MessageIDs msId;
        msId.accountId = msg.accountId();
        msId.messageId = msg.messageId();
        this->notReaded.append(msId);
        return false;
    }
    else
        return false;
}

QActionGroup* FilterMessageReaded::createActions()
{
    QActionGroup* actions = new QActionGroup(this);
    actions->setExclusive(false);

    QAction* onlyNewAction = new QAction(tr("New"), actions);
    onlyNewAction->setIcon(QIcon(":/res/msg_listnew.png"));
    onlyNewAction->setCheckable(true);

    connect(onlyNewAction, SIGNAL(toggled(bool)), SLOT(actionToggled(bool)));

    return actions;
}

void FilterMessageReaded::actionToggled(bool state)
{
    enabled = state;
    if (enabled)
        notReaded.clear();
    emitStateChanged();
}
