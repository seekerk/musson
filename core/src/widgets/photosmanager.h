#ifndef PHOTOSMANAGER_H
#define PHOTOSMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>
#include <datatypes/listmodel.h>
#include <datatypes/photoitem.h>
#include <datatypes/commentitem.h>
#include <datatypes/friend.h>
#include <datatypes/album.h>
#include <datatypes/photo.h>
#include "musson_global.h"

class musson_EXPORT PhotosManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel* model READ model NOTIFY modelChanged)
    Q_PROPERTY(ListModel* commentsModel READ commentsModel NOTIFY commentsModelChanged)
    Q_PROPERTY(QString selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(QVariant album READ album WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(int photosDownload READ photosDownload NOTIFY photosDownloadChanged)
    Q_PROPERTY(bool isDownloadCompleted READ isDownloadCompleted WRITE setDownloadCompleted NOTIFY downloadCompletedChanged)
    Q_PROPERTY(bool isPhotosUpdate READ isPhotosUpdate NOTIFY isPhotosUpdateChanged)
public:
    explicit PhotosManager(QObject *parent = 0);
    ~PhotosManager();

    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);
    ListModel* model();
    ListModel* commentsModel();

    const QString& selected() const;
    void setSelected(const QString& selectedId);

    int photosDownload() const;
    bool isDownloadCompleted() const;
    void setDownloadCompleted(bool completed);

    QVariant album();
    void setAlbum(QVariant album);

    bool isPhotosUpdate() const;

signals:
    void modelChanged();
    void selectedChanged();
    void albumChanged();
    void isPhotosUpdateChanged();
    void photoUploaded(QString filePath);
    void commentsModelChanged();
    void photosDownloadChanged();
    void downloadCompletedChanged();

    void albumCopyingFinished();

public slots:
    void updateAccounts(QString accountName, AccountList list);
    void updatePhotos(QString accountId, QString ownerId, QString albumId, PhotoList list, bool isLastUpdate);

    void refreshPhotos();
    void refreshPhotos(bool isNeedUpdate);
    void uploadPhoto(QString filePath);
    void uploaded(QString accountId, QString albumId, QString file_name, QString photoId);

    void refreshComments();
    void refreshComments(bool isNeedUpdate);
    void gotComments(QString photoId, PhotoCommentList list);
    void gotProfile(Friend profile);

    void initPhotos();
    QVariant getPhotoById(QString photoId);

    bool saveAlbum(QString albumName, QString path);
    void copyTo(const Friend& profile);

private slots:
    void copyPendingPhotosToNewAlbum(QString accountId, QString albumId);

private:
    PhotoList m_photoList;
    QSet<QString> m_downloadedPhotosIds;
    PhotoCommentList m_commentsList;
    ListModel* m_model;
    ListModel* m_commentsModel;

    QString m_selectedId;
    ServiceManager *m_sm;
    Album m_album;

    int m_photosDownload;
    bool m_isDownloadCompleted;
    bool m_isPhotosUpdate;
    QString m_albumPath;
    QString m_albumName;

    PhotoList m_pendingPhotoList; ///< List of photos that will be uploaded when new album will be created
    QAtomicInt m_pendingPhotoCount; ///< Count of photo uploads that not acked yet
};

#endif // PHOTOSMANAGER_H
