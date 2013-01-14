#ifndef VIDEO_H
#define VIDEO_H

#include <QString>
#include <QMetaType>
#include <QDomDocument>
#include <QFile>
#include <QDebug>

#include "utils/qrefreshlist.h"
#include "datatypes/baseobject.h"
#include "musson_global.h"

class Video;

#define NODE_VIDEO_ROOT "video"
#define ATTR_REFRESH "refresh"
#define NODE_VIDEO_ID "id"
#define NODE_VIDEO_TITLE "title"
#define NODE_VIDEO_DESCRIPTION "description"
#define NODE_VIDEO_DURATION "duration"
#define NODE_VIDEO_IMG "Img"
#define NODE_VIDEO_LINK "link"
#define NODE_VIDEO_URL "url"
#define NODE_VIDEO_ICON "icon"

/*! \typedef VideoList
 Structure for stacking process of video uploading
 \sa QRefreshList
*/
typedef QRefreshList<Video> VideoList;

Q_DECLARE_METATYPE(Video)
Q_DECLARE_METATYPE(VideoList)

class VideoData;

/*!
 \brief A class for video information storage.
 */
class musson_EXPORT Video : public BaseObject
{
    QSharedDataPointer<VideoData> d;

public:
    //! Constructs an empty video.
    Video();

    //! Constructs a copy of video.
    /*!
     \brief Creates an instance of Video class based on another Video.

     \param instance of Video class
     */
    Video(const Video& src);

    //! A destructor.
    ~Video();

    /*!
     \brief Store video data to XML format.

     \param doc root XML document

     \return XML structure
     */
    QDomElement toQDomElement(QDomDocument& doc) const;

    /*!
     \brief Loads video from XML struct.

     \param fr XML struct with video data
     \param srvcId Id of service
     \param accountId ID of account to which friend belongs
     \param friendId ID of the owner of the video

     \return Created Video
     */
    static Video fromQDomElement(const QDomElement& fr, const QString& srvcId, const QString& accountId, const QString& friendId);

    /*!
      \brief Overloads assignment operator for Video classes.

      \param Video source Video instance
    */
    Video& operator=(const Video&);

    /*!
      \brief compare title of two video.
      */
    bool operator <(const Video& aud1) const;

    /*!
      \brief compare title and ownerId of two video.
      */
    bool operator ==(const Video& aud1) const;

    /*!
      \brief compare title and ownerId of two video.
      */
    bool operator !=(const Video& aud1) const;

    const QString icon() const;

    // Getters
    const QString videoId() const;
    const QString title() const;
    const QString description() const;
    int duration() const;
    const QString url() const;
    const QString link() const;

    // Setters
    void setVideoId(const QString& v);
    void setTitle(const QString& v);
    void setDescription(const QString& v);
    void setDuration(int v);
    void setLink(const QString& v);
    void setUrl(const QString& v);
};

#endif // VIDEO_H
