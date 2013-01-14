#include "photo.h"
#include "utils/coresettings.h"

#ifdef DEBUG
#include <QDebug>
#endif

class PhotoData : public QSharedData
{
public:
    PhotoData();
    PhotoData(const PhotoData& other);
    ~PhotoData();

    QString albumId;     //<! album ID from service
    QString time_create; //<! creation time
    QString photo_url;   //<! path to large image
    QString description; //<! photo description
};

PhotoData::PhotoData()
    : QSharedData()
{
}

PhotoData::PhotoData(const PhotoData& other)
    : QSharedData(other),
      albumId(other.albumId),
      time_create(other.time_create),
      photo_url(other.photo_url),
      description(other.description)
{
}

PhotoData::~PhotoData()
{
}


Photo::Photo()
    :BaseObject()
{
    d = new PhotoData;
}

Photo::Photo(const Photo &src)
    : BaseObject(src), d(src.d)
{
}

Photo::~Photo()
{
}

QDomElement Photo::toQDomElement(QDomDocument& doc) const
{
    return Photo::toQDomElement(doc, *this);
}

QDomElement Photo::toQDomElement(QDomDocument& doc, const Photo& src)
{
    QDomElement ret = doc.createElement(NODE_PHOTO_ROOT);

    // ID
    QDomElement node = doc.createElement(NODE_PHOTO_ID);
    QDomText t = doc.createTextNode(src.photoId());
    ret.appendChild(node);
    node.appendChild(t);

    //time create
    node = doc.createElement(NODE_PHOTO_CREATE);
    t = doc.createTextNode(src.time_create());
    ret.appendChild(node);
    node.appendChild(t);

    //icon url
    node = doc.createElement(NODE_PHOTO_ICON_URL);
    t = doc.createTextNode(src.iconUrl());
    ret.appendChild(node);
    node.appendChild(t);

    //image url
    node = doc.createElement(NODE_PHOTO_IMAGE_URL);
    t = doc.createTextNode(src.photoUrl());
    ret.appendChild(node);
    node.appendChild(t);

    //description
    node = doc.createElement(NODE_PHOTO_DESCRIPTION);
    t = doc.createTextNode(src.description());
    ret.appendChild(node);
    node.appendChild(t);

    return ret;
}

Photo Photo::fromQDomElement(const QDomElement& fr, const QString& srvId, const QString& accId, const QString& frId, const QString& albId)
{
    Photo ret = Photo();

    ret.setPhotoId(fr.elementsByTagName(NODE_PHOTO_ID).at(0).firstChild().toText().data());
    ret.setIconUrl(fr.elementsByTagName(NODE_PHOTO_ICON_URL).at(0).firstChild().toText().data());
    ret.setTime_create(fr.elementsByTagName(NODE_PHOTO_CREATE).at(0).firstChild().toText().data());
    ret.setPhotoUrl(fr.elementsByTagName(NODE_PHOTO_IMAGE_URL).at(0).firstChild().toText().data());
    QDomNodeList description = fr.elementsByTagName(NODE_PHOTO_DESCRIPTION);
    if (!description.isEmpty())
        ret.setDescription(description.at(0).firstChild().toText().data());
    else
        ret.setDescription("");
    ret.setAccountId(accId);
    ret.setOwnerId(frId);
    ret.setAlbumId(albId);
    ret.setServiceId(srvId);

    return ret;
}

Photo& Photo::operator =(Photo const& src)
{
    BaseObject::operator =(src);
    d = src.d;
    return *this;
}

PhotoCommentList Photo::getPhotoComments() const
{
    PhotoCommentList ret;
    QFile file(CoreSettings::getAccountDir(this->accountId()) + QString(FILE_PHOTO_COMMENTS_DATA).arg(this->ownerId(), this->albumId(), this->photoId()));
#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << file.fileName();
#endif
    if (file.open(QIODevice::ReadOnly) == false)
        return ret;

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    // get list updated time
    QDomElement root = doc.elementsByTagName(NODE_PHOTO_COMMENTS_ROOT).at(0).toElement();
    QDateTime time_create = QDateTime::fromString(root.attribute(ATTR_REFRESH, ""), DATE_TIME_FORMAT);
    ret.setTimeCreate(time_create);

    QDomNodeList lst = doc.elementsByTagName(NODE_PHOTO_COMMENT_ROOT);

#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Start load of " << lst.length() << " comments for " << this->photoId();
#endif
    for (uint i = 0; i < lst.length(); i++) {
        ret.append(PhotoComment::fromQDomElement(lst.at(i).toElement(), this->accountId(), this->ownerId(), this->albumId(), this->photoId()));
    }

#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "loaded " << ret.length() << " comments";
#endif
    return ret;
}

