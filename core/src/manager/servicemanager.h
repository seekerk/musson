/**
  \file servicemgr.h
  \brief Service Manager class description.
  \author Musson team
  \date 2011-03-23
  */
#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <QObject>
#include <QList>
#include <QFile>
#include <QDomDocument>

#ifdef TRY_FUTURE
#include "concurrent/qtconcurrentrun.h"
#else
#    include <QtConcurrentRun>
#endif

#include <QMetaType>
#include <QHash>
#include <QDomNode>
#include <QtNetwork/QNetworkProxy>
#include <QUrl>
#include <QMutex>

#if QT_VERSION >= 0x040700
#include <QElapsedTimer>
#else
#include <QTimer>
#endif

#ifdef TRY_FUTURE
#define MSSFuture MFuture
#else
#define MSSFuture QFuture
#endif

#include "datatypes/account.h"
#include "datatypes/driverinfo.h"
#include "datatypes/errormessage.h"
#include "datatypes/message.h"
#include "datatypes/qeventfeed.h"
#include "datatypes/qstatinfo.h"
#include "datatypes/place.h"
#include "datatypes/checkin.h"
#include "datatypes/event.h"
#include "utils/coresettings.h"
#include "musson_global.h"
#include "interface.h"

class ServiceFilter;
class FilterManager;
class DataCache;

namespace UpdateType {
enum UpdateType {
    MUST_UPDATE,
    CAN_UPDATE,
    NOT_UPDATE
};
}

/**
  \brief File name for list of accounts storage.
  */
#define FILE_ACCOUNTS_LIST "/accounts.xml"

/**
  \brief File name for application settings.

  File corresponds QApplication::applicationName().
  */
#define FILE_SETTINGS "/%1_settings.xml"

/**
  \brief No proxy settings item string value.
  */
#define NO_PROXY "no-proxy"

/**
  \brief System proxy settings item string value.
  */
#define SYSTEM_PROXY "system-proxy"

/**
  \brief User manual proxy settings item string value.
  */
#define USER_PROXY "user-proxy"

#define NODE_ACCOUNTS_ROOT "profiles"
#define NODE_ACCOUNT "profile"

#define SETTINGS_PROXY "proxy"
#define SETTINGS_PROXYPORT "proxy-port"
#define SETTINGS_PROXYHOST "proxy-host"
#define SETTINGS_AUTOROTATE "auto-rotate"
#define SETTINGS_MULTIPANEL "multipanel"
#define SETTINGS_ALBUM_DIRECTORY "album-directory"
#define SETTINGS_LIGHT_THEME "light-theme"
#define SETTINGS_HIDE_SB "hide-sb"
#define SETTINGS_GROUP_FEEDS "group-feeds-by-author"

/**
 \brief The service manager class.

 This class allows to access combined data from various services.
 All requested data are stored into cache and can be used many times.
 */
class musson_EXPORT ServiceMgr : public QObject
{
    Q_OBJECT

public:
    /**
      \brief Default constructor.

      \param parent Pointer to parent object.
      */
    ServiceMgr(QObject *parent = 0);

    /**
      \brief A copy constructor.

      \param src Copied object.
      */
    ServiceMgr(const ServiceMgr& src);

    /**
      \brief Destructor.
      */
    ~ServiceMgr();

    /**
      \brief Gets owner profile.

      Function searches profile in downloaded friend lists and if it not found then try to receive from service.

      \param accountId The account ID.

      \param ownerId The owner ID on service.

      \param isNeedUpdate True if profile must to be updated, false if it has not.

      \param useSignal True if return must also be send as signal, otherwise - false.

      \return Owner profile stored in the cache.

      \sa updateProfile
      */
    Friend getProfile(const QString& accountId, const QString& ownerId, const bool isNeedUpdate, const bool useSignal, const bool isFullProfile);

    /**
      \brief Gets owner profile of comment author.

      Function searches profile in downloaded friend lists and if it not found then try to receive from service.

      \param comment The PhotoComment instance.

      \param isNeedUpdate True if profile must to be updated, false if it has not.

      \param useSignal True if return must also be send as signal, otherwise - false.

      \return Owner profile stored in the cache.

      \sa updateProfile
      */
    Friend getProfile(const PhotoComment& comment, const bool isNeedUpdate, const bool useSignal);

