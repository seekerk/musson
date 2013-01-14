#include "friend.h"

#include <QCoreApplication>
#include <QDomDocument>
#include <QFile>
#include <QDebug>

#include "datatypes/audio.h"
#include "datatypes/video.h"
#include "datatypes/event.h"
#include "utils/coresettings.h"

class FriendData : public QSharedData
{
public:
    FriendData();
    FriendData(const FriendData& other);
    ~FriendData();

    QString _firstName; /*!< First name if exists */
    QString _nickName;  /*!< Nickname if exists */
    QString _lastName;  /*!< Last name if exists */
    QString _gender;    /*!< friend gender (optional) */
    QString _birthday;  /*!< friend birthday (optional) */
    QString _mobilePhone; /*!< friend mobile phone number (optional) */
    QString _homePhone; /*!< friend home phone number (optional) */
    QString _city;      /*!< friend city (optional) */
    QString _country;   /*!< friend country (optional) */
    QString _status_message; /*!< status message */

    //! A friend status.
    /*!
     * true if friend is online, otherwise false (not stored in cache)
     */
    bool _online;

    /*!
     * true if this profile is main; otherwise main profile must be in list of profiles
     */
    bool _isMainProfile;

    FriendList _profiles; /*!< list of profiles from different services */
};

FriendData::FriendData()
{
}

FriendData::FriendData(const FriendData& other)
        : QSharedData(other)
{
    _birthday = other._birthday;
    _city = other._city;
    _country = other._country;
    _firstName = other._firstName;
    _gender = other._gender;
    _homePhone = other._homePhone;
    _lastName = other._lastName;
    _mobilePhone = other._mobilePhone;
    _nickName = other._nickName;
    _online = other._online;
    _status_message = other._status_message;
    _isMainProfile = other._isMainProfile;
    _profiles = other._profiles;
}

FriendData::~FriendData()
{
}


Friend::Friend()
    :BaseObject()
{
    d = new FriendData;
    d->_online = false;
    d->_isMainProfile = true;
}

Friend::Friend(const Friend& src)
    : BaseObject(src), d(src.d)
{
}

Friend::~Friend()
{
}


// ==== Setters/getters ==== //

void Friend::setMobilePhone(const QString& val)
{
    d->_mobilePhone = val;
}

const QString Friend::mobilePhone() const
{
    if (!d->_isMainProfile) {
        for (int i = 0; i < d->_profiles.length(); i++)
            if (d->_profiles.at(i).d->_isMainProfile)
                return d->_profiles.at(i).mobilePhone();
    }
    return d->_mobilePhone;
}

const QString Friend::icon() const
{
    return generateFileName(CoreSettings::getFriendsIconDir(this->serviceId()));
}

void Friend::setFirstName(const QString &val)
{
    d->_firstName = val;
}

const QString Friend::firstName() const
{
    return d->_firstName;
}

void Friend::setLastName(const QString& val)
{
    d->_lastName = val;
}

const QString Friend::lastName() const
{
    return d->_lastName;
}

void Friend::setCity(const QString& val)
{
    d->_city = val;
}

const QString Friend::city() const
{
    return d->_city;
}

void Friend::setCountry(const QString& val)
{
    d->_country = val;
}

const QString Friend::country() const
{
    return d->_country;
}

void Friend::setHomePhone(const QString& val)
{
    d->_homePhone = val;
}

const QString Friend::homePhone() const
{
    return d->_homePhone;
}

void Friend::setBirthday(const QString& val)
{
    d->_birthday = val;
}

const QString Friend::birthday() const
{
    return d->_birthday;
}

void Friend::setNickName(const QString& val)
{
    d->_nickName = val;
}

const QString Friend::nickName() const
{
    return d->_nickName;
}

void Friend::setGender(const QString& val)
{
    d->_gender = val;
}

const QString Friend::gender() const
{
    return d->_gender;
}

void Friend::setOnline(const bool val)
{
    d->_online = val;
}

bool Friend::online() const
{
    return d->_online;
}

const QString Friend::statusMessage() const
{
    return d->_status_message;
}

void Friend::setStatusMessage(const QString &message)
{
    d->_status_message = message;
}