void Photo::setPhotoComments(const PhotoCommentList& list, const QString& accountId, const QString& friendId, const QString& albumId, const QString& photoId)
{
    // store photo lists into cache
    QDomDocument out("MyDoc");
    QDomElement root = out.createElement(NODE_PHOTO_COMMENTS_ROOT);
    root.setAttribute(ATTR_REFRESH, list.timeCreate().toString(DATE_TIME_FORMAT));
    out.appendChild(root);

    for (int i = 0; i < list.length(); i++) {
        PhotoComment curPh = list.at(i);
        root.appendChild(PhotoComment::toQDomElement(out, curPh));
    }

    QDir dir;

    dir.setPath(dir.cleanPath(CoreSettings::getAccountDir(accountId)));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QFile file(dir.absolutePath() + "/" + QString(FILE_PHOTO_COMMENTS_DATA).arg(friendId, albumId, photoId));
#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << file.fileName();
#endif
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
#ifdef DEBUG
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "can't open file: " + file.errorString();
#endif
    }
    //file.write(output.toUtf8());
    file.write(out.toByteArray(1));
    file.close();
}

PhotoList Photo::loadPhotoList(const QString& srvId, const QString& accountId, const QString& ownerId, const QString& albumId)
{
    PhotoList ret;
    QFile file(CoreSettings::getAccountDir(accountId) + QString(FILE_PHOTOS_DATA).arg(ownerId, albumId));
#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << file.fileName();
#endif
    if (file.open(QIODevice::ReadOnly) == false)
        return ret;

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    // get list updated time
    QDomElement root = doc.elementsByTagName(NODE_PHOTOS_ROOT).at(0).toElement();
    QDateTime time_create = QDateTime::fromString(root.attribute(ATTR_REFRESH, ""), DATE_TIME_FORMAT);
    ret.setTimeCreate(time_create);

    QDomNodeList lst = doc.elementsByTagName(NODE_PHOTO_ROOT);

#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Start load of " << lst.length() << " photos for " << albumId;
#endif
    for (uint i = 0; i < lst.length(); i++) {
        ret.append(Photo::fromQDomElement(lst.at(i).toElement(), srvId, accountId, ownerId, albumId));
    }

    return ret;
}

QString Photo::getKey(const PhotoList& photos)
{
    if (photos.length() > 0)
        return QString("photos_%1_%2").arg(photos.at(0).accountId()).arg(photos.at(0).albumId());
    else
        return QString("photos");
}

QString Photo::getKey(const Album& album)
{
    return QString("photos_%1_%2").arg(album.accountId()).arg(album.albumId());
}

QString Photo::getKey(const Photo& val)
{
    return QString("photos_%1_%2").arg(val.accountId()).arg(val.albumId());
}

const QString Photo::icon() const
{
    return generateFileName(CoreSettings::getAlbumsIconDir(this->serviceId()));
}

const QString Photo::photo() const
{
    return generateFileName(CoreSettings::getPhotoDir(this->serviceId()));
}

// Getters
const QString Photo::photoId() const { return objectId(); }
const QString Photo::albumId() const { return d->albumId; }
const QString Photo::time_create() const { return d->time_create; }
const QString Photo::photoUrl() const { return d->photo_url; }
const QString Photo::description() const { return d->description; }

// Setters
void Photo::setPhotoId(const QString& v) { setObjectId(v); }
void Photo::setAlbumId(const QString& v) { d->albumId = v; }
void Photo::setTime_create(const QString& v) { d->time_create = v; }
void Photo::setPhotoUrl(const QString& v) { d->photo_url = v; }
void Photo::setDescription(const QString& v) { d->description = v; }

QDataStream& operator<<(QDataStream& out, const Photo& p)
{
    out << p.serviceId()
            << p.accountId()
            << p.ownerId()
            << p.objectId()
            << p.iconUrl()
            << p.d->albumId
            << p.d->time_create
            << p.d->photo_url
            << p.d->description;
    return out;
}

QDataStream& operator>>(QDataStream& in, Photo& p)
{
    p.d.detach();

    QString srvId, accId, ownId, objId, url;

    in >> srvId
            >> accId
            >> ownId
            >> objId
            >> url
            >> p.d->albumId
            >> p.d->time_create
            >> p.d->photo_url
            >> p.d->description;

    p.setServiceId(srvId);
    p.setAccountId(accId);
    p.setOwnerId(ownId);
    p.setObjectId(objId);
    p.setIconUrl(url);

    return in;
}