    Friend getMyProfile(bool isNeedUpdate);

    /**
     \brief Get list of friends.

     \param isNeedUpdate True if need to get new friend list, otherwise - false.

     \param useSignal True if return must also be send as signal, otherwise - false.

     \return Stored list of friends. Newest list will be sent using \sa updateFriends signal.
     */
    FriendList getFriends(bool isNeedUpdate, bool useSignal);

    /**
     \brief Gets list of friend albums.

     \param curFriend Instance of \sa Friend class contains friend properties.

     \param isNeedUpdate True if need to get new album list, otherwise - false.

     \param useSignal True if return must also be send as signal, otherwise - false.

     \return Stored list of albums. Newest list will be sent using \sa updateAlbumList signal.
    */
    AlbumList getAlbums(const Friend& curFriend, const bool isNeedUpdate, const bool useSignal);

    /**
      \brief Gets list of friend albums.

      \param al The friend album which contained in the list.

      \param isNeedUpdate True if need to get new album list, otherwise - false.

      \param useSignal True if return must be sended as signal, otherwise - false.

      \return Stored list of albums. Newest list will be sent using \sa updateAlbumList signal.
      */
    AlbumList getAlbums(const Album& al, const bool isNeedUpdate, const bool useSignal);

    /**
     \brief Gets list of owner albums.

     \param isNeedUpdate True if need to get new album list, otherwise - false.

     \param useSignal True if return must be sended as signal, otherwise - false.

     \return Stored list of albums. Newest list will be sent using \sa updateAlbumList signal.
    */
    AlbumList getAlbums(bool isNeedUpdate, const bool useSignal);

    /**
     \brief Get content of album.

     The function checks album content size and the stored image list size.
     If these sizes not equal, then oldest items (list of albums or list of images fro specified album) will be updated.

     \param curAlbum The Album instance.

     \param isNeedUpdate True if need to get new image list, otherwise - false.

     \param useSignal True if return must be sended as signal, otherwise - false.

     \return Stored list of images. Newest list will be sent using \sa updatePhotoList signal.
     */
    PhotoList getPhotos(const Album& curAlbum, const bool isNeedUpdate, const bool useSignal, const bool loadIcons);

    /**
     \brief Get content of album.

     The function checks album content size and the stored image list size.
     If these sizes not equal, then oldest items (list of albums or list of images fro specified album) will be updated.

     \param curAlbum The Photo instance.

     \param isNeedUpdate True if need to get new image list, otherwise - false.

     \param useSignal True if return must be sended as signal, otherwise - false.

     \return Stored list of images. Newest list will be sent using \sa updatePhotoList signal.
     */
    PhotoList getPhotos(const Photo& curPhoto, const bool isNeedUpdate, const bool useSignal, const bool loadIcons);

    /**
     \brief Get list of comments for specified image.

     \param curPhoto The Photo instance of specified image.

     \param isNeedUpdate True if need to get new list, otherwise - false.

     \param useSignal True if return must be sended as signal, otherwise - false.

     \return Stored list of image comments. Newest list will be sent using \sa updatePhotoCommentList signal.
      */
    PhotoCommentList getComments(const Photo& curPhoto, const bool isNeedUpdate, const bool useSignal);

    /**
      \brief Send owner comment to specified image.

      \param curPhoto The Photo instance of commented image.

      \param comment The text of comment.

      \return True if comment sent successfully, otherwise - false.
      */
    bool sendComment(const Photo& curPhoto, const QString& comment);

    /**
      \brief Get list of messages.

      \param isNeedUpdate True if need to get new message list, otherwise - false.

      \param useSignal True if return must be sended as signal, otherwise - false.

      \return Stored list of messages from Inbox folder. Newest list will be sent using \sa updateInboxMessageList signal.
      */
    MessageList getMessages(const bool isNeedUpdate, const bool useSignal);

    /*!
      \brief Delete message from service.

      If message was deleted successfully then signal \a messageDeleted() was called.
      Otherwise signal \a errorOccured() was called.

      \sa QTransport::deleteMessage()

      \param msg Deleted message.
      */
    void deleteMessage(const Message& msg);

