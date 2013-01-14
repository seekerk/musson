#ifndef ALBUMITEM_H
#define ALBUMITEM_H

/**
  \file albumitem.h
  \brief Album delegate for ListModel.

  Delegate for Album datatype from musson-core library.

  \author Musson team

  \date 2011-10-03
  */

#include <QObject>
#include <datatypes/listmodel.h>
#include <datatypes/friend.h>
#include <datatypes/album.h>
#include "musson_global.h"

#include "utils/coresettings.h"

/*!
 \brief A class for presenting album type from musson-core.
 */
class musson_EXPORT AlbumItem : public ListItem
{
    Q_OBJECT
public:
    /*!
     \brief Empty constructor.

     \param parent object
     */
    AlbumItem(QObject *parent = 0) : ListItem(parent) {};

    /*!
     \brief Creates an instance of AlbumItem class based on Album.

     \param instance of Album class
     \param parent object
     */
    explicit AlbumItem(Album album, QObject *parent = 0);
    ~AlbumItem();

    /*!
      \brief Get data from delegate based on defined role.

      \param data role
      \return data based on role
     */
    QVariant data(int role) const;

    /*!
      \brief Get hashtable of role names.

      \return hashtable of role names
     */
    QHash<int, QByteArray> roleNames() const;

    /*!
      \brief Get id of album owner.

      \return id of album owner
     */
    const QString& ownerId() const;

    /*!
      \brief Get id of account for album.

      \return id of account
     */
    const QString& accountId() const;

    /*!
      \brief Get album icon.

      \return album icon
     */
    const QString& icon() const;

    /*!
      \brief Get title of album.

      \return title of album
     */
    const QString& title() const;

    /*!
      \brief Get description of album.

      \return description of album
     */
    const QString& description() const;

    /*!
      \brief Get time of creating album.

      \return time of creating album
     */
    const QString& time_create() const;

    /*!
      \brief Get time of last update for album.

      \return time of last update
     */
    const QString& time_update() const;

    /*!
      \brief Get amount of photos for album.

      \return amount of photos
     */
    int size() const;

    /*!
      \brief Get instance of album, which is base for AlbumItem.

      \return instance of album
     */
    const QVariant& album() const;

    bool canShowIcon() const;

    /*!
      \brief Get id of AlbumItem.

      \return id of AlbumItem
     */
    QString id() const;

    /*!
      \brief Get key value.

      It is using for sorting AccountItem.

      \return variant with key value.
     */
    QVariant key() const;

    /*!
      \brief describes role names.
     */
    enum Roles {
        TitleRole = Qt::UserRole + 1, /*!< \brief Title of album. */
        IconRole, /*!< \brief Icon of album. */
        CanShowIconRole,
        DescriptionRole, /*!< \brief Description of album. */
        TimeCreateRole, /*!< \brief Time of creating album. */
        TimeUpdateRole, /*!< \brief Time of last update for album. */
        DataRole, /*!< \brief Instance of Album which is base for AlbumItem. */
        SizeRole, /*!< \brief Amount of photos. */
        IDRole /*!< \brief ID of AlbumItem. */
    };

private:
    QString m_ownerId;
    QString m_accountId;
    QString m_albumId;
    QString m_title;
    QString m_description;
    QString m_timeCreate;
    QString m_timeUpdate;
    bool m_canShowIcon;
    QVariant m_album;
    QString m_icon;
    int m_size;
};

#endif // ALBUMITEM_H