const QString Friend::name() const
{
    QString ret = QObject::tr("Unknown");

    if (d->_firstName.isEmpty() && d->_lastName.isEmpty())
        ret = d->_nickName;
    else
        ret = d->_firstName;

    if (ret.length() > 0 && d->_lastName.length() > 0)
        ret += " ";
    ret += d->_lastName;

    return ret;
}

// ==== End setters/getters ==== //

QDomElement Friend::toQDomElement(QDomDocument& doc, const Friend& src, const bool addOptionalInfo)
{
    QDomElement ret = doc.createElement(NODE_FRIEND_ROOT);

    // ID
    QDomElement node = doc.createElement(NODE_FRIEND_ID);
    QDomText t = doc.createTextNode(src.ownerId());
    ret.appendChild(node);
    node.appendChild(t);

    // First Name
    if (!src.firstName().isEmpty())
    {
        node = doc.createElement(NODE_FRIEND_FIRST_NAME);
        t = doc.createTextNode(src.firstName());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // Nick Name
    if (!src.nickName().isEmpty())
    {
        node = doc.createElement(NODE_FRIEND_NICK_NAME);
        t = doc.createTextNode(src.nickName());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // Last Name
    if (!src.lastName().isEmpty())
    {
        node = doc.createElement(NODE_FRIEND_LAST_NAME);
        t = doc.createTextNode(src.lastName());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // avatar url
    if (!src.iconUrl().isEmpty())
    {
        node = doc.createElement(NODE_FRIEND_ICON_URL);
        t = doc.createTextNode(src.iconUrl());
        ret.appendChild(node);
        node.appendChild(t);
    }

    if (addOptionalInfo) {
        // gender
        if (!src.gender().isEmpty())
        {
            node = doc.createElement(NODE_FRIEND_GENDER);
            t = doc.createTextNode(src.gender());
            ret.appendChild(node);
            node.appendChild(t);
        }

        // birthday
        if (!src.birthday().isEmpty())
        {
            node = doc.createElement(NODE_FRIEND_BIRTHDAY);
            t = doc.createTextNode(src.birthday());
            ret.appendChild(node);
            node.appendChild(t);
        }

        // mobile phone
        if (!src.mobilePhone().isEmpty())
        {
            node = doc.createElement(NODE_FRIEND_MOBILE_PHONE);
            t = doc.createTextNode(src.mobilePhone());
            ret.appendChild(node);
            node.appendChild(t);
        }

        // home phone
        if (!src.homePhone().isEmpty())
        {
            node = doc.createElement(NODE_FRIEND_HOME_PHONE);
            t = doc.createTextNode(src.homePhone());
            ret.appendChild(node);
            node.appendChild(t);
        }

        // city
        if (!src.city().isEmpty())
        {
            node = doc.createElement(NODE_FRIEND_CITY);
            t = doc.createTextNode(src.city());
            ret.appendChild(node);
            node.appendChild(t);
        }

        // country
        if (!src.country().isEmpty())
        {
            node = doc.createElement(NODE_FRIEND_COUNTRY);
            t = doc.createTextNode(src.country());
            ret.appendChild(node);
            node.appendChild(t);
        }

        // status message
        if (!src.statusMessage().isEmpty())
        {
            node = doc.createElement(NODE_FRIEND_STATUS);
            t = doc.createTextNode(src.statusMessage());
            ret.appendChild(node);
            node.appendChild(t);
        }
    }

    return ret;
}

Friend Friend::fromQDomElement(const QDomElement& fr, const QString& accountId_, const QString& serviceId_)
{
    Friend ret = Friend();

    if (fr.elementsByTagName(NODE_FRIEND_ID).count() > 0)
        ret.setOwnerId(fr.elementsByTagName(NODE_FRIEND_ID).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_FIRST_NAME).count() > 0)
        ret.setFirstName(fr.elementsByTagName(NODE_FRIEND_FIRST_NAME).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_NICK_NAME).count() > 0)
        ret.setNickName(fr.elementsByTagName(NODE_FRIEND_NICK_NAME).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_LAST_NAME).count() > 0)
        ret.setLastName(fr.elementsByTagName(NODE_FRIEND_LAST_NAME).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_ICON_URL).count() > 0)
        ret.setIconUrl(fr.elementsByTagName(NODE_FRIEND_ICON_URL).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_GENDER).count() > 0)
        ret.setGender(fr.elementsByTagName(NODE_FRIEND_GENDER).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_BIRTHDAY).count() > 0)
        ret.setBirthday(fr.elementsByTagName(NODE_FRIEND_BIRTHDAY).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_MOBILE_PHONE).count() > 0)
        ret.setMobilePhone(fr.elementsByTagName(NODE_FRIEND_MOBILE_PHONE).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_HOME_PHONE).count() > 0)
        ret.setHomePhone(fr.elementsByTagName(NODE_FRIEND_HOME_PHONE).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_CITY).count() > 0)
        ret.setCity(fr.elementsByTagName(NODE_FRIEND_CITY).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_COUNTRY).count() > 0)
        ret.setCountry(fr.elementsByTagName(NODE_FRIEND_COUNTRY).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_FRIEND_STATUS).count() > 0)
        ret.setStatusMessage(fr.elementsByTagName(NODE_FRIEND_STATUS).at(0).firstChild().toText().data());
    ret.setAccountId(accountId_);
    ret.setServiceId(serviceId_);

    return ret;
}

