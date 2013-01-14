#include "qeventfeed.h"

#include <QFile>

#include "utils/coresettings.h"

class QEventFeedData : public QSharedData
{
public:
    QEventFeedData();
    QEventFeedData(const QEventFeedData& other);
    ~QEventFeedData();

    /**
      \brief The name of event's author.
      */
    QString ownerName;

    /**
      \brief The contents of the event or comment.
      */
    QString text;

    /**
      \brief The event time creation.
      */
    QDateTime created;

    /**
      \brief Attached object.
      */
    QAttachment attach;
};

QEventFeedData::QEventFeedData()
    : QSharedData()
{
}

QEventFeedData::QEventFeedData(const QEventFeedData& other)
    : QSharedData(other)
{
    attach = other.attach;
    created = other.created;
    ownerName = other.ownerName;
    text = other.text;
}

QEventFeedData::~QEventFeedData()
{
}


QEventFeed::QEventFeed()
    : BaseObject()
{
    d = new QEventFeedData;
}

QEventFeed::QEventFeed(const QEventFeed &src)
    : BaseObject(src), d(src.d)
{
}

QEventFeed::~QEventFeed()
{
}

QEventFeed& QEventFeed::operator =(QEventFeed const& src)
{
    BaseObject::operator =(src);
    d = src.d;
    return *this;
}

QString QEventFeed::typeToString(const FeedType type)
{
    QString name;

    switch(type)
    {
    case photoFeed:
        name = "image";
        break;
    case messageFeed:
        name = "message";
        break;
    case commentFeed:
        name = "comment";
        break;
    case videoFeed:
        name = "video";
        break;
    case audioFeed:
        name = "audio";
        break;
    case likeFeed:
        name = "like";
        break;
    case tagFeed:
        name = "tag";
        break;
    case friendFeed:
        name = "friend";
    }

    return name;
}

QString QEventFeed::getKey(QString accountId, FeedType type)
{
    QString name = QEventFeed::typeToString(type);
    return QString("feed_%1_%2").arg(accountId).arg(name);
}

QDomElement QEventFeed::toQDomElement(QDomDocument &doc) const
{
    QDomElement ret = doc.createElement(NODE_EVENT_ROOT);

    // owner ID
    QDomElement node = doc.createElement(NODE_EVENT_OWNERID);
    QDomText t = doc.createTextNode(this->ownerId());
    ret.appendChild(node);
    node.appendChild(t);

    // owner name
    node = doc.createElement(NODE_EVENT_OWNER_NAME);
    t = doc.createTextNode(this->ownerName());
    ret.appendChild(node);
    node.appendChild(t);

    // text
    if (!this->text().isEmpty())
    {
        node = doc.createElement(NODE_EVENT_TEXT);
        t = doc.createTextNode(this->text());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // created
    node = doc.createElement(NODE_EVENT_CREATED);
    t = doc.createTextNode(this->created().toString(DATE_TIME_FORMAT));
    ret.appendChild(node);
    node.appendChild(t);

    // attachment
    ret.appendChild(this->attach().toQDomElement(doc));

    return ret;
}

QEventFeed QEventFeed::fromQDomElement(const QDomElement& fr, const QString& srvId, const QString& accountId, const FeedType)
{
    QEventFeed ret;
    ret.setAccountId(accountId);
    ret.setServiceId(srvId);

    if (fr.elementsByTagName(NODE_EVENT_OWNERID).count() > 0)
        ret.setOwnerId(fr.elementsByTagName(NODE_EVENT_OWNERID).at(0).firstChild().toText().data());

    if (fr.elementsByTagName(NODE_EVENT_OWNER_NAME).count() > 0)
        ret.setOwnerName(fr.elementsByTagName(NODE_EVENT_OWNER_NAME).at(0).firstChild().toText().data());

    if (fr.elementsByTagName(NODE_EVENT_TEXT).count() > 0)
        ret.setText(fr.elementsByTagName(NODE_EVENT_TEXT).at(0).firstChild().toText().data());

    if (fr.elementsByTagName(NODE_EVENT_CREATED).count() > 0)
        ret.setCreated(QDateTime::fromString(fr.elementsByTagName(NODE_EVENT_CREATED).at(0).firstChild().toText().data(), DATE_TIME_FORMAT));

    ret.setAttach(QAttachment::fromQDomElement(fr.firstChildElement(NODE_ATTACH_ROOT), srvId, accountId));

    ret.setAccountId(accountId);

    return ret;
}

Photo QEventFeed::toPhoto() const
{
    Photo ret;
    ret.setServiceId(this->serviceId());
    ret.setAccountId(this->accountId());
    ret.setOwnerId(this->ownerId());
    ret.setAlbumId(this->attach().albumId());
    ret.setIconUrl(this->attach().iconUrl());
    ret.setPhotoUrl(this->attach().objectUrl());
    ret.setTime_create(this->created().toString(DATE_TIME_FORMAT));
    ret.setPhotoId(this->attach().id());

    return ret;
}

// Getters
const QString QEventFeed::ownerName() const { return d->ownerName; }
const QString QEventFeed::text() const { return d->text; }
const QDateTime QEventFeed::created() const { return d->created; }
const QAttachment QEventFeed::attach() const { return d->attach; }

// Setters
void QEventFeed::setOwnerName(const QString& v) { d->ownerName = v; }
void QEventFeed::setText(const QString& v) { d->text = v; }
void QEventFeed::setCreated(const QDateTime& v) { d->created = v; }
void QEventFeed::setAttach(const QAttachment& v) { d->attach = v; }
