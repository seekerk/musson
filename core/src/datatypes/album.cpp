#include "album.h"
#include "utils/coresettings.h"

#ifdef DEBUG
#include <QDebug>
#endif

#include <QSharedData>

class AlbumData : public QSharedData
{
public:
    AlbumData();
    AlbumData(const AlbumData& other);
    ~AlbumData();

    QString title;       /*!< album title (optional) */
    QString description; /*!< album description (optional) */
    QString time_create; /*!< Unix timestamp when the album was created. */
    QString time_update; /*!< Unix timestamp when the album was modifed. */
    int size;            /*!< Number of images in this album */
};

AlbumData::AlbumData()
    : QSharedData()
{
}

AlbumData::AlbumData(const AlbumData& other)
    : QSharedData(other)
{
    this->description = other.description;
    this->size = other.size;
    this->time_create = other.time_create;
    this->time_update = other.time_update;
    this->title = other.title;
}



AlbumData::~AlbumData()
{
}


Album::Album()
    : BaseObject()
{
    d = new AlbumData;
}

Album::Album(const Album &src)
    : BaseObject(src), d(src.d)
{
}

Album::~Album()
{
}

QDomElement Album::toQDomElement(QDomDocument& doc) const
{
    QDomElement ret = doc.createElement(NODE_ALBUM_ROOT);

    // ID
    QDomElement node = doc.createElement(NODE_ALBUM_ID);
    QDomText t = doc.createTextNode(this->albumId());
    ret.appendChild(node);
    node.appendChild(t);

    // Name
    if (!this->title().isEmpty())
    {
        node = doc.createElement(NODE_ALBUM_TITLE);
        t = doc.createTextNode(this->title());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // description
    if (!this->description().isEmpty())
    {
        node = doc.createElement(NODE_ALBUM_DESCRIPTION);
        t = doc.createTextNode(this->description());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // size
    node = doc.createElement(NODE_ALBUM_SIZE);
    t = doc.createTextNode(QString("%1").arg(this->size()));
    ret.appendChild(node);
    node.appendChild(t);

    //time create
    if (!this->time_create().isEmpty())
    {
        node = doc.createElement(NODE_ALBUM_CREATE);
        t = doc.createTextNode(this->time_create());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //time update
    if (!this->time_update().isEmpty())
    {
        node = doc.createElement(NODE_ALBUM_UPDATE);
        t = doc.createTextNode(this->time_update());
        ret.appendChild(node);
        node.appendChild(t);
    }

    //icon url
    if (!this->iconUrl().isEmpty())
    {
        node = doc.createElement(NODE_ALBUM_ICON_URL);
        t = doc.createTextNode(this->iconUrl());
        ret.appendChild(node);
        node.appendChild(t);
    }

    return ret;
}

Album Album::fromQDomElement(const QDomElement& fr, const QString& srvId, const QString& accountId, const QString& friendId)
{
    Album ret = Album();

    ret.setAlbumId(fr.elementsByTagName(NODE_ALBUM_ID).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_ALBUM_TITLE).count() > 0)
        ret.setTitle(fr.elementsByTagName(NODE_ALBUM_TITLE).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_ALBUM_ICON_URL).count() > 0)
        ret.setIconUrl(fr.elementsByTagName(NODE_ALBUM_ICON_URL).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_ALBUM_DESCRIPTION).count() > 0)
        ret.setDescription(fr.elementsByTagName(NODE_ALBUM_DESCRIPTION).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_ALBUM_CREATE).count() > 0)
        ret.setTime_create(fr.elementsByTagName(NODE_ALBUM_CREATE).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_ALBUM_UPDATE).count() > 0)
        ret.setTime_update(fr.elementsByTagName(NODE_ALBUM_UPDATE).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_ALBUM_SIZE).count() > 0)
        ret.setSize(fr.elementsByTagName(NODE_ALBUM_SIZE).at(0).firstChild().toText().data().toInt());

    ret.setAccountId(accountId);
    ret.setOwnerId(friendId);
    ret.setServiceId(srvId);

    return ret;
}

// load album list from cache file.
PhotoList Album::getPhotoList() const
{
    return Photo::loadPhotoList(this->serviceId(), this->accountId(), this->ownerId(), this->albumId());
}

void Album::setPhotoList(PhotoList list, QString accountId, QString friendId, QString albumId)
{
    // store photo lists into cache
    QDomDocument out("MyDoc");
    QDomElement profile = out.createElement(NODE_PHOTOS_ROOT);
    profile.setAttribute(ATTR_REFRESH, list.timeCreate().toString(DATE_TIME_FORMAT));
    out.appendChild(profile);

    for (int i = 0; i < list.length(); i++) {
        Photo curPh = list.at(i);
        //qDebug() << "Image " << curPh.photoId << " stored at " << curPh.photo;
        profile.appendChild(Photo::toQDomElement(out, curPh));
    }

    QDir dir;

    dir.setPath(dir.cleanPath(CoreSettings::getAccountDir(accountId)));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QFile file(dir.absolutePath() + "/" + QString(FILE_PHOTOS_DATA).arg(friendId, albumId));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
#ifdef DEBUG
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "can't open file: " + file.errorString();
#endif
    }
    //file.write(output.toUtf8());
    file.write(out.toByteArray(1));
    file.close();
}

Album& Album::operator=(const Album& other)
{
    BaseObject::operator =(other);
    d = other.d;
    return *this;
}

const QString Album::icon() const
{
    return generateFileName(CoreSettings::getAlbumsIconDir(this->serviceId()));
}

// Getters
const QString Album::albumId() const { return objectId(); }
const QString Album::title() const { return d->title; }
const QString Album::description() const { return d->description; }
const QString Album::time_create() const { return d->time_create; }
const QString Album::time_update() const { return d->time_update; }
int Album::size() const { return d->size; }

// Setters
void Album::setAlbumId(const QString& v) { setObjectId(v); }
void Album::setTitle(const QString& v) { d->title = v; }
void Album::setDescription(const QString& v) { d->description = v; }
void Album::setTime_create(const QString& v) { d->time_create = v; }
void Album::setTime_update(const QString& v) { d->time_update = v; }
void Album::setSize(int v) { d->size = v; }
