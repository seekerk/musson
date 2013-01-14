#include "filterfriendonline.h"

#include <QVariant>
#include <QActionGroup>
#include <QtDebug>

#include "datatypes/friend.h"

FilterFriendOnline::FilterFriendOnline(QObject *parent) :
    Filter(parent), enabled(false)
{
}

int FilterFriendOnline::filterableType() const
{
    return qMetaTypeId<Friend>();
}

bool FilterFriendOnline::isFiltered(QVariant value) const
{
    return enabled && !value.value<Friend>().online();
}

QActionGroup* FilterFriendOnline::createActions()
{
    QActionGroup* actions = new QActionGroup(this);
    actions->setExclusive(false);

    QAction* onlineAction = new QAction(tr("Online"), actions);
#ifndef Q_WS_MAEMO_5
    onlineAction->setIcon(QIcon(":/res/list_online.png"));
#else
    onlineAction->setIcon(QIcon(":/res/list_online_w.png"));
#endif
    onlineAction->setCheckable(true);
    connect(onlineAction, SIGNAL(toggled(bool)), SLOT(onlineActionToggled(bool)));

    return actions;
}

void FilterFriendOnline::onlineActionToggled(bool state)
{
    enabled = state;
    emitStateChanged();
}
