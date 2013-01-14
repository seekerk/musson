#include "checkinitem.h"

CheckinItem::CheckinItem(Checkin checkin, QObject *parent)
    : ListItem(parent)
{
    m_checkinId = checkin.checkinId();
    m_text = checkin.text();
    m_time = checkin.date();
    m_senderIcon = checkin.ownerPhoto();
    m_senderName = checkin.ownerName();
    m_latitude = checkin.latitude();
    m_longitude = checkin.longitude();
    m_placeId = checkin.placeId();
    m_placeTitle = checkin.placeTitle();
    m_placeCountry = checkin.placeCountry();
    m_placeCity = checkin.placeCity();
    m_placeAddress = checkin.placeAddress();
    m_placeType = checkin.placeType();
    m_placeIcon = checkin.placeIcon();
}

CheckinItem::~CheckinItem()
{
}

QHash<int, QByteArray> CheckinItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[SenderNameRole] = "senderName";
    names[SenderIconRole] = "senderIcon";
    names[LatitudeRole] = "latitude";
    names[LongitudeRole] = "longitude";
    names[DataRole] = "data";
    names[TextRole] = "comment";
    names[TimeRole] = "time";
    names[IDRole] = "checkinId";
    names[PlaceIdRole] = "placeId";
    names[PlaceTitleRole] = "placeTitle";
    names[PlaceCountryRole] = "placeCountry";
    names[PlaceCityRole] = "placeCity";
    names[PlaceAddressRole] = "placeAddress";
    names[PlaceTypeRole] = "placeType";
    names[PlaceIconRole] = "placeIcon";
    return names;
}

const QString& CheckinItem::senderName() const
{
    return m_senderName;
}

const QString& CheckinItem::senderIcon() const
{
    return m_senderIcon;
}

const QString& CheckinItem::text() const
{
    return m_text;
}

const QString& CheckinItem::time() const
{
    return m_time;
}

const QString& CheckinItem::latitude() const
{
    return m_latitude;
}

const QString& CheckinItem::longitude() const
{
    return m_longitude;
}

const QString& CheckinItem::placeId() const
{
    return m_placeId;
}

const QString& CheckinItem::placeTitle() const
{
    return m_placeTitle;
}

const QString& CheckinItem::placeCountry() const
{
    return m_placeCountry;
}

const QString& CheckinItem::placeCity() const
{
    return m_placeCity;
}

const QString& CheckinItem::placeAddress() const
{
    return m_placeAddress;
}

const QString& CheckinItem::placeType() const
{
    return m_placeType;
}

const QString& CheckinItem::placeIcon() const
{
    return m_placeIcon;
}

QString CheckinItem::id() const
{
    return m_checkinId;
}

QVariant CheckinItem::key() const
{
    QDateTime dt = QDateTime::fromString(m_time, "dd.MM.yyyy hh:mm:ss");
    QVariant v;
    v.setValue(dt);
    return v;
}

QVariant CheckinItem::data(int role) const
{
    switch(role) {
    case SenderNameRole:
        return senderName();
        break;
    case SenderIconRole:
        return senderIcon();
        break;
    case TextRole:
        return text();
        break;
    case TimeRole:
        return time();
        break;
    case LatitudeRole:
        return latitude();
        break;
    case LongitudeRole:
        return longitude();
        break;
    case PlaceIdRole:
        return placeId();
        break;
    case PlaceTitleRole:
        return placeTitle();
        break;
    case PlaceCountryRole:
        return placeCountry();
        break;
    case PlaceCityRole:
        return placeCity();
        break;
    case PlaceAddressRole:
        return placeAddress();
        break;
    case PlaceTypeRole:
        return placeType();
        break;
    case PlaceIconRole:
        return placeIcon();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}
