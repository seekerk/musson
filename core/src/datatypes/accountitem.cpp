#include "accountitem.h"

AccountItem::AccountItem(Account account, QObject *parent)
    : ListItem(parent)
{
    m_account.setValue(account);
    m_isDisabled = account.isDisabled();
    m_isNetworkEnabled = account.isNetworkEnabled();
    m_name = account.accountId();
    m_icon = account.getProfile(false).icon();;
    m_driverIcon = QIcon(account.getDriverInfo().icon());
}

AccountItem::~AccountItem()
{
}

QHash<int, QByteArray> AccountItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[IconRole] = "accIcon";
    names[DriverIconRole] = "driverIcon";
    names[DataRole] = "data";
    names[StatusRole] = "disabled";
    names[NetworkRole] = "isNetworkEnabled";
    names[IDRole] = "accountId";
    return names;
}

const QString& AccountItem::name() const
{
    return m_name;
}

const QString& AccountItem::icon() const
{
    return m_icon;
}

const QIcon& AccountItem::driverIcon() const
{
    return m_driverIcon;
}

const QVariant& AccountItem::account() const
{
    return m_account;
}

const bool& AccountItem::isDisabled() const
{
    return m_isDisabled;
}

const bool& AccountItem::isNetworkEnabled() const
{
    return m_isNetworkEnabled;
}

QString AccountItem::id() const
{
    return m_name;
}

QVariant AccountItem::data(int role) const
{
    switch(role) {
    case NameRole:
        return name();
        break;
    case IconRole:
        return icon();
        break;
    case DriverIconRole:
        return driverIcon();
        break;
    case DataRole:
        return account();
        break;
    case StatusRole:
        return isDisabled();
        break;
    case NetworkRole:
        return isNetworkEnabled();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}
