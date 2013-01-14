#include "checkin.h"
#include <QSharedData>

class CheckinData : public QSharedData
{
public:
    CheckinData();
    CheckinData(const CheckinData& other);
    ~CheckinData();
    QString checkinId;    //<!
    QString ownerId;    //<!
    QString ownerName;    //<!
    QString ownerPhoto;    //<!
    QString date;      //<!
    QString latitude;   //<!
    QString longitude;  //<!
    QString text;    //<!
    QString placeId;    //<!
    QString placeCountry;    //<!
    QString placeCity;       //<!
    QString placeAddress;    //<!
    QString placeType;       //<!
    QString placeTitle;       //<!
    QString placeIcon;    //<!
    QString accountId;  //<!
};

CheckinData::CheckinData()
    : QSharedData()
{
}

CheckinData::CheckinData(const CheckinData& other)
    : QSharedData(other),
      checkinId(other.checkinId),
      ownerId(other.ownerId),
      ownerName(other.ownerName),
      ownerPhoto(other.ownerPhoto),
      latitude(other.latitude),
      longitude(other.longitude),
      text(other.text),
      placeId(other.placeId),
      placeCountry(other.placeCountry),
      placeCity(other.placeCity),
      placeAddress(other.placeAddress),
      placeType(other.placeType),
      placeTitle(other.placeTitle),
      placeIcon(other.placeIcon),
      accountId(other.accountId)
{
}

CheckinData::~CheckinData()
{
}

Checkin::Checkin()
{
    d = new CheckinData;
}

Checkin::Checkin(const Checkin &src)
    : d(src.d)
{
}
Checkin &Checkin::operator=(const Checkin &rhs)
{
    d = rhs.d;
    return *this;
}

Checkin::~Checkin()
{
}

QDomElement Checkin::toQDomElement(QDomDocument& doc) const
{
    return Checkin::toQDomElement(doc, *this);
}

QDomElement Checkin::toQDomElement(QDomDocument& doc, const Checkin& src)
{
    QDomElement ret = doc.createElement(NODE_CHECKIN_ROOT);

    // ID
    QDomElement node = doc.createElement(NODE_CHECKIN_ID);
    QDomText t = doc.createTextNode(src.checkinId());
    ret.appendChild(node);
    node.appendChild(t);

    //uid
    node = doc.createElement(NODE_CHECKIN_OWNER_ID);
    t = doc.createTextNode(src.ownerId());
    ret.appendChild(node);
    node.appendChild(t);

    //date
    node = doc.createElement(NODE_CHECKIN_DATE);
    t = doc.createTextNode(src.date());
    ret.appendChild(node);
    node.appendChild(t);

    //latitude
    node = doc.createElement(NODE_CHECKIN_LATITUDE);
    t = doc.createTextNode(src.latitude());
    ret.appendChild(node);
    node.appendChild(t);

    //longitude
    node = doc.createElement(NODE_CHECKIN_LONGITUDE);
    t = doc.createTextNode(src.longitude());
    ret.appendChild(node);
    node.appendChild(t);

    //placeID
    node = doc.createElement(NODE_CHECKIN_PLACE_ID);
    t = doc.createTextNode(src.placeId());
    ret.appendChild(node);
    node.appendChild(t);

    //place type
    node = doc.createElement(NODE_CHECKIN_PLACE_TYPE);
    t = doc.createTextNode(src.placeType());
    ret.appendChild(node);
    node.appendChild(t);

    //place title
    node = doc.createElement(NODE_CHECKIN_PLACE_TITLE);
    t = doc.createTextNode(src.placeTitle());
    ret.appendChild(node);
    node.appendChild(t);

    //place country
    node = doc.createElement(NODE_CHECKIN_PLACE_COUNTRY);
    t = doc.createTextNode(src.placeCountry());
    ret.appendChild(node);
    node.appendChild(t);

    //place city
    node = doc.createElement(NODE_CHECKIN_PLACE_CITY);
    t = doc.createTextNode(src.placeCity());
    ret.appendChild(node);
    node.appendChild(t);

    //place address
    node = doc.createElement(NODE_CHECKIN_PLACE_ADDRESS);
    t = doc.createTextNode(src.placeAddress());
    ret.appendChild(node);
    node.appendChild(t);

    //pace icon
    node = doc.createElement(NODE_CHECKIN_PLACE_ICON);
    t = doc.createTextNode(src.placeIcon());
    ret.appendChild(node);
    node.appendChild(t);

    return ret;
}

