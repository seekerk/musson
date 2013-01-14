#include "photocomments.h"

class PhotoCommentData : public QSharedData
{
public:
    PhotoCommentData();
    PhotoCommentData(const PhotoCommentData& other);
    ~PhotoCommentData();

    QString albumId;
    QString photoId;
    QString senderId;
    QString senderName;
    QString time;
    QString text;
};

PhotoCommentData::PhotoCommentData()
    : QSharedData()
{
}

PhotoCommentData::PhotoCommentData(const PhotoCommentData& other)
    : QSharedData(other),
      albumId(other.albumId),
      photoId(other.photoId),
      senderId(other.senderId),
      senderName(other.senderName),
      time(other.time),
      text(other.text)
{
}

PhotoCommentData::~PhotoCommentData()
{
}


PhotoComment::PhotoComment()
    : BaseObject()
{
    d = new PhotoCommentData;
}

PhotoComment::PhotoComment(const PhotoComment &src)
    : BaseObject(src), d(src.d)
{
}

PhotoComment::~PhotoComment()
{
}

PhotoComment PhotoComment::fromQDomElement(QDomElement fr, QString accountId, QString friendId, QString albumId, QString photoId)
{
    PhotoComment ret;

    ret.setAccountId(accountId);
    ret.setOwnerId(friendId);
    ret.setAlbumId(albumId);
    ret.setPhotoId(photoId);
    ret.setCommentId(fr.elementsByTagName(NODE_PHOTO_COMMENT_ID).at(0).firstChild().toText().data());
    ret.setSenderId(fr.elementsByTagName(NODE_PHOTO_COMMENT_SENDER_ID).at(0).firstChild().toText().data());
    ret.setSenderName(fr.elementsByTagName(NODE_PHOTO_COMMENT_SENDER_NAME).at(0).firstChild().toText().data());
    ret.setText(fr.elementsByTagName(NODE_PHOTO_COMMENT_TEXT).at(0).firstChild().toText().data());
    ret.setTime(fr.elementsByTagName(NODE_PHOTO_COMMENT_TIME).at(0).firstChild().toText().data());

    return ret;
}

QDomElement PhotoComment::toQDomElement(QDomDocument doc)
{
    return PhotoComment::toQDomElement(doc, *this);
}

QDomElement PhotoComment::toQDomElement(QDomDocument doc, PhotoComment src)
{
    QDomElement ret = doc.createElement(NODE_PHOTO_COMMENT_ROOT);

    // ID
    QDomElement node = doc.createElement(NODE_PHOTO_COMMENT_ID);
    QDomText t = doc.createTextNode(src.commentId());
    ret.appendChild(node);
    node.appendChild(t);

    //sender id
    node = doc.createElement(NODE_PHOTO_COMMENT_SENDER_ID);
    t = doc.createTextNode(src.senderId());
    ret.appendChild(node);
    node.appendChild(t);

    //sender name
    node = doc.createElement(NODE_PHOTO_COMMENT_SENDER_NAME);
    t = doc.createTextNode(src.senderName());
    ret.appendChild(node);
    node.appendChild(t);

    //time
    node = doc.createElement(NODE_PHOTO_COMMENT_TIME);
    t = doc.createTextNode(src.time());
    ret.appendChild(node);
    node.appendChild(t);

    //text
    node = doc.createElement(NODE_PHOTO_COMMENT_TEXT);
    t = doc.createTextNode(src.text());
    ret.appendChild(node);
    node.appendChild(t);

    return ret;
}

PhotoComment& PhotoComment::operator =(PhotoComment const& src)
{
    BaseObject::operator =(src);
    d = src.d;
    return *this;
}


// Getters
const QString PhotoComment::albumId() const { return d->albumId; }
const QString PhotoComment::photoId() const { return d->photoId; }
const QString PhotoComment::commentId() const { return objectId(); }
const QString PhotoComment::senderId() const { return d->senderId; }
const QString PhotoComment::senderName() const { return d->senderName; }
const QString PhotoComment::time() const { return d->time; }
const QString PhotoComment::text() const { return d->text; }

// Setters
void PhotoComment::setAlbumId(const QString& v) { d->albumId = v; }
void PhotoComment::setPhotoId(const QString& v) { d->photoId = v; }
void PhotoComment::setCommentId(const QString& v) { setObjectId(v); }
void PhotoComment::setSenderId(const QString& v) { d->senderId = v; }
void PhotoComment::setSenderName(const QString& v) { d->senderName = v; }
void PhotoComment::setTime(const QString& v) { d->time = v; }
void PhotoComment::setText(const QString& v) { d->text = v; }
