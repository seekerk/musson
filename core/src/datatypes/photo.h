/**
  \file photo.h
  \brief Photo class description
  \author Musson team
  \date 2011-03-31
  */
#ifndef PHOTO_H
#define PHOTO_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QTextCodec>
#include <QMetaType>
#include <QDomDocument>
#include <QFile>
#include <QSharedData>
#include <QDataStream>

#include "musson_global.h"
#include "utils/qrefreshlist.h"
#include "datatypes/baseobject.h"
class Photo;

/*! \typedef PhotoList
 Structure for stacking process of photo uploading
 \sa QRefreshList
*/
typedef QRefreshList<Photo> PhotoList;

#include "photocomments.h"
#include "album.h"

#define FILE_PHOTO_COMMENTS_DATA "comments_%1_%2_%3.xml"
#define NODE_PHOTO_COMMENTS_ROOT "comments"

#define NODE_PHOTO_ROOT "photo"
#define NODE_PHOTO_ID "id"
#define NODE_PHOTO_CREATE "create"
#define NODE_PHOTO_IMAGE_URL "image_url"
#define NODE_PHOTO_ICON_URL "icon_url"
#define NODE_PHOTO_DESCRIPTION "description"

class PhotoData;

/*!
 \brief A class for photo information storage.
 */
class musson_EXPORT Photo : public BaseObject
{
    friend QDataStream& operator<<(QDataStream& out, const Photo& photo);
    friend QDataStream& operator>>(QDataStream& in, Photo& photo);

    QSharedDataPointer<PhotoData> d;
public:
    Photo();
    Photo(const Photo& source);
    ~Photo();

    // store photolist data to XML format
    QDomElement toQDomElement(QDomDocument& doc) const;

    // store photolist data to XML format
    static QDomElement toQDomElement(QDomDocument& doc, const Photo& src);

    // load photolist from XML struct
    static Photo fromQDomElement(const QDomElement& fr, const QString& serviceId, const QString& accountId,
                                 const QString& friendId, const QString& albumId);

    Photo& operator=(const Photo& source);

    // load photo comments from cache file.
    PhotoCommentList getPhotoComments() const;

    // set photo comments and store it to cache file.
    static void setPhotoComments(const PhotoCommentList& list, const QString& accountId,
                                 const QString& friendId, const QString& albumId, const QString& photoId);

    /**
      \brief Generate unique key for list of images

      \param photos List of images

      \return String contains image list key
      */
    static QString getKey(const PhotoList& photos);

    /**
      \brief Generate unique key for list of images

      \param photo The Photo instance.

      \return String contains image list key.
      */
    static QString getKey(const Photo& photo);

    /**
      \brief Generate unique key for list of images

      \param album The album with images

      \return String contains image list key
      */
    static QString getKey(const Album& album);

    const QString icon() const;

    /**
      \brief Load photo list from cache to memory.

      \param srvId The Service Id.

      \param accountId The Account Id.

      \param ownerId The Owner Id.

      \param albumId The album Id.

      \return Loaded album list.
      */
    static PhotoList loadPhotoList(const QString& srvId, const QString& accountId, const QString& ownerId, const QString& albumId);

    // Getters
    const QString photoId() const;
    const QString albumId() const;
    const QString time_create() const;
    const QString photoUrl() const;
    const QString photo() const;
    const QString description() const;

    // Setters
    void setPhotoId(const QString& v);
    void setAlbumId(const QString& v);
    void setTime_create(const QString& v);
    void setPhotoUrl(const QString& v);
    void setDescription(const QString& v);
};

musson_EXPORT QDataStream& operator<<(QDataStream& out, const Photo& photo);
musson_EXPORT QDataStream& operator>>(QDataStream& in, Photo& photo);

Q_DECLARE_METATYPE(Photo)
Q_DECLARE_METATYPE(PhotoList)

#endif
