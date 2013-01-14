#ifndef _SERVICE_MANAGER_H_
#define _SERVICE_MANAGER_H_
#include <QObject>

#include <QSettings>
#include "datatypes/driverinfo.h"
#include "utils/coresettings.h"
#include "datatypes/account.h"
#include "datatypes/friend.h"
#include "datatypes/message.h"
#include "datatypes/qstatinfo.h"
#include "datatypes/qeventfeed.h"
#include "datatypes/place.h"
#include "datatypes/checkin.h"
#include "filter/filtermanager.h"

class ServiceFilter;

class ServiceManager : public QObject
{
    Q_OBJECT

public:
    /**
      \brief Default constructor.

      \param parent Pointer to parent object.
      */
    ServiceManager(QObject *parent = 0);

    /**
      \brief A copy constructor.

      \param src Copied object.
      */
    ServiceManager(const ServiceManager& src);

    /**
      \brief Destructor.
      */
    ~ServiceManager();

    /**
      \brief Gets list of supported drivers.

      Function scans Musson drivers folder and try to load each driver.
      If it loaded successfully then driver description adds into list.

      \return List of pointers to \sa DriverInfo instances with driver descriptions.
      */
    QList<DriverInfo> getDrivers();

    /**
      \brief Create new account and adds it to list.

      \param accountName The name of account.

      \param driver Selected driver.
      */
    void addAccount(QString accountName, const DriverInfo& driver);

    /**
      \brief Remove account from list.

      This function removes account for a list of accounts and removes account's data form cache.

      \param oldAcc Pointer to removed account.
      */
    void deleteAccount(Account oldAcc);

    /**
      \brief Gets list of accounts.

      \return List of accounts.
      */
    AccountList getAccounts();

    /**
      \brief Gets Account instance for account ID.

      \param accountId The Account ID.

      \return Pointer to Account instance.
      */
    Account account(QString accountId);

    bool hasAccount(QString accountId);

    CoreSettings* settings() {return mSettings;}

    /**
      \brief Clear Musson project cache storage.

      Remove download data expect profiles and account settings.
      */
    void clearCache();

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
      \brief Create new album.

      \param accounId ID of owner account.
      \param name Name of album.
      \param description Description for album.
      \param privacy Type of privacy settings.

      \return true if album was created successfully, else false.
      */
    bool createAlbum(QString accountId, QString name, QString description, QString privacy);

    /**
     \brief Get content of album.

     The function checks album content size and the stored image list size.
     If these sizes not equal, then oldest items (list of albums or list of images fro specified album) will be
updated.

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
      \brief Download images from service.

      Download selected photo and list of nearest images. The downloaded photos returns by signal \a QTransport:
:photosReceived();

      \param startPhoto The Photo instance with image info and URL to image from service.

      \param nearest The number of nearest images. Function downloads form (startPhoto - nearest) to (startPhoto
 + nearest) images.
      */
    void downloadPhotos(Photo startPhoto, int nearest);

    QString downloadPhoto(Photo curPhoto);

    /**
      \brief Load image list for selected album.

      Function loads image list for selected album from cache.

      \param curPhoto Instance of Photo class contained data of image from selected album.

      \return Stored image list for selected album.
      */
    PhotoList getPhotosForAlbum(Photo curPhoto);

    /**
      \brief Load image list for selected album.

      Function loads image list for selected album from cache.

      \param curAcc Pointer to selected account.

      \param curPhoto Instance of Photo class contained data of image from selected album.

      \return Stored image list for selected album.
      */
    PhotoList getPhotosForAlbum(Account *curAcc, Photo curPhoto);

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
    void sendComment(const Photo& curPhoto, const QString& comment);

    /**
      \brief Get list of messages.

      \param isNeedUpdate True if need to get new message list, otherwise - false.

      \param useSignal True if return must be sended as signal, otherwise - false.

      \return Stored list of messages from Inbox folder. Newest list will be sent using \sa updateInboxMessageLi
st signal.
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

    /**
      \brief Get news list for selected type.

      The updated feed will be sent using \a updateEventFeed.

      \param type The type of feed.

      \return The cached news feed.
      */
    QEventFeedList getFeed(QEventFeed::FeedType type, const bool isNeedUpdate, const bool useSignal);

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

    ServiceFilter* serviceFilter() const;

    FilterManager* filterManager() const;

    //! Get list of statistics.
    QList<QStatInfo> getStats() const;

private:
    QList<DriverInfo> cachedDriverList;

    CoreSettings *mSettings;

private slots:
    void updateAccount(QString accountId);
};

#endif /* _SERVICE_MANAGER_H_ */