AlbumList Friend::getAlbumList() const
{
    AlbumList ret;
    QFile file(CoreSettings::getAccountDir(this->accountId()) + QString(FILE_ALBUMS_DATA).arg(this->ownerId()));
    if (file.open(QIODevice::ReadOnly) == false)
        return ret;

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    // get list updated time
    QDomElement root = doc.elementsByTagName(NODE_ALBUMS_ROOT).at(0).toElement();
    QDateTime time_create = QDateTime::fromString(root.attribute(ATTR_REFRESH, ""), DATE_TIME_FORMAT);
    ret.setTimeCreate(time_create);

    // parse list of albums
    QDomNodeList lst = doc.elementsByTagName(NODE_ALBUM_ROOT);

    qDebug() << "Start load of " << lst.length() << " albums for " << this->ownerId();
    for (uint i = 0; i < lst.length(); i++) {
        ret.append(Album::fromQDomElement(lst.at(i).toElement(), this->serviceId(), this->accountId(), this->ownerId()));
    }

    return ret;

}

void Friend::setAlbumList(const AlbumList& list, const QString& accountId_, const QString& friendId_)
{
    if (friendId_.isEmpty())
        return;

    // store albums into cache
    QDomDocument out("MyDoc");
    QDomElement profile = out.createElement(NODE_ALBUMS_ROOT);
    profile.setAttribute(ATTR_REFRESH, list.timeCreate().toString(DATE_TIME_FORMAT));
    out.appendChild(profile);


    for (int i = 0; i < list.length(); i++) {
        profile.appendChild(list[i].toQDomElement(out));
    }

    QDir dir;

    dir.setPath(dir.cleanPath(CoreSettings::getAccountDir(accountId_)));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QFile file(dir.absolutePath() + "/" + QString(FILE_ALBUMS_DATA).arg(friendId_));
    qDebug() << file.fileName();
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
        qDebug() << "can't open file: " + file.errorString();
    }
    //file.write(output.toUtf8());
    file.write(out.toByteArray(1));
    file.close();

}

AudioList Friend::getAudioList() const
{
    AudioList ret;
    QFile file(CoreSettings::getAccountDir(this->accountId()) + QString(FILE_AUDIO_DATA).arg(this->ownerId()));
    if (file.open(QIODevice::ReadOnly) == false)
        return ret;

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    // get list updated time
    QDomElement root = doc.elementsByTagName(NODE_AUDIO_ROOT).at(0).toElement();
    QDateTime time_create = QDateTime::fromString(root.attribute(ATTR_REFRESH, ""), DATE_TIME_FORMAT);
    ret.setTimeCreate(time_create);

    // parse list of audio
    QDomNodeList lst = doc.elementsByTagName(NODE_AUDIO_ROOT);

    qDebug() << "Start load of " << lst.length() << " audio for " << this->ownerId();
    for (uint i = 0; i < lst.length(); i++) {
        ret.append(Audio::fromQDomElement(lst.at(i).toElement(), this->serviceId(), this->accountId(), this->ownerId()));
    }

    return ret;
}

