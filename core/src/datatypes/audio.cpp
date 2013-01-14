#include "audio.h"
#include "utils/coresettings.h"

class AudioData : public QSharedData
{
public:
    AudioData();
    AudioData(const AudioData& other);
    ~AudioData();

    QString title;       /*!< audio title (optional) */
    QString artist;      /*!< name of artist */
    int duration;        /*!< duration of audio file */
    QString url;         /*!< url link to audio file */
};

AudioData::AudioData()
    : QSharedData()
{
    duration = 0;
}

AudioData::AudioData(const AudioData& other)
    : QSharedData(other)
{
    this->title = other.title;
    this->artist = other.artist;
    this->duration = other.duration;
    this->url = other.url;
}

AudioData::~AudioData()
{
}

Audio::Audio()
    : BaseObject()
{
    d = new AudioData;
}

Audio::Audio(const Audio &src)
    : BaseObject(src), d(src.d)
{
}

Audio::~Audio()
{
}

QDomElement Audio::toQDomElement(QDomDocument& doc) const
{
    QDomElement ret = doc.createElement(NODE_AUDIO_ROOT);

    // ID
    QDomElement node = doc.createElement(NODE_AUDIO_ID);
    QDomText t = doc.createTextNode(this->audioId());
    ret.appendChild(node);
    node.appendChild(t);

    // Name
    if (!this->title().isEmpty())
    {
        node = doc.createElement(NODE_AUDIO_TITLE);
        t = doc.createTextNode(this->title());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // artist
    if (!this->artist().isEmpty())
    {
        node = doc.createElement(NODE_AUDIO_ARTIST);
        t = doc.createTextNode(this->artist());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // duration
    node = doc.createElement(NODE_AUDIO_DURATION);
    t = doc.createTextNode(QString("%1").arg(this->duration()));
    ret.appendChild(node);
    node.appendChild(t);

    // url
    if (!this->url().isEmpty())
    {
        node = doc.createElement(NODE_AUDIO_URL);
        t = doc.createTextNode(this->url());
        ret.appendChild(node);
        node.appendChild(t);
    }

    return ret;
}

Audio Audio::fromQDomElement(const QDomElement& fr, const QString& srvcId, const QString& accountId, const QString& friendId)
{
    Audio ret = Audio();

    ret.setAudioId(fr.elementsByTagName(NODE_AUDIO_ID).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_AUDIO_TITLE).count() > 0)
        ret.setTitle(fr.elementsByTagName(NODE_AUDIO_TITLE).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_AUDIO_ARTIST).count() > 0)
        ret.setArtist(fr.elementsByTagName(NODE_AUDIO_ARTIST).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_AUDIO_DURATION).count() > 0)
        ret.setDuration(fr.elementsByTagName(NODE_AUDIO_DURATION).at(0).firstChild().toText().data().toInt());
    if (fr.elementsByTagName(NODE_AUDIO_URL).count()>0)
        ret.setUrl(fr.elementsByTagName(NODE_AUDIO_URL).at(0).firstChild().toText().data());

    ret.setServiceId(srvcId);
    ret.setAccountId(accountId);
    ret.setOwnerId(friendId);

    return ret;
}

Audio& Audio::operator =(const Audio& other)
{
    BaseObject::operator =(other);
    d = other.d;
    return *this;
}

bool Audio::operator <(const Audio& aud1) const
{
    return this->title() < aud1.title();
}

bool Audio::operator ==(const Audio& aud1) const
{
    if ((this->title() == aud1.title()) && (this->artist() == aud1.artist()) && (this->ownerId() == aud1.ownerId()))
        return true;
    else
        return false;
}

const QString Audio::file() const
{
    return CoreSettings::getAudioDir() + ownerId() + QString("-") + objectId() + QString(".mp3");
}

// Getters
const QString Audio::audioId() const { return objectId(); }
const QString Audio::title() const { return d->title; }
const QString Audio::artist() const { return d->artist; }
int Audio::duration() const { return d->duration; }
const QString Audio::url() const { return d->url; }

// Setters
void Audio::setAudioId(const QString& v) { setObjectId(v); }
void Audio::setTitle(const QString& v) { d->title = v; }
void Audio::setArtist(const QString& v) { d->artist = v; }
void Audio::setDuration(int v) { d->duration = v; }
void Audio::setUrl(const QString& v) { d->url = v; }
