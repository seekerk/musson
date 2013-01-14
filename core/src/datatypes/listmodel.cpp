#include "listmodel.h"
#include <QtDebug>
#include "datatypes/message.h"
#include "datatypes/friend.h"

ListModel::ListModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

ListModel::ListModel(ListItem* prototype, QObject *parent) :
    QAbstractListModel(parent), m_prototype(prototype)
{
    setRoleNames(m_prototype->roleNames());
}

inline bool toAscending(ListItem* li1 , ListItem* li2)
{
    QVariant v1 = li1->key();
    QVariant v2 = li2->key();
    if (v1.canConvert<QDateTime>()) {
        QDateTime dt1 = v1.value<QDateTime>();
        QDateTime dt2 = v2.value<QDateTime>();
        return dt1 < dt2;
    } else {
        if (v1.canConvert<QString>()) {
            QString str1 = v1.toString();
            QString str2 = v2.toString();
            return str1 < str2;
        }
    }
    return false;
}

inline bool toDescending(ListItem* li1 , ListItem* li2)
{
    QVariant v1 = li1->key();
    QVariant v2 = li2->key();
    if (v1.canConvert<QDateTime>()) {
        QDateTime dt1 = v1.value<QDateTime>();
        QDateTime dt2 = v2.value<QDateTime>();
        return dt1 > dt2;
    } else {
        if (v1.canConvert<QString>()) {
            QString str1 = v1.toString();
            QString str2 = v2.toString();
            return str1 > str2;
        }
    }
    return false;
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_list.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_list.size())
        return QVariant();
    return m_list.at(index.row())->data(role);
}

ListModel::~ListModel() {
    delete m_prototype;
    qDeleteAll(m_list);
    m_list.clear();
}

void ListModel::appendRow(ListItem *item)
{
    appendRows(QList<ListItem*>() << item);
}

void ListModel::appendRows(const QList<ListItem *> &items)
{
    emit beginInsertRows(QModelIndex(), rowCount(), rowCount() + items.count() - 1);
    foreach(ListItem *item, items) {
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        m_list.append(item);
    }
    emit endInsertRows();
    emit dataChanged(this->index(0), this->index(rowCount() - 1));
}

void ListModel::insertRow(int row, ListItem *item)
{
    emit beginInsertRows(QModelIndex(), row, row);
    connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
    m_list.insert(row, item);
    emit endInsertRows();
}

void ListModel::handleItemChange()
{
    ListItem* item = static_cast<ListItem*>(sender());
    QModelIndex index = indexFromItem(item);
    if(index.isValid())
        emit dataChanged(index, index);
}

ListItem * ListModel::find(const QString &id) const
{
    foreach(ListItem* item, m_list) {
        if(item->id() == id) return item;
    }
    return 0;
}

QModelIndex ListModel::indexFromItem(const ListItem *item) const
{
    Q_ASSERT(item);
    for(int row=0; row<m_list.size(); ++row) {
        if(m_list.at(row) == item) return index(row);
    }
    return QModelIndex();
}

void ListModel::clear()
{
    if (rowCount() > 0) {
        emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        qDeleteAll(m_list);
        m_list.clear();

        emit endRemoveRows();
    }
    emit dataChanged(this->index(0), this->index(rowCount() - 1));
}

bool ListModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || row >= m_list.size()) return false;
    emit beginRemoveRows(QModelIndex(), row, row);
    delete m_list.takeAt(row);
    emit endRemoveRows();
    return true;
}

bool ListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || (row + count) >= m_list.size()) return false;
    emit beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i=0; i < count; ++i) {
        delete m_list.takeAt(row);
    }
    emit endRemoveRows();
    return true;
}

ListItem * ListModel::takeRow(int row)
{
    emit beginRemoveRows(QModelIndex(), row, row);
    ListItem* item = m_list.takeAt(row);
    emit endRemoveRows();
    return item;
}

void ListModel::sort(int, Qt::SortOrder order)
{
    if (order == Qt::AscendingOrder) {
        qSort(m_list.begin(), m_list.end(), toAscending);
    } else {
        qSort(m_list.begin(), m_list.end(), toDescending);
    }
    emit dataChanged(this->index(0), this->index(rowCount() - 1));
}

void ListModel::slotBeginReset()
{
    emit dataChanged(this->index(0), this->index(rowCount() - 1));
}
