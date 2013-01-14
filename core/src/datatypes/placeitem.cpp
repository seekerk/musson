#include "placeitem.h"

PlaceItem::PlaceItem(Place place, QObject *parent)
    : ListItem(parent)
{
    m_placeId = place.placeId();
    m_title = place.title();
    m_latitude = place.latitude();
    m_longitude = place.longitude();
    m_type = place.type();
    m_country = place.country();
    m_city = place.city();
    m_address = place.address();
    m_icon = place.icon();
    m_distance = place.distance();
}

PlaceItem::~PlaceItem()
{
}

QHash<int, QByteArray> PlaceItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TitleRole] = "title";
    names[LatitudeRole] = "latitude";
    names[LongitudeRole] = "longitude";
    names[TypeRole] = "type";
    names[CountryRole] = "country";
    names[CityRole] = "city";
    names[AddressRole] = "address";
    names[IconRole] = "icon";
    names[DistanceRole] = "distance";
    names[IDRole] = "placeId";
    return names;
}

const QString& PlaceItem::title() const
{
    return m_title;
}

const QString& PlaceItem::latitude() const
{
    return m_latitude;
}

const QString& PlaceItem::longitude() const
{
    return m_longitude;
}

const QString& PlaceItem::type() const
{
    return m_type;
}

const QString& PlaceItem::country() const
{
    return m_country;
}

const QString& PlaceItem::city() const
{
    return m_city;
}

const QString& PlaceItem::address() const
{
    return m_address;
}

const QString& PlaceItem::icon() const
{
    return m_icon;
}

const QString& PlaceItem::distance() const
{
    return m_distance;
}

QString PlaceItem::id() const
{
    return m_placeId;
}

QVariant PlaceItem::key() const
{
//    QDateTime dt = QDateTime::fromString(m_time, "dd.MM.yyyy hh:mm:ss");
//    QVariant v;
//    v.setValue(dt);
//    return v;
    return QVariant();
}

QVariant PlaceItem::data(int role) const
{
    switch(role) {
    case TitleRole:
        return title();
        break;
    case LatitudeRole:
        return latitude();
        break;
    case LongitudeRole:
        return longitude();
        break;
    case TypeRole:
        return type();
        break;
    case CountryRole:
        return country();
        break;
    case CityRole:
        return city();
        break;
    case AddressRole:
        return address();
        break;
    case IconRole:
        return icon();
        break;
    case DistanceRole:
        return distance();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}
