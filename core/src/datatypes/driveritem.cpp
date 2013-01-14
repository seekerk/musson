#include "driveritem.h"

DriverItem::DriverItem(const DriverInfo& info, QObject *parent)
    : ListItem(parent)
{
    m_driver.setValue(info);
    m_name = info.name();
    m_icon = QIcon(info.icon());
}

QHash<int, QByteArray> DriverItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[IconRole] = "sIcon";
    names[DataRole] = "data";
    names[IDRole] = "driverId";
    return names;
}

const QString& DriverItem::name() const
{
    return m_name;
}

const QIcon& DriverItem::sIcon() const
{
    return m_icon;
}

const QVariant DriverItem::driver() const
{
    return m_driver;
}

QVariant DriverItem::data(int role) const
{
    switch(role) {
    case NameRole:
        return name();
        break;
    case IconRole:
        return sIcon();
        break;
    case DataRole:
        return driver();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}
