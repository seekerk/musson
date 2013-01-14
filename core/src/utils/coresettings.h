#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDir>
#include <QNetworkProxy>
#include <QVariant>

#include "datatypes/photo.h"
#include "datatypes/album.h"
#include "datatypes/event.h"
#include "musson_global.h"

#define DATE_TIME_FORMAT "dd.MM.yyyy hh:mm:ss"

#define FRIEND_ICONS_DIR "friend_icons/"
#define ALBUM_ICONS_DIR "album_icons/"
#define VIDEO_ICONS_DIR "video_icons/"
#define AUDIO_FILES_DIR "audio_files/"
#define EVENT_ICONS_DIR "event_icons/"

#define SETTINGS_PROXY "proxy"
#define SETTINGS_PROXYPORT "proxy-port"
#define SETTINGS_PROXYHOST "proxy-host"
#define SETTINGS_AUTOROTATE "auto-rotate"
#define SETTINGS_MULTIPANEL "multipanel"
#define SETTINGS_ALBUM_DIRECTORY "album-directory"
#define SETTINGS_LIGHT_THEME "light-theme"
#define SETTINGS_HIDE_SB "hide-sb"
#define SETTINGS_GROUP_FEEDS "group-feeds-by-author"

#if defined(Q_OS_SYMBIAN)
#    define LIB_SUFFIX "qtplugin"
#elif defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
#    define LIB_SUFFIX "dll"
#else
#    define LIB_SUFFIX "so"
#endif

namespace ProxyType {
/**
 * @brief describes type of used proxy.
 *
 */

enum ProxyType {
    NoProxy, /**< \brief No proxy settings exists. */
    SystemProxy,  /**< \brief Proxy settings gets from system environment. */
    UserProxy /**< \brief User defined proxy settings. */
};
}


class QSettings;

/*!
  \brief Common functions and procedures

  This class contains common static functions which are used in other classes.
  */
class musson_EXPORT CoreSettings : public QObject
{
    Q_OBJECT

    static QSettings* settings;
public:
    /**
      \brief Default constructor
      */
    CoreSettings(QObject* parent);

    /**
      \brief Get home directory

      Function returns user's home directory

      \return Path to folder
      */
    static QString getHomeDir();

    /**
     \brief Get path to folder for friend icons corresponded the name of service

     \param serviceName the name of service

     \return Path to folder
    */
    static QString getFriendsIconDir(const QString& serviceName);

    /**
      \brief Get path to folder for friend icons corresponded the name of service

      \param serviceName the name of service

      \return Path to folder
      */
    static QString getFriendsIconDir(char * serviceName);

    /**
      \brief Get path to folder for album icons corresponded the name of service

      \param serviceName the name of service

      \return Path to folder
      */
    static QString getAlbumsIconDir(const QString& serviceName);

    /**
      \brief Get path to folder for album icons corresponded the name of service

      \param serviceName the name of service

      \return Path to folder
      */
    static QString getAlbumsIconDir(char * serviceName);

    /**
      \brief Get path to folder for image thumbnails corresponded the name of service

      \param serviceName the name of service

      \return Path to folder
      */
    static QString getPhotoDir(const QString& serviceName);

    /**
      \brief Get path to folder for image thumbnails corresponded the name of service

      \param serviceName the name of service

      \return Path to folder
      */
    static QString getPhotoDir(char * serviceName);

    /**
      \brief Get path to folder for video icons corresponded the name of service

      \param serviceName the name of service

      \return Path to folder
      */
    static QString getVideoIconDir(const QString& serviceName);

    /**
      \brief Get path to folder for video icons corresponded the name of service

      \param serviceName the name of service

      \return Path to folder
      */
    static QString getVideoIconDir(char * serviceName);

    /**
     \brief Get absolute path to account folder

     \param accountId the name of account

     \return Absolute path to folder
    */
    static QString getAccountDir(const QString& accountId);

    /**
     \brief Get absolute path to audio folder

     \return Absolute path to folder
    */
    static QString getAudioDir();

    /**
      \brief Decode string with XML characters

      \param str Source string with XML characters

      \return Decoded string
      */
    static QString decodeString(const QString& str);

    /**
     \brief Get path to folder of Musson drivers

     \return Absolute path to folder
    */
    static QString getDriversDir();

    /**
      \brief Recursive remove folder content

      \param aDir Folder which should be removed

      \return True if folder removes successfully; otherwise - false
      */
    static bool RemoveDirectory(QDir &aDir);

    /**
      \brief Recursive remove folder content without return any value.

      \param aDir Folder which should be removed
      */
    static void RemoveDirectoryThread(QDir &aDir);

    /**
      \brief Remove files in folder corresponded mask of file name

      \param aDir Used folder

      \param filter The string contains a mask of file which should be removed

      \return True if files removes successfully; otherwise - false
      */
    static bool RemoveFiles(QDir &aDir, const QString& filter);

    /**
      \brief Get data directory

      \return The string with path to folder
      */
    static QString getDataDir();

    /**
      \brief Generate unique key for list of comments to image.

      \param photo Selected image.

      \return String contains list of comments key.
      */
    static QString getPhotoCommentListKey(const Photo& photo);

    /**
      \brief Generate unique key for Friend instance.

      \param accountId The ID of corresponded account.

      \param ownerId Th friend ID.

      \return String contains Friend instance key.
      */
    static QString getFriendKey(const QString& accountId, const QString& ownerId);

    /**
      \brief Set settings object
      \param settings Settings object
      */
    static void setSettings(QSettings* settings);

    /**
      \brief Adjusts image to given size cutting everything that doesn't fit to size
      \return New image of size newSize
     */
    static QImage adjustImage(const QImage& image, QSize newSize);

    /**
      \brief Get path to folder for event icons corresponded the name of service

      \param serviceName the name of service

      \return Path to folder
      */
    static QString getEventsIconDir(const QString& serviceName);

    /**
      \brief Get path to folder for events icons corresponded the name of service

      \param serviceName the name of service

      \return Path to folder
      */
    static QString getEventsIconDir(char * serviceName);

    static QNetworkProxy getProxy();

    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;

    void setValue(const QString & key, const QVariant & value);
};

#endif // UTILS_H
