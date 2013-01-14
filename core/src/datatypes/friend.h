/**
  \file friend.h
  \brief Friend class description
  \author Musson team
  \date 2011-03-23
  */
#ifndef FRIEND_H
#define FRIEND_H

#include <QObject>
#include <QString>
#include <QMetaType>
#include <QSharedData>

class Friend;

#include "utils/qrefreshlist.h"
#include "datatypes/baseobject.h"
#include "musson_global.h"
#include "album.h"

/*! \def FILE_ALBUMS_DATA
 The name of file with list of albums
 */
#define FILE_ALBUMS_DATA "albums_%1.xml"

/*! \def FILE_AUDIO_DATA
 The name of file with list of audio files
 */
#define FILE_AUDIO_DATA "audio_%1.xml"

/*! \def FILE_VIDEO_DATA
 The name of file with list of video files
 */
#define FILE_VIDEO_DATA "video_%1.xml"

/*! \def FILE_EVENTS_DATA
 The name of file with list of event files
 */
#define FILE_EVENTS_DATA "events_%1.xml"

#define NODE_ALBUMS_ROOT "albums"

#define NODE_FRIEND_ROOT "friend"
#define NODE_FRIEND_ID "id"
#define NODE_FRIEND_FIRST_NAME "firstname"
#define NODE_FRIEND_NICK_NAME "nickname"
#define NODE_FRIEND_LAST_NAME "lastname"
//#define NODE_FRIEND_ICON "icon"
#define NODE_FRIEND_ICON_URL "iconUrl"
#define NODE_FRIEND_GENDER "gender"
#define NODE_FRIEND_BIRTHDAY "birthday"
#define NODE_FRIEND_MOBILE_PHONE "mobilePhone"
#define NODE_FRIEND_HOME_PHONE "homePhone"
#define NODE_FRIEND_CITY "city"
#define NODE_FRIEND_COUNTRY "country"
#define NODE_FRIEND_STATUS "status"

#define FILE_OWNER_DATA "owner_%1.xml"

typedef QRefreshList<Friend> FriendList;

Q_DECLARE_METATYPE(Friend)
Q_DECLARE_METATYPE(FriendList)

// FriendData defined in friend.cpp
class FriendData;

// Audio defined in audio.cpp
class Audio;
typedef QRefreshList<Audio> AudioList;

// Video defined in video.cpp
class Video;
typedef QRefreshList<Video> VideoList;

// Events defined in event.cpp
class Event;
typedef QRefreshList<Event> EventList;

/*!
 \brief A class for friend information storage.
 */
class musson_EXPORT Friend : public BaseObject
{
    QSharedDataPointer<FriendData> d;
public:
    //! An empty constructor.
    Friend();

    //! A destructor.
    ~Friend();

    //! A copy constructor
    Friend(const Friend& fr);

    /*!
      \brief Sets first name of a friend

      \param val first name of a friend
     */
    void setFirstName(const QString& val);

    /*!
      \brief Gets first name of a friend.

      \return first name of a friend
     */
    const QString firstName() const;

    /*!
      \brief Sets nick name of a friend.

      \param val nick of a friend
     */
    void setNickName(const QString& val);

    /*!
      \brief Gets nick name of a friend.

      \return nick name of a friend
     */
    const QString nickName() const;

    /*!
      \brief Sets last name of a friend.

      \param val last name of a friend
     */
    void setLastName(const QString &val);

    /*!
      \brief Gets last name of a friend.

      \return last name of a friend
     */
    const QString lastName() const;

    /*!
      \brief Sets status of a friend.

      \param val status of a friend
     */
    void setOnline(const bool val);

    /*!
      \brief Gets status of a friend.

      \return bool true if friend is online, otherwise false
     */
    bool online() const;

    /*!
      \brief Sets gender of a friend.

      \param val gender of a friend
     */
    void setGender(const QString& val);

    /*!
      \brief Gets gender of a friend.

      \return gender of a friend
     */
    const QString gender() const;

    /*!
      \brief Sets birthday of a friend.

      \param val birthday of a friend
     */
    void setBirthday(const QString& val);

    /*!
      \brief Gets birthday of a friend.

      \return birthday of a friend
     */
    const QString birthday() const;

    /*!
      \brief Sets mobile phone number of a friend.

      \param val mobile phone number of a friend
     */
    void setMobilePhone(const QString& val);

    /*!
      \brief Gets mobile phone number of a friend.

      \return mobile phone number of a friend
     */
    const QString mobilePhone() const;

    /*!
      \brief Sets home phone number of a friend.

      \param val home phone number of a friend
     */
    void setHomePhone(const QString &val);

    /*!
      \brief Gets home phone number of a friend.

      \return home phone number of a friend
     */
    const QString homePhone() const;

    /*!
      \brief Sets city of a friend.

      \param val city of a friend
     */
    void setCity(const QString& val);

