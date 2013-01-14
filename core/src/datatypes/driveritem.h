#ifndef DRIVERITEM_H
#define DRIVERITEM_H

/**
  \file driveritem.h
  \brief Driver delegate for ListModel.

  Delegate for DriverInfo datatype from musson-core library.

  \author Musson team

  \date 2011-10-03
  */

#include "datatypes/listmodel.h"
#include <datatypes/driverinfo.h>
#include <QIcon>
#include "musson_global.h"

/*!
 \brief A class for presenting driver type from musson-core.
 */
class musson_EXPORT DriverItem : public ListItem
{
public:
    /*!
     \brief Empty constructor.

     \param parent object
     */
    DriverItem(QObject *parent = 0) : ListItem(parent) {};

    /*!
     \brief Creates an instance of DriverItem class based on DriverInfo.

     \param instance of DriverInfo class
     \param parent object
     */
    explicit DriverItem(const DriverInfo& info, QObject *parent = 0);

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
      \brief Get name of service.

      \return name of service
     */
    const QString& name() const;

    /*!
      \brief Get service icon.

      \return service icon
     */
    const QIcon& sIcon() const;

    /*!
      \brief Get instance of DriverInfo which is base for DriverItem.

      \return variant with instance of DriverInfo
     */
    const QVariant driver() const;

    /*!
      \brief Get key value.

      It is using for sorting DriverItem.

      \return variant with key value.
     */
    inline QVariant key() const { return m_name; };

    /*!
      \brief Get id of DriverItem.

      \return id of DriverItem
     */
    inline QString id() const { return m_name; };

    /*!
      \brief describes role names.
     */
    enum Roles {
        NameRole = Qt::UserRole + 1, /*!< \brief Name of service. */
        IconRole, /*!< \brief Icon of service. */
        DataRole, /*!< \brief Instance of DriverInfo. */
        IDRole /*!< \brief Id of DriverItem. */
    };

private:
    QVariant m_driver;
    QIcon m_icon;
    QString m_name;
};

#endif // DRIVERITEM_H
