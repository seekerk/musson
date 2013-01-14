#include "mstringlistmodel.h"

MStringListModel::MStringListModel(const QByteArray &displayRoleName, QObject *parent)
    : QStringListModel(parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, displayRoleName);
    setRoleNames(roleNames);
}

void
MStringListModel::setStringList(const QStringList &strings)
{
    if (!stringList().isEmpty())
    {
        emit stringsReset();
    }
    QStringListModel::setStringList(strings);
    QStringList::const_iterator i = strings.constBegin();
    for (; i != strings.constEnd(); ++i) {
        QString s = *i;
        emit stringAdded(s);
    }
}
