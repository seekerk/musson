#include "event.h"
#include "utils/coresettings.h"
#include <QSharedData>

class EventData : public QSharedData
{
public:
    EventData();
    EventData(const EventData& other);
    ~EventData();
    QString title;        //<!
    QString description;  //<!
    QString startTime;    //<!
    QString endTime;      //<!
    QString updateTime;   //<!
    QString location;     //<!
    QString privacy;      //<!
    QString creator;      //<!
    QString icon;         //<!
    QString rsvpStatus;   //<!
};

EventData::EventData()
    : QSharedData()
{
}

EventData::EventData(const EventData& other)
    : QSharedData(other),
    title(other.title),
    description(other.description),
    startTime(other.startTime),
    endTime(other.endTime),
    updateTime(other.updateTime),
    location(other.location),
    privacy(other.privacy),
    creator(other.creator),
    icon(other.icon),
    rsvpStatus(other.rsvpStatus)
{
}

EventData::~EventData()
{
}

Event::Event()
    :BaseObject()
{
    d = new EventData;
}

Event::Event(const Event &src)
    :BaseObject(src), d(src.d)
{
}
Event &Event::operator=(const Event &rhs)
{
    d = rhs.d;
    return *this;
}

Event::~Event()
{
}

QDomElement Event::toQDomElement(QDomDocument& doc) const
{
    return Event::toQDomElement(doc, *this);
}