    /*!
      \brief Delete message from service.

      If message was deleted successfully then signal \a messageDeleted() was called.
      Otherwise signal \a errorOccured() was called.

      \sa QTransport::deleteMessage()

      \param accountId The ID of account.

      \param messageId The ID of deleted message.
      */
    void deleteMessage(const QString& accountId, const QString& messageId);

    /*!
      \brief Checks that service can send message.

      \param accountId The ID of account.

      \return true if service can send message, otherwise - false.
      */
    bool canSendMessage(const QString& accountId);

    /**
      \brief Sends private message to friend on service.

      If message was sent successfully then signal \a messageSent() was called.
      Otherwise signal \a errorOccured() was called.

      \param accountId ID of account.

      \param ownerId The ID of message recepient.

      \param title The message title.

      \param message The message.
      */
    bool sendMessage(const QString& accountId, const QString& ownerId,
                     const QString& title, const QString& message);

    /*!
     \brief Marks message as read.

     The success result will be sent by \a messageReaded() signal.
     Otherwise the signal \a errorOccured() was called.

     \param accountId ID of account to which message belongs.

     \param messageId ID of message.
     */
    void readMessage(const QString& accountId, const QString& messageId);

    /**
      \brief Gets list of accounts.

      \return List of accounts.
      */
    AccountList getAccounts();

    /**
      \brief Create new account and adds it to list.

      \param accountName The name of account.

      \param driver Selected driver.
      */
    void addAccount(QString accountName, const DriverInfo& driver);

    /*!
      \brief emit update signal for this account

      \param acc Updated account.
      */
    void updateAccount(Account *acc);

    /**
      \brief Remove account from list.

      This function removes account for a list of accounts and removes account's data form cache.

      \param oldAcc Pointer to removed account.
      */
    void deleteAccount(Account *oldAcc);

    void removeAccountData(Account *oldAcc);

    /**
      \brief Gets Account instance for account ID.

      \param accountId The Account ID.

      \return Pointer to Account instance.
      */
    Account *account(QString accountId);

    /**
      \brief Download images from service.

      Download selected photo and list of nearest images. The downloaded photos returns by signal \a QTransport::photosReceived();

      \param startPhoto The Photo instance with image info and URL to image from service.

      \param nearest The number of nearest images. Function downloads form (startPhoto - nearest) to (startPhoto + nearest) images.
      */
    void downloadPhotos(Photo startPhoto, int nearest);

    QString downloadPhoto(Photo curPhoto);

    bool canUploadPhoto(const QString &accountId);

    /**
      \brief Upload image to service.

      Function uploads the file with image to selected service. The result returns by signal \a QTransport::photoUploaded()

      \param accountId The name on account.

      \param albumId The identificator of album.

      \param file The path to file with image.

      \param description The description of uploaded image.
      */
    void uploadPhoto(QString accountId, QString albumId, QString file, QString description);

    /**
      \brief Upload image to service.

      Function uploads the file with image to selected service.

      \param album Album instance with selected album info.

      \param file The path to file with image.

      \param description The description of uploaded image.
      */
    void uploadPhoto(const Album album, QString file, QString description);

    /**
     \brief Gets list of friend audio files.

     \param curFriend Instance of \sa Friend class contains friend properties.

     \param isNeedUpdate True if need to get new audio list, otherwise - false.

     \param useSignal True if return must also be send as signal, otherwise - false.

     \return Stored list of audio files. Newest list will be sent using \sa updateAudioList signal.
    */
    AudioList getAudio(const Friend& curFriend, const bool isNeedUpdate, const bool useSignal);

    /**
     \brief Gets list of friend audio files.

     \param aud The friend audio which contained in the list.

     \param isNeedUpdate True if need to get new audio list, otherwise - false.

     \param useSignal True if return must be sended as signal, otherwise - false.

     \return Stored list of audio files. Newest list will be sent using \sa updateAudioList signal.
    */
    AudioList getAudio(const Audio& aud, const bool isNeedUpdate, const bool useSignal);

