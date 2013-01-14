#include "coresettings.h"

#include <QSettings>
#include <QPixmap>
#include <QApplication>
#include <QUrl>
#if QT_VERSION >= 0x050000
#    include <QStandardPaths>
#else
#    include <QDesktopServices>
#endif

/**
  \brief File name for application settings.

  File corresponds QApplication::applicationName().
  */
#define FILE_SETTINGS "/%1_settings.xml"

CoreSettings::CoreSettings(QObject *parent)
    : QObject(parent)
{
    // configure settings
#ifdef Q_OS_SYMBIAN
    settings = new QSettings(QApplication::applicationDirPath() + QString(FILE_SETTINGS).arg(QApplication::applicationName()), QSettings::IniFormat, this);
#elif defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
    settings = new QSettings(getHomeDir() + QString(FILE_SETTINGS).arg(QApplication::applicationName()), QSettings::IniFormat, this);
#else
    settings = new QSettings(getHomeDir() + QString(FILE_SETTINGS).arg(QApplication::applicationName()), QSettings::NativeFormat, this);
#endif

    if (!settings->contains(SETTINGS_PROXY))
        settings->setValue(SETTINGS_PROXY, ProxyType::SystemProxy);
    if (!settings->contains(SETTINGS_MULTIPANEL))
    {
#if defined(Q_WS_MAEMO_5) || defined(MEEGO_EDITION_HARMATTAN)
        settings->setValue(SETTINGS_MULTIPANEL, false);
#else
        settings->setValue(SETTINGS_MULTIPANEL, true);
#endif
    }

#if defined(Q_WS_MAEMO_5) || defined(MEEGO_EDITION_HARMATTAN)
    if (!settings->contains(SETTINGS_AUTOROTATE))
    {
        settings->setValue(SETTINGS_AUTOROTATE, true);
    }
#endif

}

QString CoreSettings::getHomeDir()
{
    QString home;
#if defined(Q_OS_WIN) || defined(Q_OS_SYMBIAN)
#       if QT_VERSION >= 0x050000
            home = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
#       else
            home = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#       endif
#   endif
    if (home.isEmpty()) {
#       ifdef HOMEDIR
            home = QString(HOMEDIR);
#       else
            home = QDir::homePath();
#           ifndef Q_OS_SYMBIAN
                home += QString(LOCALDATADIR);
#           endif
#       endif
    }
    return home + QDir::separator();
}

QString CoreSettings::getFriendsIconDir(char *serviceName)
{
    return CoreSettings::getFriendsIconDir(QString::fromUtf8(serviceName, -1));
}

QString CoreSettings::getFriendsIconDir(const QString& serviceName)
{
    QString icon = CoreSettings::getHomeDir() + QString(FRIEND_ICONS_DIR) + QDir::separator() + serviceName + QDir::separator();

    return icon;
}

QString CoreSettings::getAlbumsIconDir(char *serviceName)
{
    return CoreSettings::getAlbumsIconDir(QString::fromUtf8(serviceName, -1));
}

QString CoreSettings::getAlbumsIconDir(const QString& serviceName)
{
    QString icon = CoreSettings::getHomeDir() + QString(ALBUM_ICONS_DIR) + QDir::separator() + serviceName + QDir::separator();

    return icon;
}

QString CoreSettings::getPhotoDir(char *serviceName)
{
    return CoreSettings::getPhotoDir(QString::fromUtf8(serviceName, -1));
}

QString CoreSettings::getPhotoDir(const QString& serviceName)
{
    QString baseDir = settings->value(SETTINGS_ALBUM_DIRECTORY).toString();
    if (baseDir.isEmpty()) {
#       if QT_VERSION >= 0x050000
            baseDir = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0);
#       else
            baseDir = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
#       endif
    }
    QString path = baseDir + QDir::separator() + serviceName + QDir::separator();
    return path;
}

QString CoreSettings::getVideoIconDir(char *serviceName)
{
    return CoreSettings::getVideoIconDir(QString::fromUtf8(serviceName, -1));
}

QString CoreSettings::getVideoIconDir(const QString& serviceName)
{
    QString icon = CoreSettings::getHomeDir() + QString(VIDEO_ICONS_DIR) + QDir::separator() + serviceName + QDir::separator();

    return icon;
}

QString CoreSettings::getAudioDir()
{
    QString path = CoreSettings::getHomeDir() + QString(AUDIO_FILES_DIR);

    return path;
}

QString CoreSettings::decodeString(const QString& str)
{
    QDomDocument doc;
    doc.setContent(QString("<string>") + str + QString("</string>"));
    doc.normalize();
    QString ret = doc.documentElement().firstChild().toText().nodeValue();
    return ret;
}

QString CoreSettings::getAccountDir(const QString& accountId)
{
    return CoreSettings::getHomeDir() + QDir::separator() + QString("accounts") + QDir::separator() + (accountId.isEmpty() ? "" : accountId + QDir::separator());
}

QString CoreSettings::getDriversDir()
{
    return QString(DRIVERS_DIR);
}

