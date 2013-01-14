/**
  \file album.h
  \brief Album class description
  \author Musson team
  \date 2011-03-30
  */
#ifndef ALBUM_H
#define ALBUM_H

#include <QString>
#include <QMetaType>
#include <QDomDocument>
#include <QFile>

#include "musson_global.h"
#include "utils/qrefreshlist.h"
#include "datatypes/baseobject.h"

class Album;

/*! \typedef AlbumList
 Structure for stacking process of album uploading
 \sa QRefreshList
*/
typedef QRefreshList<Album> AlbumList;

#include "photo.h"

/*! \def FILE_ALBUMS_DATA
 The name of file with list of photos
 */
#define FILE_PHOTOS_DATA "photos_%1_%2.xml"
#define NODE_PHOTOS_ROOT "photos"

#define NODE_ALBUM_ROOT "album"
#define ATTR_REFRESH "refresh"
#define NODE_ALBUM_ID "id"
#define NODE_ALBUM_TITLE "title"
#define NODE_ALBUM_DESCRIPTION "description"
#define NODE_ALBUM_SIZE "size"
#define NODE_ALBUM_CREATE "create"
#define NODE_ALBUM_UPDATE "update"
#define NODE_ALBUM_ICON "icon"
#define NODE_ALBUM_ICON_URL "iconUrl"

class AlbumData;

/*!
 \brief A class for album information storage.
 */
class musson_EXPORT Album : public BaseObject
{
    QSharedDataPointer<AlbumData> d;
public:
    //! Constructs an empty album.
    Album();

    //! Constructs a copy of album.
    /*!
     \brief Creates an instance of Album class based on another Album.

     \param instance of Album class
     */
    Album(const Album& src);

    //! A destructor.
    ~Album();

    /*!
     \brief Store album data to XML format.

     \param doc root XML document

     \return XML structure
     */
    QDomElement toQDomElement(QDomDocument& doc) const;

    /*!
     \brief Loads album from XML struct.

     \param fr XML struct with album data
     \param accountId ID of account to which friend belongs
     \param friendId ID of the owner of the album

     \return Created Album
     */
    static Album fromQDomElement(const QDomElement& fr, const QString& srvId, const QString& accountId, const QString& friendId);

    /*!
     \brief Loads photo list into Album from cache file.

     \return Created PhotoList
     */
    PhotoList getPhotoList() const;

    /*!
     \brief Set photo list and store it to cache file.

     \param list list of photos to store
     \param accountId account id for which need to save phot list
     \param friendId owner id of the images
     \param albumId id of saving album
     */
    static void setPhotoList(const PhotoList list, const QString accountId, const QString friendId, const QString albumId);

    /*!
      \brief Overloads assignment operator for Album classes.

      \param Album source Album instance
    */
    Album& operator=(const Album&);

    // Getters
    const QString albumId() const;
    const QString title() const;
    const QString description() const;
    const QString time_create() const;
    const QString time_update() const;
    int size() const;
    const QString icon() const;

    // Setters
    void setAlbumId(const QString& v);
    void setTitle(const QString& v);
    void setDescription(const QString& v);
    void setTime_create(const QString& v);
    void setTime_update(const QString& v);
    void setSize(int v);
};

Q_DECLARE_METATYPE(Album)
Q_DECLARE_METATYPE(AlbumList)


#endif // ALBUM_H