    /**
     \brief Gets list of owner audio files.

     \param isNeedUpdate True if need to get new audio list, otherwise - false.

     \param useSignal True if return must be sended as signal, otherwise - false.

     \return Stored list of audio files. Newest list will be sent using \sa updateAudioList signal.
    */
    AudioList getAudio(bool isNeedUpdate, const bool useSignal);

    /**
     \brief Gets list of friend video files.

     \param curFriend Instance of \sa Friend class contains friend properties.

     \param isNeedUpdate True if need to get new video list, otherwise - false.

     \param useSignal True if return must also be send as signal, otherwise - false.

     \return Stored list of video files. Newest list will be sent using \sa updateVideoList signal.
    */

    VideoList getVideo(const Friend& curFriend, const bool isNeedUpdate, const bool useSignal);

    /**
      \brief Gets list of friend video files.

      \param vid The friend video which contained in the list.

      \param isNeedUpdate True if need to get new video list, otherwise - false.

      \param useSignal True if return must be sended as signal, otherwise - false.

      \return Stored list of video files. Newest list will be sent using \sa updateVideoList signal.
      */
    VideoList getVideo(const Video& vid, const bool isNeedUpdate, const bool useSignal);

    /**
     \brief Gets list of owner video files.

     \param isNeedUpdate True if need to get new video list, otherwise - false.

     \param useSignal True if return must be sended as signal, otherwise - false.

     \return Stored list of video files. Newest list will be sent using \sa updateVideoList signal.
    */
    VideoList getVideo(bool isNeedUpdate, const bool useSignal);

    /**
     * @brief describes type of used proxy.
     *
     */

    enum ProxyType {
        NoProxy, /**< \brief No proxy settings exists. */
        SystemProxy,  /**< \brief Proxy settings gets from system environment. */
        UserProxy /**< \brief User defined proxy settings. */
    };

    /**
      \brief Application settings storage.

      Store settings into FILE_SETTINGS file.
      */
    CoreSettings *settings;

    /**
      \brief Gets list of supported drivers.

      Function scans Musson drivers folder and try to load each driver.
      If it loaded successfully then driver description adds into list.

      \return List of pointers to \sa DriverInfo instances with driver descriptions.
      */
    QList<DriverInfo> getDrivers();

    /**
      \brief Load image list for selected album.

      Function loads image list for selected album from cache.

      \param curPhoto Instance of Photo class contained data of image from selected album.

      \return Stored image list for selected album.
      */
    PhotoList getPhotosForAlbum(Photo curPhoto);

    /**
      \brief Create new album.

      \param accounId ID of owner account.
      \param name Name of album.
      \param description Description for album.
      \param privacy Type of privacy settings.

      \return true if album was created successfully, else false.
      */
    bool createAlbum(QString accountId, QString name, QString description, QString privacy);

    /**
      \brief Load image list for selected album.

      Function loads image list for selected album from cache.

      \param curAcc Pointer to selected account.

      \param curPhoto Instance of Photo class contained data of image from selected album.

      \return Stored image list for selected album.
      */
    PhotoList getPhotosForAlbum(Account *curAcc, Photo curPhoto);

    /**
      \brief Clear Musson project cache storage.

      Remove download data expect profiles and account settings.
      */
    void clearCache();

    /**
      \brief Get news list for selected type.

      The updated feed will be sent using \a updateEventFeed.

      \param type The type of feed.

      \return The cached news feed.
      */
    QEventFeedList getFeed(QEventFeed::FeedType type, const bool isNeedUpdate, const bool useSignal);

    int getNumThreadsFriendUpdate() const { return this->friendsUpdate;}

    int getNumThreadsAlbumUpdate() const { return this->albumsUpdate;}

    int getNumThreadsPhotoUpdate() const { return this->photosUpdate;}

    int getNumThreadsAudioUpdate() const { return this->audioUpdate;}

    int getNumThreadsVideoUpdate() const { return this->videoUpdate;}

    int getNumThreadsFeedUpdate() const { return this->feedsUpdate;}

    FilterManager* filterManager() const;
    ServiceFilter* serviceFilter() const;
    DataCache* dataCache() const;

    bool canUpdateFriends; /**< \brief Service manager can start friend List update independently. */

    bool canUpdateAlbums; /**< \brief Service manager can start album List update independently. */

    bool canUpdateMessages; /**< \brief Service manager can start message List update independently. */

