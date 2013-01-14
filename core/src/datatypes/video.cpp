#include "video.h"
#include "utils/coresettings.h"

class VideoData : public QSharedData
{
public:
    VideoData();
    VideoData(const VideoData& other);
    ~VideoData();

    QString videoId;     /*!< video ID from service */
    QString title;       /*!< video title (optional) */
    QString description; /*!< description of video file */
    int duration;        /*!< duration of video file */
    QString link;        /*!< url link to page with video */
    QString url;         /*!< url link to video file */
};

VideoData::VideoData()
    : QSharedData()
{
    duration = 0;
}

VideoData::VideoData(const VideoData& other)
    : QSharedData(other)
{
    this->videoId = other.videoId;
    this->title = other.title;
    this->description = other.description;
    this->duration = other.duration;
    this->link = other.link;
    this->url = other.url;
}

VideoData::~VideoData()
{
}


Video::Video()
    : BaseObject()
{
    d = new VideoData;
}

Video::Video(const Video &src)
    : BaseObject(src), d(src.d)
{
}

Video::~Video()
{
}

QDomElement Video::toQDomElement(QDomDocument& doc) const
{
    QDomElement ret = doc.createElement(NODE_VIDEO_ROOT);

    // ID
    QDomElement node = doc.createElement(NODE_VIDEO_ID);
    QDomText t = doc.createTextNode(this->videoId());
    ret.appendChild(node);
    node.appendChild(t);

    // Name
    if (!this->title().isEmpty()) {
        node = doc.createElement(NODE_VIDEO_TITLE);
        t = doc.createTextNode(this->title());
        ret.appendChild(node);
        node.appendChild(t);
    } else {
        node = doc.createElement(NODE_VIDEO_TITLE);
        t = doc.createTextNode("Untitled video");
        ret.appendChild(node);
        node.appendChild(t);
    }

    // description
    if (!this->description().isEmpty())
    {
        node = doc.createElement(NODE_VIDEO_DESCRIPTION);
        t = doc.createTextNode(this->description());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // duration
    node = doc.createElement(NODE_VIDEO_DURATION);
    t = doc.createTextNode(QString("%1").arg(this->duration()));
    ret.appendChild(node);
    node.appendChild(t);

    // icon url
    if (!this->iconUrl().isEmpty())
    {
        node = doc.createElement(NODE_VIDEO_IMG);
        t = doc.createTextNode(this->iconUrl());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // link
    if (!this->link().isEmpty())
    {
        node = doc.createElement(NODE_VIDEO_LINK);
        t = doc.createTextNode(this->link());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // url
    if (!this->url().isEmpty())
    {
        node = doc.createElement(NODE_VIDEO_URL);
        t = doc.createTextNode(this->url());
        ret.appendChild(node);
        node.appendChild(t);
    }

    return ret;
}

Video Video::fromQDomElement(const QDomElement& fr, const QString& srvcId, const QString& accountId, const QString& friendId)
{
    Video ret = Video();

    ret.setVideoId(fr.elementsByTagName(NODE_VIDEO_ID).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_VIDEO_TITLE).count() > 0)
        ret.setTitle(fr.elementsByTagName(NODE_VIDEO_TITLE).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_VIDEO_DESCRIPTION).count() > 0)
        ret.setDescription(fr.elementsByTagName(NODE_VIDEO_DESCRIPTION).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_VIDEO_DURATION).count() > 0)
        ret.setDuration(fr.elementsByTagName(NODE_VIDEO_DURATION).at(0).firstChild().toText().data().toInt());
    if (fr.elementsByTagName(NODE_VIDEO_IMG).count()>0)
        ret.setIconUrl(fr.elementsByTagName(NODE_VIDEO_IMG).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_VIDEO_LINK).count()>0)
        ret.setLink(fr.elementsByTagName(NODE_VIDEO_LINK).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_VIDEO_URL).count()>0)
        ret.setUrl(fr.elementsByTagName(NODE_VIDEO_URL).at(0).firstChild().toText().data());

    ret.setServiceId(srvcId);
    ret.setAccountId(accountId);
    ret.setOwnerId(friendId);

    return ret;
}

Video& Video::operator =(const Video& other)
{
    BaseObject::operator =(other);
    d = other.d;
    return *this;
}

bool Video::operator <(const Video& aud1) const
{
    return this->title() < aud1.title();
}

bool Video::operator ==(const Video& vid1) const
{
    if ((this->title() == vid1.title()) && (this->ownerId() == vid1.ownerId()))
        return true;
    else
        return false;
}

const QString Video::icon() const
{
    return generateFileName(CoreSettings::getVideoIconDir(this->serviceId()));
}

// Getters
const QString Video::videoId() const { return d->videoId; }
const QString Video::title() const { return d->title; }
const QString Video::description() const { return d->description; }
int Video::duration() const { return d->duration; }
const QString Video::link() const { return d->link; }
const QString Video::url() const { return d->url; }

// Setters
void Video::setVideoId(const QString& v) { d->videoId = v; }
void Video::setTitle(const QString& v) { d->title = v; }
void Video::setDescription(const QString& v) { d->description = v; }
void Video::setDuration(int v) { d->duration = v; }
void Video::setLink(const QString& v) { d->link = v; }
void Video::setUrl(const QString& v) { d->url = v; }