void Friend::setAudioList(const AudioList& list, const QString& accountId_, const QString& friendId_)
{
    if (friendId_.isEmpty())
        return;

    // store audio into cache
    QDomDocument out("MyDoc");
    QDomElement profile = out.createElement("audios");
    profile.setAttribute(ATTR_REFRESH, list.timeCreate().toString(DATE_TIME_FORMAT));
    out.appendChild(profile);


    for (int i = 0; i < list.length(); i++) {
        profile.appendChild(list[i].toQDomElement(out));
    }

    QDir dir;

    dir.setPath(dir.cleanPath(CoreSettings::getAccountDir(accountId_)));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QFile file(dir.absolutePath() + "/" + QString(FILE_AUDIO_DATA).arg(friendId_));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
        qDebug() << "can't open file: " + file.errorString();
    }
    //file.write(output.toUtf8());
    file.write(out.toByteArray(1));
    file.close();

}

VideoList Friend::getVideoList() const
{
    VideoList ret;
    QFile file(CoreSettings::getAccountDir(this->accountId()) + QString(FILE_VIDEO_DATA).arg(this->ownerId()));
    if (file.open(QIODevice::ReadOnly) == false)
        return ret;

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    // get list updated time
    QDomElement root = doc.elementsByTagName(NODE_VIDEO_ROOT).at(0).toElement();
    QDateTime time_create = QDateTime::fromString(root.attribute(ATTR_REFRESH, ""), DATE_TIME_FORMAT);
    ret.setTimeCreate(time_create);

    // parse list of video
    QDomNodeList lst = doc.elementsByTagName(NODE_VIDEO_ROOT);

    qDebug() << "Start load of " << lst.length() << " video for " << this->ownerId();
    for (uint i = 0; i < lst.length(); i++) {
        ret.append(Video::fromQDomElement(lst.at(i).toElement(), this->serviceId(), this->accountId(), this->ownerId()));
    }

    return ret;
}

void Friend::setVideoList(const VideoList& list, const QString& accountId_, const QString& friendId_)
{
    if (friendId_.isEmpty())
        return;

    // store video into cache
    QDomDocument out("MyDoc");
    QDomElement profile = out.createElement("videos");
    profile.setAttribute(ATTR_REFRESH, list.timeCreate().toString(DATE_TIME_FORMAT));
    out.appendChild(profile);

    for (int i = 0; i < list.length(); i++) {
        profile.appendChild(list[i].toQDomElement(out));
    }

    QDir dir;

    dir.setPath(dir.cleanPath(CoreSettings::getAccountDir(accountId_)));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QFile file(dir.absolutePath() + "/" + QString(FILE_VIDEO_DATA).arg(friendId_));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
        qDebug() << "can't open file: " + file.errorString();
    }
    //file.write(output.toUtf8());
    file.write(out.toByteArray(1));
    file.close();
}

EventList Friend::getEventsList() const
{
    EventList ret;
    QFile file(CoreSettings::getAccountDir(this->accountId()) + QString(FILE_EVENTS_DATA).arg(this->ownerId()));
    if (file.open(QIODevice::ReadOnly) == false)
        return ret;

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    // get list updated time
    QDomElement root = doc.elementsByTagName(NODE_EVENT_ROOT).at(0).toElement();
    QDateTime time_create = QDateTime::fromString(root.attribute(ATTR_REFRESH, ""), DATE_TIME_FORMAT);
    ret.setTimeCreate(time_create);

    // parse list of events
    QDomNodeList lst = doc.elementsByTagName(NODE_EVENT_ROOT);

    qDebug() << "Start load of " << lst.length() << " events for " << this->ownerId();
    for (uint i = 0; i < lst.length(); i++) {
        ret.append(Event::fromQDomElement(lst.at(i).toElement(), this->serviceId(), this->accountId(), this->ownerId()));
    }

    return ret;
}

void Friend::setEventsList(const EventList& list, const QString& accountId_, const QString& friendId_)
{
    if (friendId_.isEmpty())
        return;

    // store events into cache
    QDomDocument out("MyDoc");
    QDomElement profile = out.createElement("events");
    profile.setAttribute(ATTR_REFRESH, list.timeCreate().toString(DATE_TIME_FORMAT));
    out.appendChild(profile);


    for (int i = 0; i < list.length(); i++) {
        profile.appendChild(list[i].toQDomElement(out));
    }

    QDir dir;

    dir.setPath(dir.cleanPath(CoreSettings::getAccountDir(accountId_)));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QFile file(dir.absolutePath() + "/" + QString(FILE_EVENTS_DATA).arg(friendId_));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
        qDebug() << "can't open file: " + file.errorString();
    }
    //file.write(output.toUtf8());
    file.write(out.toByteArray(1));
    file.close();
}

