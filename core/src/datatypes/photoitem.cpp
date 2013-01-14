#include "photoitem.h"
#include <QFile>

PhotoItem::PhotoItem(Photo photo, QObject *parent) :
    ListItem(parent)
{
    m_photoData.setValue(photo);
    m_photo = photo.photo();
    m_description = photo.description();
    m_icon = photo.icon();
    m_timeCreate = photo.time_create();
    m_photoId = photo.photoId();
    m_canShowImage = QFile::exists(m_photo);
    m_canShowIcon = QFile::exists(m_icon);
}

PhotoItem::~PhotoItem()
{

}

QHash<int, QByteArray> PhotoItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[PhotoRole] = "photo";
    names[IconRole] = "photoIcon";
    names[CanShowImageRole] = "canShowImage";
    names[CanShowIconRole] = "canShowIcon";
    names[DescriptionRole] = "description";
    names[TimeCreateRole] = "timeCreate";
    names[IDRole] = "photoId";
    names[DataRole] = "photoData";
    return names;
}

const QString& PhotoItem::photo() const
{
    return m_photo;
}

const QString& PhotoItem::icon() const
{
    return m_icon;
}

const QString& PhotoItem::description() const
{
    return m_description;
}

const QString& PhotoItem::time_create() const
{
    return m_timeCreate;
}

const QVariant& PhotoItem::photoData() const
{
    return m_photoData;
}

bool PhotoItem::canShowImage() const
{
    return m_canShowImage;
}

bool PhotoItem::canShowIcon() const
{
    return m_canShowIcon;
}

QString PhotoItem::id() const
{
    return m_photoId;
}

QVariant PhotoItem::key() const
{
    return QVariant();
}

QVariant PhotoItem::data(int role) const
{
    switch(role) {
    case PhotoRole:
        return photo();
        break;
    case IconRole:
        return icon();
        break;
    case CanShowImageRole:
        return canShowImage();
        break;
    case CanShowIconRole:
        return canShowIcon();
        break;
    case DescriptionRole:
        return description();
        break;
    case TimeCreateRole:
        return time_create();
        break;
    case IDRole:
        return id();
        break;
    case DataRole:
        return photoData();
        break;
    default:
        return QVariant();
    }
}
