#include "place.h"
#include "utils/coresettings.h"

class PlaceData : public QSharedData
{
public:
    PlaceData();
    PlaceData(const PlaceData& other);
    ~PlaceData();
    QString placeId;    //<!
    QString title;      //<!
    QString latitude;   //<!
    QString longitude;  //<!
    QString type;       //<!
    QString country;    //<!
    QString city;       //<!
    QString address;    //<!
    QString created;    //<!
    QString checkins;   //<!
    QString updated;    //<!
    QString icon;
    QString accountId;  //<!
    QString distance;
};

PlaceData::PlaceData()
    : QSharedData()
{
}

PlaceData::PlaceData(const PlaceData& other)
    : QSharedData(other),
      placeId(other.placeId),
      title(other.title),
      latitude(other.latitude),
      longitude(other.longitude),
      type(other.type),
      country(other.country),
      city(other.city),
      address(other.address),
      created(other.created),
      checkins(other.checkins),
      updated(other.updated),
      icon(other.icon),
      accountId(other.accountId),
      distance(other.distance)
{
}

PlaceData::~PlaceData()
{
}

Place::Place()
{
    d = new PlaceData;
}

Place::Place(const Place &src)
    : d(src.d)
{
}

Place &Place::operator=(const Place &rhs)
{
    d = rhs.d;
    return *this;
}

Place::~Place()
{
}
/*
QDomElement Place::toQDomElement(QDomDocument& doc) const
{
    return Place::toQDomElement(doc, *this);
}

QDomElement Place::toQDomElement(QDomDocument& doc, const Place& src)
{
    QDomElement ret = doc.createElement(NODE_PLACE_ROOT);

    // ID
    QDomElement node = doc.createElement(NODE_PLACE_ID);
    QDomText t = doc.createTextNode(src.placeId());
    ret.appendChild(node);
    node.appendChild(t);

    //title
    node = doc.createElement(NODE_PLACE_TITLE);
    t = doc.createTextNode(src.title());
    ret.appendChild(node);
    node.appendChild(t);

    //latitude
    node = doc.createElement(NODE_PLACE_LATITUDE);
    t = doc.createTextNode(src.latitude());
    ret.appendChild(node);
    node.appendChild(t);

    //longitude
    node = doc.createElement(NODE_PLACE_LONGITUDE);
    t = doc.createTextNode(src.longitude());
    ret.appendChild(node);
    node.appendChild(t);

    //type
    node = doc.createElement(NODE_PLACE_TYPE);
    t = doc.createTextNode(src.type());
    ret.appendChild(node);
    node.appendChild(t);

    //country
    node = doc.createElement(NODE_PLACE_COUNTRY);
    t = doc.createTextNode(src.country());
    ret.appendChild(node);
    node.appendChild(t);

    //city
    node = doc.createElement(NODE_PLACE_CITY);
    t = doc.createTextNode(src.city());
    ret.appendChild(node);
    node.appendChild(t);

    //address
    node = doc.createElement(NODE_PLACE_ADDRESS);
    t = doc.createTextNode(src.address());
    ret.appendChild(node);
    node.appendChild(t);

    //created
    node = doc.createElement(NODE_PLACE_CREATED);
    t = doc.createTextNode(src.created());
    ret.appendChild(node);
    node.appendChild(t);

    //checkins
    node = doc.createElement(NODE_PLACE_CHECKINS);
    t = doc.createTextNode(src.checkins());
    ret.appendChild(node);
    node.appendChild(t);

    //updated
    node = doc.createElement(NODE_PLACE_UPDATED);
    t = doc.createTextNode(src.updated());
    ret.appendChild(node);
    node.appendChild(t);

    return ret;
}

Place Place::fromQDomElement(const QDomElement& fr, const QString& accountId)
{
    Place ret = Place();

    ret.setPlaceId(fr.elementsByTagName(NODE_PLACE_ID).at(0).firstChild().toText().data());
    ret.setTitle(fr.elementsByTagName(NODE_PLACE_TITLE).at(0).firstChild().toText().data());
    ret.setLatitude(fr.elementsByTagName(NODE_PLACE_LATITUDE).at(0).firstChild().toText().data());
    ret.setLongitude(fr.elementsByTagName(NODE_PLACE_LONGITUDE).at(0).firstChild().toText().data());
    ret.setType(fr.elementsByTagName(NODE_PLACE_TYPE).at(0).firstChild().toText().data());
    ret.setCreated(fr.elementsByTagName(NODE_PLACE_CREATED).at(0).firstChild().toText().data());
    QDomNodeList country = fr.elementsByTagName(NODE_PLACE_COUNTRY);
    if (!country.isEmpty())
        ret.setCountry(country.at(0).firstChild().toText().data());
    else
        ret.setCountry("");
    QDomNodeList city = fr.elementsByTagName(NODE_PLACE_CITY);
    if (!city.isEmpty())
        ret.setCity(city.at(0).firstChild().toText().data());
    else
        ret.setCity("");
    QDomNodeList address = fr.elementsByTagName(NODE_PLACE_ADDRESS);
    if (!address.isEmpty())
        ret.setAddress(address.at(0).firstChild().toText().data());
    else
        ret.setAddress("");
    QDomNodeList checkins = fr.elementsByTagName(NODE_PLACE_CHECKINS);
    if (!checkins.isEmpty())
        ret.setCheckins(checkins.at(0).firstChild().toText().data());
    else
        ret.setCheckins("0");
    QDomNodeList updated = fr.elementsByTagName(NODE_PLACE_UPDATED);
    if (!updated.isEmpty())
        ret.setUpdated(updated.at(0).firstChild().toText().data());
    else
        ret.setUpdated("");
    ret.setAccountId(accountId);

    return ret;
}
*/
CheckinList Place::getCheckinList() const
{
    CheckinList ret;
    QFile file(CoreSettings::getAccountDir(this->accountId()) + QString(FILE_PLACE_CHECKINS_DATA).arg(this->placeId()));
    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): " << file.fileName();
    if (file.open(QIODevice::ReadOnly) == false)
        return ret;

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    // get list updated time
//    QDomElement root = doc.elementsByTagName(NODE_PLACE_CHECKINS_ROOT).at(0).toElement();
//    QDateTime time_create = QDateTime::fromString(root.attribute(ATTR_REFRESH, ""), DATE_TIME_FORMAT);
//    ret.setTimeCreate(time_create);

    QDomNodeList lst = doc.elementsByTagName(NODE_PHOTO_COMMENT_ROOT);

    qDebug() << "Start load of " << lst.length() << " checkins for " << this->placeId();
    for (uint i = 0; i < lst.length(); i++) {
        ret.append(Checkin::fromQDomElement(lst.at(i).toElement(), this->accountId(), this->placeId()));
    }

    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): loaded " << ret.length() << " comments";
    return ret;
}