Checkin Checkin::fromQDomElement(const QDomElement& fr, const QString& accountId, const QString& placeId)
{
    Checkin ret = Checkin();

    ret.setCheckinId(fr.elementsByTagName(NODE_CHECKIN_ID).at(0).firstChild().toText().data());
    ret.setOwnerId(fr.elementsByTagName(NODE_CHECKIN_OWNER_ID).at(0).firstChild().toText().data());
    ret.setDate(fr.elementsByTagName(NODE_CHECKIN_DATE).at(0).firstChild().toText().data());
    ret.setLatitude(fr.elementsByTagName(NODE_CHECKIN_LATITUDE).at(0).firstChild().toText().data());
    ret.setLongitude(fr.elementsByTagName(NODE_CHECKIN_LONGITUDE).at(0).firstChild().toText().data());
    QDomNodeList country = fr.elementsByTagName(NODE_CHECKIN_PLACE_COUNTRY);
    if (!country.isEmpty())
        ret.setPlaceCountry(country.at(0).firstChild().toText().data());
    else
        ret.setPlaceCountry("");
    QDomNodeList city = fr.elementsByTagName(NODE_CHECKIN_PLACE_CITY);
    if (!city.isEmpty())
        ret.setPlaceCity(city.at(0).firstChild().toText().data());
    else
        ret.setPlaceCity("");
    QDomNodeList address = fr.elementsByTagName(NODE_CHECKIN_PLACE_ADDRESS);
    if (!address.isEmpty())
        ret.setPlaceAddress(address.at(0).firstChild().toText().data());
    else
        ret.setPlaceAddress("");
    QDomNodeList checkins = fr.elementsByTagName(NODE_CHECKIN_PLACE_ICON);
    if (!checkins.isEmpty())
        ret.setPlaceIcon(checkins.at(0).firstChild().toText().data());
    else
        ret.setPlaceIcon("");
    ret.setAccountId(accountId);
    ret.setPlaceId(placeId);

    return ret;
}

// Getters
const QString Checkin::checkinId() const { return d->checkinId; }
const QString Checkin::ownerId() const { return d->ownerId; }
const QString Checkin::ownerName() const { return d->ownerName; }
const QString Checkin::ownerPhoto() const { return d->ownerPhoto; }
const QString Checkin::date() const { return d->date; }
const QString Checkin::latitude() const { return d->latitude; }
const QString Checkin::longitude() const { return d->longitude; }
const QString Checkin::text() const { return d->text; }
const QString Checkin::placeId() const { return d->placeId; }
const QString Checkin::placeCountry() const { return d->placeCountry; }
const QString Checkin::placeCity() const { return d->placeCity; }
const QString Checkin::placeAddress() const { return d->placeAddress; }
const QString Checkin::placeType() const { return d->placeType; }
const QString Checkin::placeTitle() const { return d->placeTitle; }
const QString Checkin::placeIcon() const { return d->placeIcon; }
const QString Checkin::accountId() const { return d->accountId; }

// Setters
void Checkin::setCheckinId(const QString& v) { d->checkinId = v; }
void Checkin::setOwnerId(const QString& v) { d->ownerId = v; }
void Checkin::setOwnerName(const QString& v) { d->ownerName = v; }
void Checkin::setOwnerPhoto(const QString& v) { d->ownerPhoto = v; }
void Checkin::setDate(const QString& v) { d->date = v; }
void Checkin::setLatitude(const QString& v) { d->latitude = v; }
void Checkin::setLongitude(const QString& v) { d->longitude = v; }
void Checkin::setText(const QString& v) { d->text = v; }
void Checkin::setPlaceId(const QString& v) { d->placeId = v; }
void Checkin::setPlaceCountry(const QString& v) { d->placeCountry = v; }
void Checkin::setPlaceCity(const QString& v) { d->placeCity = v; }
void Checkin::setPlaceAddress(const QString& v) { d->placeAddress = v; }
void Checkin::setPlaceType(const QString& v) { d->placeType = v; }
void Checkin::setPlaceTitle(const QString& v) { d->placeTitle = v; }
void Checkin::setPlaceIcon(const QString& v) { d->placeIcon = v; }
void Checkin::setAccountId(const QString& v) { d->accountId = v; }

