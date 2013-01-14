#ifndef PROFILEITEM_H
#define PROFILEITEM_H

/**
  \file profileitem.h
  \brief Profile delegate for ListModel.

  Delegate for Profile datatype from musson-core library.

  \author Musson team

  \date 2011-10-03
  */

#include <QObject>
#include "datatypes/listmodel.h"
#include <datatypes/friend.h>
#include <QIcon>
#include "musson_global.h"

/**
 * @brief A class for presenting friend type from musson-core.
 *
 */
class musson_EXPORT ProfileItem : public ListItem
{
    Q_OBJECT

public:
    /**
    * @brief Empty constructor.
    *
    * @param parent object
    */
    ProfileItem(QObject *parent = 0) : ListItem(parent) {};

    /**
     * @brief Creates an instance of ProfileItem using Friend and service name.
     *
     * @param profile An instance of Friend class.
     * @param service Service name.
     * @param parent Parent object.
     */
    explicit ProfileItem(Friend profile, QString service, QObject *parent = 0);

    /**
     * @brief Creates an instance of ProfileItem based on Friend.
     *
     * @param profile An instance of Friend class.
     * @param parent Parent object.
     */
    explicit ProfileItem(Friend profile, QObject *parent = 0);

    /**
     * @brief Destructor.
     *
     */
    ~ProfileItem();

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
     * @brief Get name of friend.
     *
     * @return name of friend.
     */
    const QString& name() const;

    /**
     * @brief Get avatar of friend.
     *
     * @return avatar of friend.
     */
    const QString& icon() const;

    /**
     * @brief Get online status of friend.
     *
     * @return true if friend is online, else false.
     */
    const bool& offline() const;

    /**
     * @brief Get id of service.
     *
     * @return Id of service.
     */
    const QString& serviceId() const;

    /**
     * @brief Get instance of Friend class which is base for ProfileItem.
     *
     * @return variant with instance of Friend class.
     */
    const QVariant& profile() const;

    /**
     * @brief Get id of ProfileItem.
     *
     * @return Id of ProfileItem.
     */
    QString id() const;

    /**
     * @brief Get keu value of ProfileItem
     * It is using for sorting items.
     *
     * @return key value
     */
    QVariant key() const;

    /**
     * @brief Describes data roles.
     *
     */
    enum Roles {
        DataRole = Qt::UserRole + 1, /**< Friend class which is base for ProfileItem. */
        NameRole, /**< Name of friend. */
        IconRole, /**< Avatar of friend. */
        StatusRole, /**< Online status of friend. */
        ServiceRole, /**< Service name for friend. */
        IDRole /**< ID of ProfileItem. */
    };

private:
    QString m_ownerId;
    QVariant m_profile;
    QString m_name;
    QString m_icon;
    QString m_serviceId;
    bool m_offline;
};

#endif // PROFILEITEM_H