void Place::setCheckinList(const CheckinList& list, const QString& accountId,  const QString& placeId)
{
    // store place lists into cache
    QDomDocument out("MyDoc");
    QDomElement root = out.createElement(NODE_PLACE_CHECKINS_ROOT);
    //root.setAttribute(ATTR_REFRESH, list.timeCreate().toString(DATE_TIME_FORMAT));
    out.appendChild(root);

    for (int i = 0; i < list.length(); i++) {
        Checkin curPh = list.at(i);
        root.appendChild(Checkin::toQDomElement(out, curPh));
    }

    QDir dir;

    dir.setPath(dir.cleanPath(CoreSettings::getAccountDir(accountId)));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QFile file(dir.absolutePath() + "/" + QString(FILE_PLACE_CHECKINS_DATA).arg(placeId));
    qDebug() << __FILE__ << ":" << __LINE__ << ": " << file.fileName();
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
        qDebug() << "can't open file: " + file.errorString();
    }
    //file.write(output.toUtf8());
    file.write(out.toByteArray(1));
    file.close();
}


// Getters
const QString Place::placeId() const { return d->placeId; }
const QString Place::title() const { return d->title; }
const QString Place::latitude() const { return d->latitude; }
const QString Place::longitude() const { return d->longitude; }
const QString Place::type() const { return d->type; }
const QString Place::country() const { return d->country; }
const QString Place::city() const { return d->city; }
const QString Place::address() const { return d->address; }
const QString Place::created() const { return d->created; }
const QString Place::checkins() const { return d->checkins; }
const QString Place::updated() const { return d->updated; }
const QString Place::icon() const { return d->icon; }
const QString Place::accountId() const { return d->accountId; }
const QString Place::distance() const { return d->distance; }

// Setters
void Place::setPlaceId(const QString& v) { d->placeId = v; }
void Place::setTitle(const QString& v) { d->title = v; }
void Place::setLatitude(const QString& v) { d->latitude = v; }
void Place::setLongitude(const QString& v) { d->longitude = v; }
void Place::setType(const QString& v) { d->type = v; }
void Place::setCountry(const QString& v) { d->country = v; }
void Place::setCity(const QString& v) { d->city = v; }
void Place::setAddress(const QString& v) { d->address = v; }
void Place::setCreated(const QString& v) { d->created = v; }
void Place::setCheckins(const QString& v) { d->checkins = v; }
void Place::setUpdated(const QString& v) { d->updated = v; }
void Place::setIcon(const QString& v) { d->icon = v; }
void Place::setAccountId(const QString& v) { d->accountId = v; }
void Place::setDistance(const QString& v) { d->distance = v; }
