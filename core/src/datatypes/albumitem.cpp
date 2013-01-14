#include "albumitem.h"
#include <QFile>

AlbumItem::AlbumItem(Album album, QObject *parent) :
    ListItem(parent)
{
    m_album.setValue(album);
    m_title = album.title();
    m_description = album.description();
    m_icon = album.icon();

    QDateTime tc = QDateTime::fromString(album.time_create(), DATE_TIME_FORMAT);
    if (tc.date().year() == 1970)
        m_timeCreate = "";
    else
        m_timeCreate = album.time_create();

    QDateTime tu = QDateTime::fromString(album.time_update(), DATE_TIME_FORMAT);
    if (tu.date().year() == 1970)
        m_timeUpdate = "";
    else
        m_timeUpdate = album.time_update();

    m_ownerId = album.ownerId();
    m_accountId = album.accountId();
    m_albumId = album.albumId();
    m_size = album.size();
    m_canShowIcon = QFile::exists(m_icon);
}

AlbumItem::~AlbumItem()
{

}

QHash<int, QByteArray> AlbumItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TitleRole] = "title";
    names[CanShowIconRole] = "canShowIcon";
    names[DescriptionRole] = "description";
    names[TimeCreateRole] = "timeCreate";
    names[TimeUpdateRole] = "timeUpdate";
    names[IconRole] = "albumIcon";
    names[DataRole] = "album";
    names[IDRole] = "albumId";
    names[SizeRole] = "size";
    return names;
}

const QString& AlbumItem::title() const
{
    return m_title;
}

const QString& AlbumItem::description() const
{
    return m_description;
}

const QVariant& AlbumItem::album() const
{
    return m_album;
}

int AlbumItem::size() const
{
    return m_size;
}

const QString& AlbumItem::time_create() const
{
    return m_timeCreate;
}

const QString& AlbumItem::time_update() const
{
    return m_timeUpdate;
}

const QString& AlbumItem::icon() const
{
    return m_icon;
}

bool AlbumItem::canShowIcon() const
{
    return m_canShowIcon;
}

QString AlbumItem::id() const
{
    return m_albumId;
}

QVariant AlbumItem::key() const
{
    QVariant v;
    v.setValue(m_title);
    return v;
}

QVariant AlbumItem::data(int role) const
{
    switch(role) {
    case TitleRole:
        return title();
        break;
    case DescriptionRole:
        return description();
        break;
    case TimeCreateRole:
        return time_create();
        break;
    case TimeUpdateRole:
        return time_update();
        break;
    case IconRole:
        return icon();
        break;
    case CanShowIconRole:
        return canShowIcon();
        break;
    case DataRole:
        return album();
        break;
    case SizeRole:
        return size();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}
