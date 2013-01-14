#ifndef PHOTOITEM_H
#define PHOTOITEM_H

/**
  \file photoitem.h
  \brief Photo delegate for ListModel.

  Delegate for Photo datatype from musson-core library.

  \author Musson team

  \date 2011-10-03
  */

#include <QObject>
#include <datatypes/listmodel.h>
#include <datatypes/photo.h>
#include <datatypes/album.h>
#include <QIcon>
#include "musson_global.h"

/**
 * @brief A class for presenting photo type from musson-core.
 *
 */
class musson_EXPORT PhotoItem : public ListItem
{
public:
    /**
    * @brief Empty constructor.
    *
    * @param parent Parent object.
    */
    PhotoItem(QObject *parent = 0) : ListItem(parent) {};

    /**
     * @brief Creates an instance of PhotoItem based on Photo.
     *
     * @param photo An instance of Photo class.
     * @param parent Parent object.
     */
    explicit PhotoItem(Photo photo, QObject *parent = 0);

    /**
     * @brief Desctructor.
     *
     */
    ~PhotoItem();

    /**
     * @brief Get data based on defined role.
     *
     * @param role Data role.
     */
    QVariant data(int role) const;

    /**
     * @brief Get hashtable of role names.
     *
     * @return Hashtable <role index, role name>.
     */
    QHash<int, QByteArray> roleNames() const;

    /**
     * @brief Get description of photo.
     *
     * @return Description of photo.
     */
    const QString& description() const;

    /**
     * @brief Get path to photo.
     *
     * @return Path to photo.
     */
    const QString& photo() const;

    /**
     * @brief Get time of creating photo.
     *
     * @return Time of creating photo.
     */
    const QString& time_create() const;

    /**
     * @brief Get preview image for photo.
     *
     * @return preview image
     */
    const QString& icon() const;

    /**
     * @brief Get instance of Photo class which is base for PhotoItem.
     *
     * @return variant with instance of Photo class.
     */
    const QVariant& photoData() const;

    bool canShowImage() const;
    bool canShowIcon() const;

    /**
     * @brief Get id of PhotoItem.
     *
     * @return Id of PhotoItem.
     */
    QString id() const;

    /**
     * @brief Get keu value of PhotoItem
     * It is using for sorting items.
     *
     * @return key value
     */
    QVariant key() const;

    /**
     * @brief Describes roles.
     *
     */
    enum Roles {
        PhotoRole = Qt::UserRole + 1, /**< Path to photo. */
        CanShowImageRole,
        CanShowIconRole,
        IconRole, /**< Preview image for photo. */
        DescriptionRole, /**< Description of photo. */
        TimeCreateRole, /**< Time of creating photo. */
        DataRole, /**< Photo class which is base for PhotoItem. */
        IDRole /**< Id of PhotoItem. */
    };

private:
    QString m_photoId;
    QString m_accountId;
    QString m_albumId;
    QString m_description;
    QString m_timeCreate;
    QString m_photo;
    bool m_canShowImage;
    bool m_canShowIcon;
    QVariant m_photoData;
    QString m_icon;
};

#endif // PHOTOITEM_H
