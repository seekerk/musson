#ifndef AUDIO_H
#define AUDIO_H

#include <QString>
#include <QMetaType>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QSharedData>

#include "musson_global.h"
#include "utils/qrefreshlist.h"
#include "datatypes/baseobject.h"

class Audio;

#define NODE_AUDIO_ROOT "audio"
#define ATTR_REFRESH "refresh"
#define NODE_AUDIO_ID "id"
#define NODE_AUDIO_TITLE "title"
#define NODE_AUDIO_ARTIST "artist"
#define NODE_AUDIO_DURATION "duration"
#define NODE_AUDIO_URL "url"

/*! \typedef AudioList
 Structure for stacking process of audio uploading
 \sa QRefreshList
*/
typedef QRefreshList<Audio> AudioList;

Q_DECLARE_METATYPE(Audio)
Q_DECLARE_METATYPE(AudioList)

class musson_EXPORT AudioData;

/*!
 \brief A class for audio information storage.
 */
class musson_EXPORT Audio : public BaseObject
{
    QSharedDataPointer<AudioData> d;

public:
    //! Constructs an empty audio.
    Audio();

    //! Constructs a copy of audio.
    /*!
     \brief Creates an instance of Audio class based on another Audio.

     \param instance of Audio class
     */
    Audio(const Audio& src);

    //! A destructor.
    ~Audio();

    /*!
     \brief Store audio data to XML format.

     \param doc root XML document

     \return XML structure
     */
    QDomElement toQDomElement(QDomDocument& doc) const;

    /*!
     \brief Loads audio from XML struct.

     \param fr XML struct with audio data
     \param srvcId ID of service
     \param accountId ID of account to which friend belongs
     \param friendId ID of the owner of the album

     \return Created Audio
     */
    static Audio fromQDomElement(const QDomElement& fr, const QString& srvcId, const QString& accountId, const QString& friendId);

    /*!
      \brief Overloads assignment operator for Audio classes.

      \param Audio source Audio instance
    */
    Audio& operator=(const Audio&);

    /*!
      \brief compare title of two audio.
      */
    bool operator <(const Audio& aud1) const;

    /*!
      \brief compare title, name artist and ownerId of two audio.
      */
    bool operator ==(const Audio& aud1) const;

    // Getters
    const QString audioId() const;
    const QString title() const;
    const QString artist() const;
    int duration() const;
    const QString url() const;
    const QString file() const;

    // Setters
    void setTitle(const QString& v);
    void setArtist(const QString& v);
    void setDuration(int v);
    void setUrl(const QString& v);
    void setAudioId(const QString& v);
};

#endif // AUDIO_H