    bool canUpdateFeeds; /**< \brief Service manager can start event feed update independently. */

    bool canUpdateEvents; /**< \brief Service manager can start event feed update independently. */

    //! Get list of statistics.
    QList<QStatInfo> getStats() const;

    /**
     \brief Search places.

     \param text String of search query.

     \param latitude

     \param longitude

     \param useSignal True if return must be sended as signal, otherwise - false.

     \return Stored list of places
      */
    PlaceList searchPlaces(QString text, QString latitude, QString longitude, QString radius);

    /**
     \brief get checkins.

     \param placeId The Checkin instance

     \param latitude

     \param longitude

     \param useSignal True if return must be sended as signal, otherwise - false.

     \return Stored list of checkins
      */
    CheckinList getCheckins(QString placeId, QString latitude, QString longitude, QString ownerId, const int type, const bool useSignal);

    /**
     \brief Create new place

     \param title - name of place.

     \param latitude - geographic latitude of new place.

     \param longitude - geographic longitude of new place.

     \param useSignal True if return must be sended as signal, otherwise - false.

     \return Stored list of places
      */
    bool createPlace(QString text, QString latitude, QString longitude, QString type, QString address, QString comment, bool isCheckin);

    /*!
     \brief Checkin in place.

     \param placeId - identifier place in which it should be checkin.

     \param text - string of comment.
     */
    bool checkIn(QString placeId, QString comment);

    /*!
      \brief Create new event.

      \param title - name of event.

      \param description - description of new event.

      \param startTime - time when event will start (in ISO_8601 or Unix_timestamp format).

      \param endTime - time when event will end (in ISO_8601 or Unix_timestamp format).

      \param location - geographic location of new event.

      \param privacy - degree of event privacy ("OPEN", "CLOSED", "SECRET").

      \return true if event was created successfully, else false.
    */

    bool createEvent(QString title, QString description, QString startTime, QString endTime, QString location, QString privacy);

    /*!
      \brief Gets list of events.

      \param ownerId - user - owner of list

      \param startTime - date since when need to get events

      \param useSignal True if return must be sended as signal, otherwise - false.

      \return Stored list of events.
    */

    //EventList getEvents(QString ownerId, const bool isNeedUpdate, const bool useSignal);
    EventList getEvents(bool isNeedUpdate, bool useSignal);

    EventList getEvents(const Friend& curFriend, const bool isNeedUpdate, const bool useSignal);

    EventList getEvents(const Event& ev, const bool isNeedUpdate, const bool useSignal);


    /**
      \brief Gets a list of active requests.

      \return The list of internal names of active requests from \a threads.
      */
    QList<QString> activeRequests();

    /**
      \brief Check whether given friend id belongs to local account
      \param accountId Friend id
      \return true if account local, false otherwise
      */
    bool isLocalFriend(const QString& friendId);

    /**
      \brief Make ServiceManager load \a count messages on next getMessages call
      */
    void allowDownloadingOldMessages(const QString& accountId, const QString& friendId);

    /**
      \brief This function emits \a settingsChanged() signal.
      */
    void updateSettings();

    void audioFinish();

private:
    /**
     \brief List of user accounts.
     */
    AccountList accounts;

    /**
     \brief List of threads that currently running.
     */
    QHash<QString, MSSFuture<void> > threads;

    /**
     * @brief Load accounts from localdatadir/FILE_ACCOUNTS_LIST.
     *
     */
    void loadAccounts();

    /**
     * @brief Load settings from localdatadir/FILE_SETTINGS.
     *
     */
    void loadSettings();

    /**
     * @brief Store settings to localdatadir/FILE_SETTINGS.
     *
     */
    void storeSettings();

    /**
     * @brief Remove unused threads from \a threads.
     *
     */
    void cleanThreads();

    /**
     * @brief Connects own slots to transport signals.
     *
     * @param account The pointer to Account instance.
     */
    void connectToTransport(Account *account);

    int friendsUpdate; /**< \brief Number of friends update processes. */

    int albumsUpdate; /**< \brief Number of albums update processes. */

    int photosUpdate; /**< \brief Number of images update processes.  */

    int audioUpdate; /**< \brief Number of audio update processes. */