bool Friend::operator <(const Friend& fr1) const
{
    // compare names
    if (this->online() == fr1.online())
        return this->name() < fr1.name();

    // if this is online then prefer
    if (this->online())
        return true;

    // fr1 is online, prefer
    return false;
}

bool Friend::operator ==(const Friend& fr1) const
{
    return this->ownerId() == fr1.ownerId() && this->accountId() == fr1.accountId();
}

void Friend::storeData()
{
    if (this->isEmpty())
        return;

    // store albums into cache
    QDomDocument out("MyDoc");

    out.appendChild(this->toQDomElement(out, *this, true));

    QDir dir;

    dir.setPath(dir.cleanPath(CoreSettings::getAccountDir(this->accountId())));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QFile file(dir.absolutePath() + "/" + QString(FILE_OWNER_DATA).arg(this->ownerId()));
    qDebug() << file.fileName();
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
        qDebug() << "can't open file: " + file.errorString();
    }
    //file.write(output.toUtf8());
    file.write(out.toByteArray(1));
    file.close();
}

Friend& Friend::operator=(const Friend& other)
{
    BaseObject::operator=(other);
    d = other.d;
    return *this;
}

Friend Friend::loadData(const QString& serviceId_, const QString& accountId_, const QString& ownerId_)
{
    Friend ret;
    QFile file(CoreSettings::getAccountDir(accountId_) + QString(FILE_OWNER_DATA).arg(ownerId_));
    if (file.open(QIODevice::ReadOnly) == false)
        return ret;

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    return Friend::fromQDomElement(doc.firstChildElement(), accountId_, serviceId_);
}

bool Friend::hasFullProfile() const
{
    QFile file(CoreSettings::getAccountDir(this->accountId()) + QString(FILE_OWNER_DATA).arg(this->ownerId()));
    if (file.exists())
        return true;

    return false;
}

bool Friend::isMetaProfile() const
{
    if (this->d->_profiles.length() > 0)
        return true;
    else
        return false;
}

FriendList Friend::getProfileList() const
{
    return this->d->_profiles;
}

void Friend::addProfile(const Friend& fr, const bool isMainProfile)
{
    if (isMainProfile) {
        d->_isMainProfile = false;
        for (int i = 0; i < d->_profiles.length(); i++)
            d->_profiles[i].d->_isMainProfile = false;
    }

    d->_profiles.append(fr);
}

bool Friend::hasProfile(const Friend& fr) const
{
    if (this->accountId().compare(fr.accountId()) == 0 &&
        this->ownerId().compare(fr.ownerId()) == 0)
        return true;

    for (int i = 0; i < d->_profiles.length(); i++) {
        if (fr.accountId().compare(d->_profiles.at(i).accountId()) == 0 &&
            fr.ownerId().compare(d->_profiles.at(i).ownerId()) == 0)
            return true;
    }

    return false;
}

bool Friend::hasProfile(const QString& serviceId, const QString& ownerId) const
{
    if (this->serviceId().compare(serviceId) == 0 &&
        this->ownerId().compare(ownerId) == 0)
        return true;

    for (int i = 0; i < d->_profiles.length(); i++) {
        if (serviceId.compare(d->_profiles.at(i).serviceId()) == 0 &&
            ownerId.compare(d->_profiles.at(i).ownerId()) == 0)
            return true;
    }

    return false;
}

FriendList Friend::mergeLists(FriendList src1, FriendList src2)
{
    if (src1.isEmpty())
        return src2;

    if (src2.isEmpty())
        return src1;

    FriendList ret;

    for (int i = 0; i < src1.length(); i++) {
        Friend fr = src1.at(i);
        for (int j = 0; j < src2.length(); j++) {
            Friend fr2 = src2.at(j);
            if (fr2.hasProfile(fr.serviceId(), fr.ownerId())) {
                fr.addProfile(fr2, false);
                src2.removeAt(j);
            }
        }
        ret.append(fr);

        if (ret.count() % 100 == 0)
            QCoreApplication::processEvents();
    }

    ret.append(src2);

    return ret;
}