QDomElement Event::toQDomElement(QDomDocument& doc, const Event& src)
{
    QDomElement ret = doc.createElement(NODE_EVENT_ROOT);

    // ID
    QDomElement node;
    QDomText t;

    // ID
    if (!src.objectId().isEmpty()) {
        node = doc.createElement(NODE_EVENT_ID);
        t = doc.createTextNode(src.objectId());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //uid
    if (!src.ownerId().isEmpty()) {
        node = doc.createElement(NODE_EVENT_OWNER_ID);
        t = doc.createTextNode(src.ownerId());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //title
    if (!src.title().isEmpty()) {
        node = doc.createElement(NODE_EVENT_TITLE);
        t = doc.createTextNode(src.title());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //description
    if (!src.description().isEmpty()) {
        node = doc.createElement(NODE_EVENT_DESCRIPTION);
        t = doc.createTextNode(src.description());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //start_time
    if (!src.startTime().isEmpty()) {
        node = doc.createElement(NODE_EVENT_START_TIME);
        t = doc.createTextNode(src.startTime());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //end_time
    if (!src.endTime().isEmpty()) {
        node = doc.createElement(NODE_EVENT_END_TIME);
        t = doc.createTextNode(src.endTime());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //update_time
    if (!src.updateTime().isEmpty()) {
        node = doc.createElement(NODE_EVENT_UPDATE_TIME);
        t = doc.createTextNode(src.updateTime());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //location
    if (!src.location().isEmpty()) {
        node = doc.createElement(NODE_EVENT_LOCATION);
        t = doc.createTextNode(src.location());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //creator
    if (!src.creator().isEmpty()) {
        node = doc.createElement(NODE_EVENT_CREATOR);
        t = doc.createTextNode(src.creator());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //privacy
    if (!src.privacy().isEmpty()) {
        node = doc.createElement(NODE_EVENT_PRIVACY);
        t = doc.createTextNode(src.privacy());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //icon_url
    if (!src.iconUrl().isEmpty()) {
        node = doc.createElement(NODE_EVENT_ICON_URL);
        t = doc.createTextNode(src.iconUrl());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //rsvp_status
    if (!src.rsvpStatus().isEmpty()) {
        node = doc.createElement(NODE_EVENT_RSVP_STATUS);
        t = doc.createTextNode(src.rsvpStatus());
        ret.appendChild(node);
        node.appendChild(t);
    }

    return ret;
}

Event Event::fromQDomElement(const QDomElement& fr, const QString& srvcId, const QString& accountId, const QString& friendId)
{
    Event ret = Event();

    ret.setObjectId(fr.elementsByTagName(NODE_EVENT_ID).at(0).firstChild().toText().data());
    //ret.setOwnerId(fr.elementsByTagName(NODE_EVENT_OWNER_ID).at(0).firstChild().toText().data());
    ret.setTitle(fr.elementsByTagName(NODE_EVENT_TITLE).at(0).firstChild().toText().data());
    QDomNodeList description = fr.elementsByTagName(NODE_EVENT_DESCRIPTION);
    if (!description.isEmpty())
        ret.setDescription(fr.elementsByTagName(NODE_EVENT_DESCRIPTION).at(0).firstChild().toText().data());
    else
        ret.setDescription("");
    ret.setStartTime(fr.elementsByTagName(NODE_EVENT_START_TIME).at(0).firstChild().toText().data());
    QDomNodeList endTime = fr.elementsByTagName(NODE_EVENT_END_TIME);
    if (!endTime.isEmpty())
        ret.setEndTime(fr.elementsByTagName(NODE_EVENT_END_TIME).at(0).firstChild().toText().data());
    else
        ret.setEndTime("");
    ret.setUpdateTime(fr.elementsByTagName(NODE_EVENT_UPDATE_TIME).at(0).firstChild().toText().data());
    QDomNodeList location = fr.elementsByTagName(NODE_EVENT_END_TIME);
    if (!location.isEmpty())
        ret.setLocation(fr.elementsByTagName(NODE_EVENT_LOCATION).at(0).firstChild().toText().data());
    else
        ret.setLocation("");
    ret.setCreator(fr.elementsByTagName(NODE_EVENT_CREATOR).at(0).firstChild().toText().data());
    ret.setPrivacy(fr.elementsByTagName(NODE_EVENT_PRIVACY).at(0).firstChild().toText().data());
    ret.setIconUrl(fr.elementsByTagName(NODE_EVENT_ICON_URL).at(0).firstChild().toText().data());
    ret.setRsvpStatus(fr.elementsByTagName(NODE_EVENT_RSVP_STATUS).at(0).firstChild().toText().data());

    ret.setOwnerId(friendId);
    ret.setServiceId(srvcId);
    ret.setAccountId(accountId);

    return ret;
}

const QString Event::icon() const
{
    return generateFileName(CoreSettings::getEventsIconDir(this->serviceId()));
}

QString Event::deleteEventKey(const QString &accountId, const QString &eventId)
{
    return QString("deleteEvent_%1_%2").arg(accountId).arg(eventId);
}

/*bool Event::hasEvent(const QString& serviceId, const QString& objectId) const
{
    if (this->serviceId().compare(serviceId) == 0 &&
        this->objectId().compare(objectId) == 0)
        return true;

    for (int i = 0; i < d->_profiles.length(); i++) {
        if (serviceId.compare(d->_profiles.at(i).serviceId()) == 0 &&
            ownerId.compare(d->_profiles.at(i).ownerId()) == 0)
            return true;
    }

    return false;
}

EventList Event::mergeLists(EventList src1, EventList src2)
{
    if (src1.isEmpty())
        return src2;

    if (src2.isEmpty())
        return src1;

    EventList ret;

    for (int i = 0; i < src1.length(); i++) {
        Event ev = src1.at(i);
        for (int j = 0; j < src2.length(); j++) {
            Event ev2 = src2.at(j);
            if (ev2.hasEvent(ev.serviceId(), ev.objectId())) {
                ev.addProfile(ev2, false);
                src2.removeAt(j);
            }
        }
        ret.append(ev);

        if (ret.count() % 100 == 0)
            QCoreApplication::processEvents();
    }

    ret.append(src2);

    return ret;
}*/

// Getters
const QString Event::title() const { return d->title; }
const QString Event::description() const { return d->description; }
const QString Event::startTime() const { return d->startTime; }
const QString Event::endTime() const { return d->endTime; }
const QString Event::updateTime() const { return d->updateTime; }
const QString Event::location() const { return d->location; }
const QString Event::creator() const { return d->creator; }
const QString Event::privacy() const { return d->privacy; }
const QString Event::rsvpStatus() const { return d->rsvpStatus; }

// Setters
void Event::setTitle(const QString& v) { d->title = v; }
void Event::setDescription(const QString& v) { d->description = v; }
void Event::setStartTime(const QString& v) { d->startTime = v; }
void Event::setEndTime(const QString& v) { d->endTime = v; }
void Event::setUpdateTime(const QString& v) { d->updateTime = v; }
void Event::setLocation(const QString& v) { d->location = v; }
void Event::setCreator(const QString& v) { d->creator = v; }
void Event::setPrivacy(const QString& v) { d->privacy = v; }
void Event::setRsvpStatus(const QString& v) { d->rsvpStatus = v; }

