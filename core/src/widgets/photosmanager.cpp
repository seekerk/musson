#include "photosmanager.h"

PhotosManager::PhotosManager(QObject *parent) :
    QObject(parent)
{
    m_model = new ListModel(new PhotoItem, this);
    m_commentsModel = new ListModel(new CommentItem, this);
    m_isPhotosUpdate = false;
    m_photosDownload = -1;
    m_isDownloadCompleted = false;
}

PhotosManager::~PhotosManager()
{
    delete m_model;
    delete m_commentsModel;
}

void PhotosManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;

    connect(m_sm, SIGNAL(updateAccounts(QString,AccountList)),
            this, SLOT(updateAccounts(QString,AccountList)));
    connect(m_sm, SIGNAL(updatePhotoList(QString,QString,QString,PhotoList,bool)),
            this, SLOT(updatePhotos(QString,QString,QString,PhotoList,bool)));
    connect(m_sm, SIGNAL(photoUploaded(QString,QString,QString,QString)),
            this, SLOT(uploaded(QString,QString,QString,QString)));
    connect(m_sm, SIGNAL(updatePhotoCommentList(QString,PhotoCommentList)),
            this, SLOT(gotComments(QString,PhotoCommentList)));
    connect(m_sm, SIGNAL(updateProfile(Friend)),
            this, SLOT(gotProfile(Friend)));
    connect(m_sm, SIGNAL(albumCreated(QString,QString)),
            this, SLOT(copyPendingPhotosToNewAlbum(QString,QString)));
}

void PhotosManager::initPhotos()
{
    AccountList acc = m_sm->getAccounts();
    this->updateAccounts(QString::null, acc);
}

ServiceManager* PhotosManager::sm()
{
    return m_sm;
}

ListModel* PhotosManager::model()
{
    return m_model;
}

ListModel* PhotosManager::commentsModel()
{
    return m_commentsModel;
}

const QString& PhotosManager::selected() const
{
    return m_selectedId;
}

void PhotosManager::setSelected(const QString &selectedId)
{
    if (m_selectedId != selectedId) {
        m_selectedId = selectedId;
        emit selectedChanged();
    }
}

QVariant PhotosManager::album()
{
    QVariant v;
    v.setValue(m_album);
    return v;
}

void PhotosManager::setAlbum(QVariant album)
{
    if (album.canConvert<Album>()) {
        m_album = album.value<Album>();
        m_sm->getPhotos(m_album, false, true, true);
        emit albumChanged();
    }
}

bool PhotosManager::isPhotosUpdate() const
{
    return m_isPhotosUpdate;
}

void PhotosManager::uploadPhoto(QString filePath)
{
    m_sm->uploadPhoto(m_album, filePath, "");
}

void PhotosManager::uploaded(QString accountId, QString albumId, QString file_name = "", QString photoId = "")
{
    Q_UNUSED(photoId)
    Q_UNUSED(albumId)
    if (accountId != m_album.accountId())
        return;

    if (!file_name.isEmpty()) {
        emit photoUploaded(file_name);
    }

    // m_pendingPhotoCount now 0
    if (1 == m_pendingPhotoCount.fetchAndAddAcquire(-1)) {
        emit albumCopyingFinished();
    }
}

void PhotosManager::updateAccounts(QString, AccountList list)
{
    if (!list.isEmpty())
        this->refreshPhotos(false);
}

void PhotosManager::refreshComments()
{
    this->refreshComments(true);
}

void PhotosManager::refreshComments(bool isNeedUpdate)
{
    ListModel* tmp = new ListModel(new CommentItem);
    delete m_commentsModel;
    m_commentsModel = tmp;
    emit commentsModelChanged();

    m_commentsList.clear();
    foreach (Photo photo, m_photoList) {
        m_sm->getComments(photo, isNeedUpdate, true);
    }
}

void PhotosManager::gotComments(QString , PhotoCommentList list)
{
    for (int i = 0; i < list.length(); i++) {
        bool isFound = false;
        for (int j = 0; j < m_commentsList.length(); j++) {
            if (list.at(i).commentId() == m_commentsList.at(j).commentId()) {
                isFound = true;
                break;
            }
        }

        if (!isFound || m_commentsList.isEmpty()) {
            m_commentsList.append(list.at(i));

            Friend profile = m_sm->getProfile(list.at(i), false, false);
            m_commentsModel->appendRow(new CommentItem(list.at(i), profile));
        }
    }
    m_commentsModel->sort(0, Qt::DescendingOrder);
    emit commentsModelChanged();
}

