#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QObject>
#include <manager/servicemanager.h>
#include <datatypes/photoitem.h>
#include <datatypes/commentitem.h>
#include <datatypes/friend.h>
#include <datatypes/album.h>
#include <datatypes/photo.h>
#include <datatypes/photocomments.h>
#include "musson_global.h"

class musson_EXPORT ImageViewer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel* model READ model NOTIFY modelChanged)
    Q_PROPERTY(ListModel* commentsModel READ commentsModel NOTIFY commentsModelChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString userIcon READ userIcon NOTIFY userIconChanged)
    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)
public:
    explicit ImageViewer(QObject *parent = 0);
    ~ImageViewer();

    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);
    ListModel* model();
    ListModel* commentsModel();

    const QString& userIcon() const;
    const QString& userName() const;

    int currentIndex() const;

signals:
    void modelChanged();
    void commentsModelChanged();
    void currentIndexChanged();
    void userIconChanged();
    void userNameChanged();

public slots:
    void updatePhotos(QString accountId, QString ownerId, QString albumId, PhotoList list, bool isLastUpdate);
    void gotComments(QString photoId, PhotoCommentList list);
    void gotProfile(Friend profile);

    void loadImage(QVariant image);
    void loadImageAtIndex(int index);

    void sendComment(QString text);

    void refreshComments();
    void refreshComments(bool isNeedUpdate);

private:
    PhotoList m_photoList;
    PhotoCommentList m_commentsList;
    ListModel* m_model;
    ListModel* m_commentsModel;
    ServiceManager *m_sm;
    QString m_userIcon;
    QString m_userName;

    Photo currentPhoto;
    int currentImageIndex;
};

#endif // IMAGEVIEWER_H