    int videoUpdate; /**< \brief Number of video update processes. */

    int feedsUpdate; /**< \brief Number of feeds update processes.  */

    int messagesUpdate; /**< \brief Number of Inbox folder messages update processes.  */

    int eventsUpdate; /**< \brief Number of events update processes. */

    bool isSkipFriendListUpdate; /**< \brief Skip unwanted friend list update flag. */
    FriendList cachedFriendList; /**< \brief The cached friend list. Used for optimize sorting and merging processes. */
    QMutex mutexFriends; /**< \brief Locker for all friends objects. */
    void createCacheFriends(UpdateType::UpdateType upd);

    QList<DriverInfo> cachedDriverList; /**< \brief The cached drivers list. */

    DataCache* dataCache_;
    MessageList cachedMessageList; /**< \brief The cached list of messages. Used for optimize sorting and merging processes. */
    QMutex mutexMessage; /**< \brief Locker for all message ojbects. */
    void createCacheMessages(UpdateType::UpdateType upd);

    QHash<QEventFeed::FeedType, QEventFeedList> cachedNewsList;
    void createCacheNews(UpdateType::UpdateType upd, QEventFeed::FeedType type);

    bool isSkipEventListUpdate; /**< \brief Skip unwanted event list update flag. */
    QMutex mutexEvents; /**< \brief Locker for all event ojbects. */
    EventList cachedEventList; /**< \brief The cached list of events. Used for optimize sorting and merging processes. */
    void createCacheEvents(UpdateType::UpdateType upd);

    QMutex mutex;
    
    // Filters
    ServiceFilter* serviceFilter_;
    FilterManager* filterManager_;
    void createFilter();
    void loadWebauth();

#if QT_VERSION >= 0x040700
QElapsedTimer timer;
#else
QTime timer;
#endif

signals:
    /**
     * @brief Update friends list signal.

     \sa getFriends()
     *
     * @param list The list of friends.

     * @param isLastUpdate True if it is a last signal, otherwise - false.
     */
    void updateFriends(FriendList list, bool isLastUpdate);

    /**
     * @brief Update album list signal

     \sa getAlbums()
     *
     * @param owner The albums owner.

     * @param list The list of albums.

     * @param isLastUpdate True if it is a last signal, otherwise - false.
     */
    void updateAlbumList(Friend owner, AlbumList list, bool isLastUpdate);

    /**
     * @brief Update album content signal.

     This signal occurs when list of images was download successfully.
     *
     \sa getPhotos()

     \param accountId The ID of account.

     \param ownerId The ID of album owner.

     * @param albumId The album ID.

     * @param list The list of images.

     * @param isLastUpdate True if it is last signal, otherwise - false.
     */
    void updatePhotoList(QString accountId, QString ownerId, QString albumId, PhotoList list, bool isLastUpdate);

    /**
     * @brief Update List of image comments signal.

     This signal sent when list of comments was download successfully.

       \sa getComments()
     *
     * @param photoId The image ID.

     * @param list The list of comments.
     */
    void updatePhotoCommentList(QString photoId, PhotoCommentList list);

    /**
     * @brief Update message folder signal.
     *
     This signal sent when content of message folder was download successfully.

     \sa getMessages()

     * @param list The list of messages.

     * @param isLastUpdate True if it is last signal, otherwise - false.
     */
    void updateMessageList(MessageList list, bool isLastUpdate);

    /**
     * @brief Update account list signal.
     *
     This signal sent when new list of friends was download successfully.

     \sa addAccount()
     \sa getProfile()
     \sa deleteAccount()

     * @param list The new list of accounts.
     */
    void updateAccounts(QString accountId, AccountList list);

    void updateAudioList(Friend owner, AudioList list, bool isLastUpdate);

    void updateVideoList(Friend owner, VideoList list, bool isLastUpdate);

    /**
      \brief Error occured signal.

      This signal sends when occured an error during request to driver or other action.

      \param errMsg The text of error message.

      \param action Describes action in which error occured.

      \param isMajor True if error is major and must be shown to user; false if error not major and uses to inform user.
      */
    void errorOccured(QString errMsg, Driver::Action action, ErrorMessage msg, bool isMajor);