    /*!
      \brief Gets city of a friend.

      \return city of a friend
     */
    const QString city() const;

    /*!
      \brief Sets country of a friend.

      \param val country of a friend
     */
    void setCountry(const QString &val);

    /*!
      \brief Gets country of a friend.

      \return country of a friend
     */
    const QString country() const;

    /*!
      \brief Gets path to friend's avatar.

      \return path to friend's avatar
     */
    const QString icon() const;

    /*!
      \brief Sets friend's status message.

      \param friend's status message
     */
    void setStatusMessage(const QString& message);

    /*!
      \brief Gets friend's status message.

      \return friend's status message
     */
    const QString statusMessage() const;

    /*!
      \brief Checks that profile is a meta profile (contains more than one record).

      \return bool true if profile contains more than one record, otherwise - false
     */
    bool isMetaProfile() const;

    /*!
      \brief Checks that full profile is downloaded.

      \return bool true if full profile downloaded, otherwise - false
     */
    bool hasFullProfile() const;

    /*!
     \brief Gets list of profiles from meta profile.

     \return list of profiles from meta profile
     */
    FriendList getProfileList() const;

    /*!
      \brief Add profile for friend.

      \param fr new profile for friend

      \param isMainProfile true if this profile is main, otherwise - false
     */
    void addProfile(const Friend& fr, const bool isMainProfile);

    /*!
      \brief Check existence of profile for friend.

      \param fr friend source profile

      \return bool true if friend has profile, otherwise - false
     */
    bool hasProfile(const Friend& fr) const;

    /*!
      \brief Check existence of profile for friend.

      \param serviceId ID of service

      \param ownerId ID of friend

      \return bool true if friend has profile, otherwise - false
     */
    bool hasProfile(const QString& serviceId, const QString& ownerId) const;

    /*!
      Merges two lists, creates metaprofiles for duplicate items.

      \param src1 The first list

      \param src2 The second list

      \return New friend list
     */
    static FriendList mergeLists(FriendList src1, FriendList src2);

    /*!
     \brief Gets friend name as a combination of first, last and nick names.

     \return friend name as a combination of first, last and nick names
    */
    const QString name() const;

    /*!
     \brief Loads album list from cache file.

     \return album list
     */
    AlbumList getAlbumList() const;

    /*!
     \brief Sets album list and store it to cache file.

     \param list album list

     \param accountId ID of account to which friend belongs

     \param friendId ID of album master friend
     */
    static void setAlbumList(const AlbumList& list, const QString& accountId, const QString& friendId);

    /*!
     \brief Loads audio list from cache file.

     \return audio list
     */
    AudioList getAudioList() const;

    /*!
     \brief Sets audio list and store it to cache file.

     \param list audio list

     \param accountId ID of account to which friend belongs

     \param friendId ID of audio master friend
     */
    static void setAudioList(const AudioList& list, const QString& accountId, const QString& friendId);

    /*!
     \brief Loads video list from cache file.

     \return video list
     */
    VideoList getVideoList() const;

    /*!
     \brief Sets video list and store it to cache file.

     \param list video list

     \param accountId ID of account to which friend belongs

     \param friendId ID of video master friend
     */
    static void setVideoList(const VideoList& list, const QString& accountId, const QString& friendId);

    /*!
     \brief Loads events list from cache file.

     \return events list
     */
    EventList getEventsList() const;

    /*!
     \brief Sets events list and store it to cache file.

     \param list events list

     \param accountId ID of account to which friend belongs

     \param friendId ID of events master friend
     */
    static void setEventsList(const EventList& list, const QString& accountId, const QString& friendId);

    /*!
     \brief Store friends data to XML format.

     \param doc root XML document

     \param src source Friend entity

     \param addOptionalInfo true if proceed XML file should contain additional fields; otherwise - false

     \return XML structure
     */
    static QDomElement toQDomElement(QDomDocument& doc, const Friend& src, const bool addOptionalInfo);

    /*!
     \brief Loads friend from XML struct.

     \param fr XML struct with friend data

     \param accountId ID of account to which friend belongs

     \param serviceId ID of service

     \return Friend
     */
    static Friend fromQDomElement(const QDomElement& fr, const QString& accountId, const QString& serviceId);

    /*!
      \brief compare names of two friends.
      */
    bool operator <(const Friend& fr1) const;

    /*!
      \brief compare names of two friends.
      */
    bool operator ==(const Friend& fr1) const;

    /*!
      \brienf assignment operator
      */
    Friend& operator=(const Friend& other);

    /*!
     \brief Stores friend data to local cache.
     */
    void storeData();

    /*!
      \brief Loads friend data from local cache.

      \param serviceId ID of service

      \param accountId ID of account to which friend belongs

      \param ownerId ID of friend

      \return new instance of Friend class
     */
    static Friend loadData(const QString& serviceId, const QString& accountId, const QString& ownerId);
};

#endif // FRIEND_H
