#ifndef AUDIOITEM_H
#define AUDIOITEM_H

#include <datatypes/listmodel.h>
#include <datatypes/audio.h>

class AudioItem : public ListItem
{
public:
    AudioItem(QObject *parent = 0);
    explicit AudioItem(Audio audio, QObject *parent = 0);
    ~AudioItem();

    /*!
      \brief Get data from delegate based on defined role.

      \param data role
      \return data based on role
     */
    QVariant data(int role) const;

    /*!
      \brief Get hashtable of role names.

      \return hashtable of role names
     */
    QHash<int, QByteArray> roleNames() const;

    /*!
      \brief Get instance of audio, which is base for AudioItem.

      \return instance of audio
     */
    const QVariant& audio() const;

    /*!
      \brief Get id of AlbumItem.

      \return id of AlbumItem
     */
    QString id() const;

    /*!
      \brief Get key value.

      It is using for sorting AccountItem.

      \return variant with key value.
     */
    QVariant key() const;

    /*!
      \brief describes role names.
     */
    enum Roles {
        TitleRole = Qt::UserRole + 1, /*!< \brief Title of audio track. */
        ArtistRole, /*!< \brief Name of artist. */
        UrlRole, /*!< \brief URL for audio track. */
        FileRole, /*!< \brief Path to local audio file. */
        DataRole, /*!< \brief Instance of Audio which is base for AudioItem. */
        DurationRole, /*!< \brief Duration of audio track. */
        IDRole /*!< \brief ID of AudioItem. */
    };

    // Getters
    const QString audioId() const;
    const QString title() const;
    const QString artist() const;
    int duration() const;
    const QString url() const;
    const QString file() const;

private:
    QString m_audioId;
    QString m_title;
    QString m_artist;
    QString m_url;
    QString m_file;
    int m_duration;
    QVariant m_audio;
};

#endif // AUDIOITEM_H
