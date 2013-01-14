#ifndef ALBUMSMANAGER_H
#define ALBUMSMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>
#include <datatypes/listmodel.h>
#include <datatypes/albumitem.h>
#include <datatypes/friend.h>
#include <datatypes/album.h>
#include "musson_global.h"

class musson_EXPORT AlbumsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QString selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(QString profileIcon READ profileIcon NOTIFY profileIconChanged)
    Q_PROPERTY(QVariant owner READ owner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(bool isAlbumsUpdate READ isAlbumsUpdate NOTIFY isAlbumsUpdateChanged)
    Q_PROPERTY(bool showMyAlbums READ showMyAlbums WRITE setShowMyAlbums NOTIFY showMyAlbumsChanged)
public:
    explicit AlbumsManager(QObject *parent = 0);
    ~AlbumsManager();

    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);
    ListModel* model();

    const QString& selected() const;
    void setSelected(const QString& selectedId);

    const QString& profileIcon() const;

    QVariant owner();
    void setOwner(QVariant owner);

    bool isAlbumsUpdate() const;

    bool showMyAlbums() const;
    void setShowMyAlbums(bool show);

signals:
    void modelChanged();
    void selectedChanged();
    void ownerChanged();
    void isAlbumsUpdateChanged();
    void showMyAlbumsChanged();
    void profileIconChanged();

public slots:
    void updateAccounts(QString accountName, AccountList list);
    void updateAlbums(Friend owner, AlbumList list, bool isLastUpdate);

    void refreshAlbums();
    void refreshAlbums(bool isNeedUpdate);

    void createAlbum(QVariant profile, QString name, QString description, QString privacy);
    void initAlbums();

private:
    AlbumList m_albumList;
    QString m_selectedId;
    ListModel* m_model;
    ServiceManager *m_sm;
    Friend m_owner;
    QString m_profileIcon;

    bool m_isAlbumsUpdate;
    bool m_showMyAlbums;
};

#endif // ALBUMSMANAGER_H
