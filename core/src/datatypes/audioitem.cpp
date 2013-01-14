#include "audioitem.h"

AudioItem::AudioItem(QObject *parent) :
    ListItem(parent)
{
}

AudioItem::AudioItem(Audio audio, QObject *parent) :
    ListItem(parent)
{
    m_audio.setValue(audio);
    m_title = audio.title();
    m_artist = audio.artist();
    m_audioId = audio.audioId();
    m_duration = audio.duration();
    m_url = audio.url();
    m_file = audio.file();
}

AudioItem::~AudioItem()
{

}

QHash<int, QByteArray> AudioItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TitleRole] = "title";
    names[ArtistRole] = "artist";
    names[DurationRole] = "duration";
    names[UrlRole] = "url";
    names[FileRole] = "file";
    names[DataRole] = "audio";
    names[IDRole] = "audioId";
    return names;
}

const QString AudioItem::title() const
{
    return m_title;
}

const QString AudioItem::artist() const
{
    return m_artist;
}

const QString AudioItem::url() const
{
    return m_url;
}

const QString AudioItem::file() const
{
    return m_file;
}

const QVariant& AudioItem::audio() const
{
    return m_audio;
}

int AudioItem::duration() const
{
    return m_duration;
}

QString AudioItem::id() const
{
    return m_audioId;
}

QVariant AudioItem::key() const
{
    QVariant v;
    v.setValue(m_title);
    return v;
}

QVariant AudioItem::data(int role) const
{
    switch(role) {
    case TitleRole:
        return title();
        break;
    case ArtistRole:
        return artist();
        break;
    case UrlRole:
        return url();
        break;
    case FileRole:
        return file();
        break;
    case DurationRole:
        return duration();
        break;
    case DataRole:
        return audio();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}

