#include "profileitem.h"

ProfileItem::ProfileItem(Friend profile, QObject* parent) :
    ListItem(parent)
{
    m_profile.setValue(profile);
    m_offline = !profile.online();
    m_ownerId = profile.ownerId();
    m_name = profile.name();
    m_icon = profile.icon();
    m_serviceId = profile.serviceId();
}

ProfileItem::ProfileItem(Friend profile, QString service, QObject* parent) :
    ListItem(parent)
{
    m_profile.setValue(profile);
    m_offline = !profile.online();
    m_ownerId = profile.ownerId();
    m_name = profile.name();
    m_icon = profile.icon();
    m_serviceId = service;
}

ProfileItem::~ProfileItem()
{

}

QHash<int, QByteArray> ProfileItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[IconRole] = "profileIcon";
    names[DataRole] = "profile";
    names[StatusRole] = "offline";
    names[IDRole] = "ownerId";
    names[ServiceRole] = "service";
    return names;
}

const QString& ProfileItem::name() const
{
    return m_name;
}

const QString& ProfileItem::icon() const
{
    return m_icon;
}

const QVariant& ProfileItem::profile() const
{
    return m_profile;
}

const bool& ProfileItem::offline() const
{
    return m_offline;
}

const QString& ProfileItem::serviceId() const
{
    return m_serviceId;
}

QString ProfileItem::id() const
{
    return m_ownerId;
}

QVariant ProfileItem::key() const
{
    QVariant v;
    v.setValue(m_name);
    return v;
}

QVariant ProfileItem::data(int role) const
{
    switch(role) {
    case NameRole:
        return name();
        break;
    case IconRole:
        return icon();
        break;
    case DataRole:
        return profile();
        break;
    case StatusRole:
        return offline();
        break;
    case ServiceRole:
        return serviceId();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}
