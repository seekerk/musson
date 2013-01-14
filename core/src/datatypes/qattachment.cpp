#include "qattachment.h"

#include "utils/coresettings.h"

#ifdef DEBUG
#include <QDebug>
#endif

class QAttachmentData : public QSharedData
{
public:
    QAttachmentData();
    QAttachmentData(const QAttachmentData& other);
    ~QAttachmentData();

    /**
      \brief The type of attachment.
      */
    QAttachment::AttachmentType type;

    /**
      \brief The name of attached object.

      Used at image, audio, link, note and video attachment.
      */
    QString name;

    /**
      \brief The album Id.

      Used at image attachment.
      */
    QString albumId;

    /**
      \brief The URL to origin object.

      Used at image, audio, link, note and video attachment.
      */
    QString objectUrl;

    /**
      \brief The video duration.

      Used at video and audio attachment.
      */
    QString duration;
};

QAttachmentData::QAttachmentData()
    : QSharedData()
{
}

QAttachmentData::QAttachmentData(const QAttachmentData& other)
    : QSharedData(other),
      type(other.type),
      name(other.name),
      albumId(other.albumId),
      objectUrl(other.objectUrl),
      duration(other.duration)
{
}

QAttachmentData::~QAttachmentData()
{
}


QAttachment::QAttachment()
    : BaseObject()
{
    d = new QAttachmentData;
    setType(QAttachment::NoteAttach);
}

QAttachment::QAttachment(const QAttachment &src)
    : BaseObject(src), d(src.d)
{
}

QAttachment::~QAttachment()
{
}

QAttachment& QAttachment::operator =(QAttachment const& src)
{
    BaseObject::operator =(src);
    d = src.d;
    return *this;
}

QDomElement QAttachment::toQDomElement(QDomDocument &doc) const
{
    QDomElement ret = doc.createElement(NODE_ATTACH_ROOT);

    // ID
    QDomElement node = doc.createElement(NODE_ATTACH_ID);
    QDomText t = doc.createTextNode(this->id());
    ret.appendChild(node);
    node.appendChild(t);

    // owner ID
    node = doc.createElement(NODE_ATTACH_OWNERID);
    t = doc.createTextNode(this->ownerId());
    ret.appendChild(node);
    node.appendChild(t);

    // type
    node = doc.createElement(NODE_ATTACH_TYPE);
    switch(this->type())
    {
    case ImageAttach:
        t = doc.createTextNode("image");
        break;
    case VideoAttach:
        t = doc.createTextNode("video");
        break;
    case AudioAttach:
        t = doc.createTextNode("audio");
        break;
    case LinkAttach:
        t = doc.createTextNode("link");
        break;
    case NoteAttach:
        t = doc.createTextNode("note");
        break;
    }
    ret.appendChild(node);
    node.appendChild(t);

    // name
    if (!this->name().isEmpty())
    {
        node = doc.createElement(NODE_ATTACH_NAME);
        t = doc.createTextNode(this->name());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // albumId
    if (!this->albumId().isEmpty())
    {
        node = doc.createElement(NODE_ATTACH_ALBUMID);
        t = doc.createTextNode(this->albumId());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // icon URL
    if (!this->iconUrl().isEmpty())
    {
        node = doc.createElement(NODE_ATTACH_ICONURL);
        t = doc.createTextNode(this->iconUrl());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // object URL
    if (!this->objectUrl().isEmpty())
    {
        node = doc.createElement(NODE_ATTACH_OBJECT);
        t = doc.createTextNode(this->objectUrl());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // duration
    if (!this->duration().isEmpty())
    {
        node = doc.createElement(NODE_ATTACH_DURATION);
        t = doc.createTextNode(this->duration());
        ret.appendChild(node);
        node.appendChild(t);
    }

    return ret;
}

QAttachment QAttachment::fromQDomElement(const QDomElement &fr, const QString &srvId, const QString &accountId)
{
    QAttachment ret;

    ret.setAccountId(accountId);
    ret.setServiceId(srvId);

    if (fr.elementsByTagName(NODE_ATTACH_ID).count() > 0)
        ret.setId(fr.elementsByTagName(NODE_ATTACH_ID).at(0).firstChild().toText().data());

    if (fr.elementsByTagName(NODE_ATTACH_ALBUMID).count() > 0)
        ret.setAlbumId(fr.elementsByTagName(NODE_ATTACH_ALBUMID).at(0).firstChild().toText().data());

    if (fr.elementsByTagName(NODE_ATTACH_OBJECT).count() > 0)
        ret.setObjectUrl(fr.elementsByTagName(NODE_ATTACH_OBJECT).at(0).firstChild().toText().data());

    if (fr.elementsByTagName(NODE_ATTACH_NAME).count() > 0)
        ret.setName(fr.elementsByTagName(NODE_ATTACH_NAME).at(0).firstChild().toText().data());

    if (fr.elementsByTagName(NODE_ATTACH_DURATION).count() > 0)
        ret.setDuration(fr.elementsByTagName(NODE_ATTACH_DURATION).at(0).firstChild().toText().data());

    if (fr.elementsByTagName(NODE_ATTACH_ICONURL).count() > 0)
        ret.setIconUrl(fr.elementsByTagName(NODE_ATTACH_ICONURL).at(0).firstChild().toText().data());

    if (fr.elementsByTagName(NODE_ATTACH_OWNERID).count() > 0)
        ret.setOwnerId( fr.elementsByTagName(NODE_ATTACH_OWNERID).at(0).firstChild().toText().data());

    if (fr.elementsByTagName(NODE_ATTACH_TYPE).count() > 0)
    {
        QString tp = fr.elementsByTagName(NODE_ATTACH_TYPE).at(0).firstChild().toText().data();
        if (tp.compare("image") == 0)
            ret.setType(ImageAttach);
        else if (tp.compare("video") == 0)
            ret.setType(VideoAttach);
        else if (tp.compare("audio") == 0)
            ret.setType(AudioAttach);
        else if (tp.compare("link") == 0)
            ret.setType(LinkAttach);
        else
            ret.setType(NoteAttach);
    }

    return ret;
}

const QString QAttachment::icon() const
{
    return generateFileName(CoreSettings::getAlbumsIconDir(this->serviceId()));
}

const QString QAttachment::image() const
{
    return generateFileName(CoreSettings::getPhotoDir(this->serviceId()), objectUrl());
}

// Getters
const QString QAttachment::id() const { return objectId(); }
QAttachment::AttachmentType QAttachment::type() const { return d->type; }
const QString QAttachment::name() const { return d->name; }
const QString QAttachment::albumId() const { return d->albumId; }
const QString QAttachment::objectUrl() const { return d->objectUrl; }
const QString QAttachment::duration() const { return d->duration; }

// Setters
void QAttachment::setId(const QString& v) { setObjectId(v); }
void QAttachment::setType(AttachmentType v) { d->type = v; }
void QAttachment::setName(const QString& v) { d->name = v; }
void QAttachment::setAlbumId(const QString& v) { d->albumId = v; }
void QAttachment::setObjectUrl(const QString& v) { d->objectUrl = v; }
void QAttachment::setDuration(const QString& v) { d->duration = v; }
