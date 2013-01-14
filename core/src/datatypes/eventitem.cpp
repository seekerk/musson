#include "eventitem.h"

EventItem::EventItem(Event event, QObject *parent)
    : ListItem(parent)
{
    m_eventId = event.objectId();
    m_title = event.title();
    m_description = event.description();
    m_startTime = event.startTime();
    m_endTime = event.endTime();
    m_updateTime = event.updateTime();
    m_location = event.location();
    m_creator = event.creator();
    m_privacy = event.privacy();
    m_icon = event.icon();
    m_iconUrl = event.iconUrl();
    m_rsvpStatus = event.rsvpStatus();
}

EventItem::~EventItem()
{
}

QHash<int, QByteArray> EventItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TitleRole] = "title";
    names[DescriptionRole] = "description";
    names[StartTimeRole] = "startTime";
    names[EndTimeRole] = "endTime";
    names[UpdateTimeRole] = "updateTime";
    names[LocationRole] = "location";
    names[CreatorRole] = "creator";
    names[PrivacyRole] = "privacy";
    names[IconRole] = "icon";
    names[IconUrlRole] = "iconUrl";
    names[RsvpStatusRole] = "rsvpStatus";
    names[IDRole] = "eventId";
    return names;
}

const QString& EventItem::title() const
{
    return m_title;
}

const QString& EventItem::description() const
{
    return m_description;
}

const QString& EventItem::startTime() const
{
    return m_startTime;
}

const QString& EventItem::endTime() const
{
    return m_endTime;
}

const QString& EventItem::updateTime() const
{
    return m_updateTime;
}

const QString& EventItem::location() const
{
    return m_location;
}

const QString& EventItem::creator() const
{
    return m_creator;
}

const QString& EventItem::privacy() const
{
    return m_privacy;
}

const QString& EventItem::iconUrl() const
{
    return m_iconUrl;
}

const QString& EventItem::icon() const
{
    return m_icon;
}

const QString& EventItem::rsvpStatus() const
{
    return m_rsvpStatus;
}

QString EventItem::id() const
{
    return m_eventId;
}

void EventItem::setTitle(QVariant a)
{
    QString aTmp = a.toString();
    if (aTmp != m_title)
        this->m_title = aTmp;
}

void EventItem::setDescription(QVariant a)
{
    QString aTmp = a.toString();
    if (aTmp != m_description)
        this->m_description = aTmp;
}

void EventItem::setStartTime(QVariant a)
{
    QString aTmp = a.toString();
    if (aTmp != m_startTime)
        this->m_startTime = aTmp;
}

void EventItem::setEndTime(QVariant a)
{
    QString aTmp = a.toString();
    if (aTmp != m_endTime)
        this->m_endTime = aTmp;
}

void EventItem::setLocation(QVariant a)
{
    QString aTmp = a.toString();
    if (aTmp != m_location)
        this->m_location = aTmp;
}

void EventItem::setIcon(QVariant a)
{
    QString aTmp = a.toString();
    if (aTmp != m_icon)
        this->m_icon = aTmp;
}

void EventItem::setCreator(QVariant a)
{
    QString aTmp = a.toString();
    if (aTmp != m_creator)
        this->m_creator = aTmp;
}


QVariant EventItem::key() const
{
    QDateTime dt = QDateTime::fromString(m_time, "dd.MM.yyyy hh:mm:ss");
    QVariant v;
    v.setValue(dt);
    return v;
}

QVariant EventItem::data(int role) const
{
    switch(role) {
    case TitleRole:
        return title();
        break;
    case DescriptionRole:
        return description();
        break;
    case StartTimeRole:
        return startTime();
        break;
    case EndTimeRole:
        return endTime();
        break;
    case UpdateTimeRole:
        return updateTime();
        break;
    case LocationRole:
        return location();
        break;
    case CreatorRole:
        return creator();
        break;
    case PrivacyRole:
        return privacy();
        break;
    case IconRole:
        return icon();
        break;
    case IconUrlRole:
        return iconUrl();
        break;
    case RsvpStatusRole:
        return rsvpStatus();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}


