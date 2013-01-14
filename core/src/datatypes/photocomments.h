#ifndef PHOTOCOMMENT_H
#define PHOTOCOMMENT_H

#include <QObject>
#include <QMetaType>
#include <QDomDocument>
#include <QSharedDataPointer>

#include "musson_global.h"
#include "utils/qrefreshlist.h"
#include "datatypes/baseobject.h"

class PhotoComment;
typedef QRefreshList<PhotoComment> PhotoCommentList;

#define NODE_PHOTO_COMMENT_ROOT "comment"
#define NODE_PHOTO_COMMENT_ID "id"
#define NODE_PHOTO_COMMENT_SENDER_ID "senderid"
#define NODE_PHOTO_COMMENT_SENDER_NAME "sendername"
#define NODE_PHOTO_COMMENT_TEXT "text"
#define NODE_PHOTO_COMMENT_TIME "time"

class PhotoCommentData;

class musson_EXPORT PhotoComment : public BaseObject
{
    QSharedDataPointer<PhotoCommentData> d;

public:
    explicit PhotoComment();
    PhotoComment(const PhotoComment& source);
    ~PhotoComment();

    // store data to XML format
    QDomElement toQDomElement(QDomDocument doc);

    // store data to XML format
    static QDomElement toQDomElement(QDomDocument doc, PhotoComment src);

    // load photolist from XML struct
    static PhotoComment fromQDomElement(QDomElement fr, QString accountId, QString friendId, QString albumId, QString photoId);

    PhotoComment& operator=(const PhotoComment& source);

    // Getters
    const QString albumId() const;
    const QString photoId() const;
    const QString commentId() const;
    const QString senderId() const;
    const QString senderName() const;
    const QString time() const;
    const QString text() const;

    // Setters
    void setAlbumId(const QString& v);
    void setPhotoId(const QString& v);
    void setCommentId(const QString& v);
    void setSenderId(const QString& v);
    void setSenderName(const QString& v);
    void setTime(const QString& v);
    void setText(const QString& v);
};

Q_DECLARE_METATYPE(PhotoComment)
//Q_DECLARE_METATYPE(Photo*)
Q_DECLARE_METATYPE(PhotoCommentList)


#endif // PHOTOCOMMENT_H