    /**
     * @brief Update profile signal.

     \sa getProfile()
     *
     * @param profile The new profile.
     */
    void updateProfile(Friend profile);

    void photoUploaded(QString accountId, QString albumId, QString local_file_name, QString photoId);

    void messageSent(QString accountId, QString ownerId, QString title, QString message);

    void albumCreated(QString accountId, QString albumId);

    /**
      \brief Message was deleted signal.

      This signal sends when message was deleted successfully.

      \sa QTransport::messageDeleted()

      \param messageId The id of deleted message.
      */
    void messageDeleted(QString accountId, QString messageId);

    /**
      \brief Message was readed signal.

      This signal sends when message was readed successfully.

      \sa QTransport::messageReaded()

      \param accountId THe ID of account.

      \param messageId The ID of readed message.
      */
    void messageReaded(QString accountId, QString messageId);

    /**
      \brief Update feed of events signal.

      This signal sends when feed of events was updated.

      \sa QTransport::eventFeedReceived

      \param list The list of events in the feed.

      \param type The type of feed.
      */
    void updateFeed(QEventFeedList list, QEventFeed::FeedType type, bool isLastUpdate);

    /**
      \brief Update list of places signal.

      This signal sends when place was updated.

      \sa QTransport::checkinsReceived

      \param list The list of places
      */
    void updatePlaceList(PlaceList list);

    /**
      \brief Update list of places signal.

      This signal sends when checkins was updated.

      \sa QTransport::placesReceived

      \param placeId The place id.

      \param list The list of checkins in place.
      */
    void updateCheckinList(QString placeId, CheckinList list, const int type);

    /**
      \brief Update list of events signal.

      This signal sends when events was updated.

      \sa QTransport::eventsReceived

      \param list The list of events

      \param isLastUpdate True if it is last signal, otherwise - false.
    */
    void updateEventList(Friend owner, EventList list, bool isLastUpdate);

    void statsUpdated();

    /**
      \brief Indicates is messages loading now or not
      \param state true if loading false if not
      */
    void messageLoadingStateChanged(bool state);
    void settingsChanged();

    void audioStop();

private slots:
    void gotFriends(QString accountid, FriendList, bool isLastUpdate);

    void gotAlbumList(QString accountid, QString friendId, AlbumList, bool isLastUpdate);

    void gotPhotoList(QString accountid, QString friendId, QString albumId, PhotoList, bool isLastUpdate);

    void gotMessageList(QString accountId, MessageList, bool isLastUpdate);

    void gotProfile(QString accountId, QString reqOwnerId, Friend profile);

    void gotComments(QString accountId, QString ownerId, QString albumId, QString photoId, PhotoCommentList list);

    void gotPlaces(PlaceList list);

    void gotCheckins(QString placeId, CheckinList list, const int type);

    void gotAudioList(QString accountid, QString friendId, AudioList, bool isLastUpdate);

    void gotVideoList(QString accountid, QString friendId, VideoList, bool isLastUpdate);

    void gotEventsList(QString accountId, QString friendId, EventList list, bool isLastUpdate);

    void storeAccounts(QString accountId, AccountList list);

    /**
      \brief Stores new events feed and call signal \a updateEventFeed().

      This slot attached to \a QTransport::eventFeedReceived().
     */
    void gotEventFeed(QString accountId, QEventFeedList list,
                      QEventFeed::FeedType type, bool isLastUpdate);

    /**
      \brief Update settings for each account's driver (for example, proxy settings).

      Settings gets from \sa settings storage.
      */
    void updateDriverSettings();

    void gotErrorMsg(QString accountId, ErrorMessage msg, Driver::Action acc);

    void gotMessageReaded(const QString& accountId, const QString& messageId);

    void gotMessageDeleted(const QString& accountId, const QString& messageId);

    void gotMessageSent(QString accountId, QString ownerId, QString title, QString text);

    void gotAlbumCreated(const QString& accountId, const QString& albumId);

public slots:
    void updateData(int type = -1);

    void stopMessageLoading();

    /**
     * @brief
     *
     */
    void disableSkipFriendListUpdate();
};

Q_DECLARE_METATYPE(ServiceMgr::ProxyType)

#endif // SERVICEMGR_H