QString CoreSettings::getDataDir()
{
    QString path;

#if defined(Q_OS_WIN) || defined(Q_OS_SYMBIAN)
        path = QCoreApplication::applicationDirPath();
#   else
#       ifdef DATAPREFIX
            path = QString(DATAPREFIX);
#       else
#           if QT_VERSION >= 0x050000
                path = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
#           else
                path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#           endif
#       endif
#   endif

    return path;
}

void CoreSettings::RemoveDirectoryThread(QDir &aDir)
{
    CoreSettings::RemoveDirectory(aDir);
}

bool CoreSettings::RemoveDirectory(QDir &aDir)
{
    bool has_err = false;    
    if (aDir.exists()) { //QDir::NoDotAndDotDot
        QFileInfoList entries = aDir.entryInfoList(QDir::NoDotAndDotDot |
                                                   QDir::Dirs | QDir::Files);
        int count = entries.size();
        for (int idx = 0; ((idx < count) && (false == has_err)); idx++) {
            QFileInfo entryInfo = entries[idx];
            QString path = entryInfo.absoluteFilePath();
            if (entryInfo.isDir()) {
                QDir dir(path);
                has_err = RemoveDirectory(dir);
            } else {
                QFile file(path);
                if (!file.remove())
                    has_err = true;
            }
        }
        if (!aDir.rmdir(aDir.absolutePath()))
            has_err = true;
    }
    return(has_err);
}

bool CoreSettings::RemoveFiles(QDir &aDir, const QString& filter)
{
    bool has_err = false;
    aDir.setNameFilters(QStringList(filter));
    if (aDir.exists()) {
        QFileInfoList entries = aDir.entryInfoList(QDir::NoDotAndDotDot |
                                                   QDir::Dirs | QDir::Files);
        int count = entries.size();
        for (int idx = 0; ((idx < count) && (false == has_err)); idx++) {
            QFileInfo entryInfo = entries[idx];
            QString path = entryInfo.absoluteFilePath();
            if (entryInfo.isDir()) {
                QDir dir(path);
                has_err = RemoveFiles(dir, filter);
            } else {
                QFile file(path);
                if (!file.remove())
                    has_err = true;
            }
        }
    }
    return(has_err);
}

QString CoreSettings::getPhotoCommentListKey(const Photo& photo)
{
    return QString("comments_%1_%2").arg(photo.accountId()).arg(photo.photoId());
}

QString CoreSettings::getFriendKey(const QString& accountId, const QString& ownerId)
{
    return QString("friend_%1_%2").arg(accountId).arg(ownerId);
}

QSettings* CoreSettings::settings = 0;
void CoreSettings::setSettings(QSettings *settings_)
{
    settings = settings_;
}

QImage CoreSettings::adjustImage(const QImage& image, QSize newSize)
{
    QImage ret;
    if (image.height() > image.width())
        ret = image.copy(0, (image.height() - image.width()) / 2, image.width(), image.width());
    else if (image.width() > image.height())
        ret = image.copy((image.width() - image.height()) / 2, 0, image.height(), image.height());
    else
        ret = image;

    return ret.scaled(newSize.width(), newSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QString CoreSettings::getEventsIconDir(char *serviceName)
{
    return CoreSettings::getEventsIconDir(QString::fromUtf8(serviceName, -1));
}

QString CoreSettings::getEventsIconDir(const QString& serviceName)
{
    QString icon = CoreSettings::getHomeDir() + QString(EVENT_ICONS_DIR) + QString("/") + serviceName + QString("/");

    return icon;
}

QNetworkProxy CoreSettings::getProxy()
{
    QNetworkProxy proxy;

    if (settings == NULL || settings->value(SETTINGS_PROXY).toInt() == ProxyType::SystemProxy) {
        QString curProxy;
        QString curPort;
        QNetworkProxyQuery npq(QUrl(QLatin1String("http://maemo.org")));
        QList<QNetworkProxy> proxyList = QNetworkProxyFactory::systemProxyForQuery(npq);
        if (proxyList.length() > 0 && proxyList.at(0) != QNetworkProxy::NoProxy)
        {
            curProxy = proxyList.at(0).hostName();
            curPort = proxyList.at(0).port();
        } else {
            QString var(getenv("http_proxy"));
            if (var.isNull()) {
                var = getenv("all_proxy");
            }
            QRegExp regex("(http://)?(.*):(\\d*)/?");
            int pos = regex.indexIn(var);

            if(pos > -1) {
                curProxy = regex.cap(2);
                curPort = regex.cap(3);
            }
        }
        if (!curProxy.isEmpty() && !curPort.isEmpty())
        {
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(curProxy);
            proxy.setPort(curPort.toInt());
        }
    } else if (settings->value(SETTINGS_PROXY).toInt() == ProxyType::UserProxy) {
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(settings->value(SETTINGS_PROXYHOST).toString());
        proxy.setPort(settings->value(SETTINGS_PROXYPORT).toInt());
    } else {
        proxy.setType(QNetworkProxy::NoProxy);
    }

    return proxy;
}

QVariant CoreSettings::value(const QString &key, const QVariant &defaultValue) const
{
    return this->settings->value(key, defaultValue);
}

void CoreSettings::setValue(const QString &key, const QVariant &value)
{
    this->settings->setValue(key, value);
}