void PhotosManager::gotProfile(Friend profile)
{
    for (int i = 0; i < m_commentsList.length(); i++) {
        if (m_commentsList.at(i).senderId() == profile.ownerId()) {
            m_commentsModel->removeRow(i);
            m_commentsModel->insertRow(i, new CommentItem(m_commentsList.at(i), profile));
        }
    }
}

QVariant PhotosManager::getPhotoById(QString photoId)
{
    for (int i = 0; i < m_photoList.length(); i++) {
        if (m_photoList.at(i).photoId() == photoId) {
            QVariant v;
            v.setValue(m_photoList.at(i));
            return v;
        }
    }
    return QVariant();
}

void PhotosManager::refreshPhotos()
{
    this->refreshPhotos(true);
}

void PhotosManager::refreshPhotos(bool isNeedUpdate)
{
    if (!m_isPhotosUpdate) {
        m_isPhotosUpdate = true;
        emit isPhotosUpdateChanged();
        if (!m_album.albumId().isEmpty()) {
            m_sm->getPhotos(m_album, isNeedUpdate, true, true);
        }
    }
}

bool PhotosManager::saveAlbum(QString albumName, QString path)
{
    m_albumName = albumName;

    if (path.isEmpty())
        m_albumPath = "/home/user/MyDocs/";
    else
        m_albumPath = path;

    m_downloadedPhotosIds.clear();
    if (m_album.albumId().isEmpty() || albumName.isEmpty())
        return false;

    m_photosDownload = m_photoList.length();
    emit photosDownloadChanged();

    m_isDownloadCompleted = false;
    emit downloadCompletedChanged();

    int half = qRound(m_photoList.length() / 2);
    m_sm->downloadPhotos(m_photoList.at(half), half);

    return true;
}

int PhotosManager::photosDownload() const
{
    return m_photosDownload;
}

bool PhotosManager::isDownloadCompleted() const
{
    return m_isDownloadCompleted;
}

void PhotosManager::setDownloadCompleted(bool completed)
{
    if (m_isDownloadCompleted != completed) {
        m_isDownloadCompleted = completed;
        emit downloadCompletedChanged();
    }
}

void PhotosManager::updatePhotos(QString accountId, QString ownerId, QString albumId, PhotoList list, bool isLastUpdate)
{
    if (isLastUpdate)
        m_isPhotosUpdate = false;
    else
        m_isPhotosUpdate = true;
    emit isPhotosUpdateChanged();

    if (m_photosDownload > 0) {
        m_photoList = list;

        QDir dir;
        dir.setPath(dir.cleanPath(m_albumPath + "/" + m_albumName));
        if (!dir.exists())
            dir.mkpath(dir.absolutePath());

        for (int i = 0; i < m_photoList.length(); i++) {
            Photo ph = m_photoList.at(i);
            if (m_downloadedPhotosIds.contains(ph.photoId()))
                continue;

            if (!ph.photo().isEmpty()) {
                m_downloadedPhotosIds.insert(ph.photoId());
                m_photosDownload--;
                emit photosDownloadChanged();

                QImage im(ph.photo());
                QString fileName = QString("IMG_%1.%2").arg(i).arg("png");
                if (!im.save(dir.absolutePath() + "/" + fileName))
                    Q_ASSERT(1 == 0);
            }
        }

        if (m_photosDownload == 0) {
            m_photosDownload = -1;
            m_isDownloadCompleted = true;
            emit downloadCompletedChanged();
        }
    } else {

        if (m_album.albumId() == albumId &&
                m_album.accountId() == accountId &&
                m_album.ownerId() == ownerId) {
            m_photoList = list;
            ListModel* m = new ListModel(new PhotoItem);
            delete m_model;
            m_model = m;
            for (int i = 0; i < list.size(); i++) {
                m_model->appendRow(new PhotoItem(list.at(i)));
            }
            emit modelChanged();
        }
    }
}

void PhotosManager::copyTo(const Friend& profile)
{
    // Create album and wait while service manager creates album
    m_pendingPhotoList = m_album.getPhotoList();
    m_sm->createAlbum(profile.accountId(), m_album.title(),
                      tr("Copy of ") + m_album.title() + " from " + m_album.ownerId(), "EVERYONE");
}

void PhotosManager::copyPendingPhotosToNewAlbum(QString accountId, QString albumId)
{
    m_pendingPhotoCount.fetchAndAddAcquire(m_pendingPhotoList.size());

    foreach (const Photo& photo, m_pendingPhotoList) {
        if (!QFile::exists(photo.photo())) {
            m_sm->downloadPhoto(photo);
        }

        if (QFile::exists(photo.photo())) {
            m_sm->uploadPhoto(accountId, albumId, photo.photo(), photo.description());
        }
    }

    m_pendingPhotoList.clear();
}
