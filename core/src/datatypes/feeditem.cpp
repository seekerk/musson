#include "feeditem.h"

#include "utils/coresettings.h"

FeedItem::FeedItem(QEventFeed feed, QEventFeed::FeedType type, Friend owner, QObject* parent)
    : ListItem(parent)
{
    m_feed.setValue(feed);
    m_ownerName = feed.ownerName();

    if (!owner.isEmpty())
        m_ownerIcon = owner.icon();
    m_text = feed.text();
    m_ownerId = feed.ownerId();
    m_created = feed.created().toString(DATE_TIME_FORMAT);
    m_feedType = type;

    QAttachment attach = feed.attach();
    switch (attach.type()) {
    case QAttachment::ImageAttach:
        m_photo = attach.icon();
        m_photoData = feed.toPhoto();
        break;
    case QAttachment::LinkAttach:
        m_link = attach.objectUrl();
        break;
    default:
        break;
    }
}

QHash<int, QByteArray> FeedItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "ownerName";
    names[IconRole] = "ownerIcon";
    names[TextRole] = "feedMessage";
    names[CreatedRole] = "created";
    names[PhotoRole] = "photo";
    names[PhotoDataRole] = "photoData";
    names[IDRole] = "ownerId";
    names[TypeRole] = "type";
    names[LinkRole] = "link";
    names[DataRole] = "feed";
    return names;
}

const QString& FeedItem::ownerName() const
{
    return m_ownerName;
}

const QString& FeedItem::ownerIcon() const
{
    return m_ownerIcon;
}

const QString& FeedItem::photo() const
{
    return m_photo;
}

const QVariant FeedItem::photoData() const
{
    QVariant v;
    v.setValue(m_photoData);
    return v;
}

const QString& FeedItem::text() const
{
    return m_text;
}

const QString& FeedItem::link() const
{
    return m_link;
}

const QString& FeedItem::created() const
{
    return m_created;
}

const QVariant FeedItem::feed() const
{
    return m_feed;
}

int FeedItem::feedType() const
{
    return m_feedType;
}

QVariant FeedItem::key() const
{
    QDateTime dt = QDateTime::fromString(m_created, "dd.MM.yyyy hh:mm:ss");
    QVariant v;
    v.setValue(dt);
    return v;
}

QVariant FeedItem::data(int role) const
{
    switch(role) {
    case NameRole:
        return ownerName();
        break;
    case IconRole:
        return ownerIcon();
        break;
    case TextRole:
        return text();
        break;
    case CreatedRole:
        return created();
        break;
    case PhotoRole:
        return photo();
        break;
    case PhotoDataRole:
        return photoData();
        break;
    case LinkRole:
        return link();
        break;
    case TypeRole:
        return feedType();
        break;
    case DataRole:
        return feed();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}
