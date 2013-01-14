#include "filterfriendgender.h"

#include <QVariant>
#include <QActionGroup>
#include <QtDebug>
#include <QString>

#include "datatypes/friend.h"

FilterFriendGender::FilterFriendGender(QObject *parent) :
    Filter(parent), maleFiltered(false), femaleFiltered(true)
{
}

int FilterFriendGender::filterableType() const
{
    return qMetaTypeId<Friend>();
}

bool FilterFriendGender::isFiltered(QVariant value) const
{
    QString gender = value.value<Friend>().gender();

    if (gender.isNull())
        return false;

    if (gender == "male" && maleFiltered)
        return false;

    if (gender == "female" && femaleFiltered)
        return false;

    return true;
}

QActionGroup* FilterFriendGender::createActions()
{
    QActionGroup* group = new QActionGroup(this);
    group->setExclusive(false);

    QAction* maleGender = new QAction(tr("Male"), group);
    group->addAction(maleGender);
    maleGender->setCheckable(true);
    maleGender->setChecked(true);
#ifndef Q_WS_MAEMO_5
    maleGender->setIcon(QIcon(":/res/male.png"));
#else
    maleGender->setIcon(QIcon(":/res/male_w.png"));
#endif

    connect(maleGender, SIGNAL(toggled(bool)), SLOT(maleFilterChanged(bool)));

    QAction* femaleGender = new QAction(tr("Female"), group);
    group->addAction(femaleGender);
    femaleGender->setCheckable(true);
    femaleGender->setChecked(true);
#ifndef Q_WS_MAEMO_5
    femaleGender->setIcon(QIcon(":/res/female.png"));
#else
    femaleGender->setIcon(QIcon(":/res/female_w.png"));
#endif
    connect(femaleGender, SIGNAL(toggled(bool)), SLOT(femaleFilterChanged(bool)));

    return group;
}

void FilterFriendGender::maleFilterChanged(bool state)
{
    maleFiltered = state;
    emitStateChanged();
}

void FilterFriendGender::femaleFilterChanged(bool state)
{
    femaleFiltered = state;
    emitStateChanged();
}
