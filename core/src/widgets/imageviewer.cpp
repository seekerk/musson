#include "imageviewer.h"

ImageViewer::ImageViewer(QObject *parent) :
    QObject(parent)
{
    m_model = new ListModel(new PhotoItem, this);
    m_commentsModel = new ListModel(new CommentItem, this);
}

ImageViewer::~ImageViewer()
{
    delete m_model;
    delete m_commentsModel;
}

void ImageViewer::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;

    connect(m_sm, SIGNAL(updatePhotoList(QString,QString,QString,PhotoList,bool)),
            this, SLOT(updatePhotos(QString,QString,QString,PhotoList,bool)));
    connect(m_sm, SIGNAL(updatePhotoCommentList(QString,PhotoCommentList)),
            this, SLOT(gotComments(QString,PhotoCommentList)));
    connect(m_sm, SIGNAL(updateProfile(Friend)),
            this, SLOT(gotProfile(Friend)));
}

ServiceManager* ImageViewer::sm()
{
    return m_sm;
}

ListModel* ImageViewer::model()
{
    return m_model;
}

ListModel* ImageViewer::commentsModel()
{
    return m_commentsModel;
}

const QString& ImageViewer::userIcon() const
{
    return m_userIcon;
}

const QString& ImageViewer::userName() const
{
    return m_userName;
}

void ImageViewer::updatePhotos(QString accountId, QString ownerId, QString albumId, PhotoList list, bool isLastUpdate)
{
    qDebug() <<__FILE__ <<":" << __LINE__ << ":" << __FUNCTION__ << "() got " << list.count() << " new images for album " << albumId << " isLastUpdate=" << isLastUpdate;
    if (this->currentPhoto.accountId() == accountId &&
            this->currentPhoto.ownerId() == ownerId &&
            this->currentPhoto.albumId() == albumId)
        m_photoList = list;
    else
        return;

    this->currentImageIndex = -1;

    for (int i = 0; i < m_photoList.length(); i++) {
        if (m_photoList.at(i).photoId() == this->currentPhoto.photoId()) {
            this->currentImageIndex = i;

            if (!m_photoList.at(i).isPrivate() && !m_photoList.at(i).photo().isEmpty()) {
                if (!QFile::exists(m_photoList.at(i).photo()) && isLastUpdate)
                    m_sm->downloadPhotos(m_photoList.at(i), 1);
            }

            emit currentIndexChanged();
            break;
        }
    }

    if (this->currentImageIndex < 0 && !this->currentPhoto.photo().isEmpty()) {
        QFile fl(this->currentPhoto.photo());
        if (!fl.exists() && !this->currentPhoto.isPrivate()) {
            m_sm->downloadPhoto(this->currentPhoto);
        }
        return;
    }

    ListModel* m = new ListModel(new PhotoItem);
    delete m_model;
    m_model = m;

    for (int i = 0; i < m_photoList.size(); i++) {
        m_model->appendRow(new PhotoItem(m_photoList.at(i)));
    }
    emit modelChanged();
}

void ImageViewer::loadImage(QVariant image)
{
    ListModel* m = new ListModel(new PhotoItem);
    delete m_model;
    m_model = m;
    emit modelChanged();
    m_photoList.clear();

    if (image.canConvert<Photo>()) {
        this->currentPhoto = image.value<Photo>();
        currentImageIndex = -1;

        if (!this->currentPhoto.albumId().isEmpty())
            m_photoList = m_sm->getPhotosForAlbum(this->currentPhoto);

        AccountList list = m_sm->getAccounts();
        for (int i = 0; i < list.length(); i++) {
            if (this->currentPhoto.accountId() == list.at(i).accountId()) {
                Friend profile = list.at(i).getProfile(false);
                m_userIcon = profile.icon();
                m_userName = profile.name();
                emit userIconChanged();
                emit userNameChanged();
                break;
            }
        }

        for (int i = 0; i < m_photoList.length(); i++) {
            if (m_photoList.at(i).photoId() == this->currentPhoto.photoId()) {
                this->currentImageIndex = i;
                emit currentIndexChanged();
                break;
            }
        }

        if (this->currentImageIndex >= 0) {
            m_sm->downloadPhotos(this->currentPhoto, 1);
        } else {
            m_sm->downloadPhoto(this->currentPhoto);
            m_photoList.append(this->currentPhoto);
            this->updatePhotos(currentPhoto.accountId(), currentPhoto.ownerId(), currentPhoto.albumId(), m_photoList, true);
        }
    }
}

void ImageViewer::loadImageAtIndex(int index)
{
    if (index >= m_photoList.length() || index < 0)
        return;
    this->currentPhoto = this->m_photoList.at(index);

    // load next or previous images for comfortable image viewing
    if (this->currentImageIndex > index) {
        if (index >= 1) {
            Photo prevPhoto = m_photoList.at(index - 1);
            if (!QFile::exists(prevPhoto.photo())) {
                qDebug() <<__FILE__ <<":" << __LINE__ << ":" << __FUNCTION__ << " download next photo";
                m_sm->downloadPhotos(this->currentPhoto, 1);
            }
        }
    } else {
        if (index < m_photoList.length() - 1) {
            Photo nextPhoto = m_photoList.at(index + 1);
            if (!QFile::exists(nextPhoto.photo())) {
                qDebug() <<__FILE__ <<":" << __LINE__ << ":" << __FUNCTION__ << " download previous photo";
                m_sm->downloadPhotos(this->currentPhoto, 1);
            }
        }
    }
    this->currentImageIndex = index;
    emit currentIndexChanged();

    if (!currentPhoto.photo().isEmpty() || currentPhoto.isPrivate())
        return;
    m_sm->downloadPhotos(this->currentPhoto, 0);
}

int ImageViewer::currentIndex() const
{
    return this->currentImageIndex;
}

void ImageViewer::gotComments(QString photoId, PhotoCommentList list)
{
    ListModel* tmp = new ListModel(new CommentItem);
    delete m_commentsModel;
    m_commentsModel = tmp;
    if (this->currentPhoto.photoId() == photoId) {
        m_commentsList = list;

        for (int i = 0; i < m_commentsList.size(); i++) {
            Friend profile = m_sm->getProfile(m_commentsList.at(i), false, false);
            m_commentsModel->appendRow(new CommentItem(m_commentsList.at(i), profile));
        }
    }
    m_commentsModel->sort(0, Qt::DescendingOrder);
    emit commentsModelChanged();
}

void ImageViewer::gotProfile(Friend profile)
{
    for (int i = 0; i < m_commentsList.length(); i++) {
        if (m_commentsList.at(i).senderId() == profile.ownerId()) {
            m_commentsModel->removeRow(i);
            m_commentsModel->insertRow(i, new CommentItem(m_commentsList.at(i), profile));
        }
    }
}

void ImageViewer::sendComment(QString text)
{
    m_sm->sendComment(this->currentPhoto, text);
}

void ImageViewer::refreshComments()
{
    this->refreshComments(true);
}

void ImageViewer::refreshComments(bool isNeedUpdate)
{
    if (this->currentPhoto.photo().isEmpty())
        return;
    m_sm->getComments(this->currentPhoto, isNeedUpdate, true);
}
