#ifndef PLACEITEM_H
#define PLACEITEM_H

/**
  \file placeitem.h
  \brief Place delegate for ListModel.

  Delegate for Place datatype from musson-core library.

  \author Musson team

  \date 2011-10-17
  */

#include <QObject>
#include <datatypes/listmodel.h>
#include <datatypes/place.h>
#include "musson_global.h"

/*!
 \brief A class for presenting type from musson-core.
 */
class musson_EXPORT PlaceItem : public ListItem
{
public:
    /*!
     \brief Empty constructor.

     \param parent object
     */
    PlaceItem(QObject *parent = 0) : ListItem(parent) {};

    /*!
     \brief Creates an instance of PlaceItem class based on Place.

     \param instance of Place class
     \param parent object
     */
    explicit PlaceItem(Place place, QObject *parent = 0);
    ~PlaceItem();

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

    const QString& title() const;
    const QString& latitude() const;
    const QString& longitude() const;
    const QString& type() const;
    const QString& country() const;
    const QString& city() const;
    const QString& address() const;
    const QString& icon() const;
    const QString& distance() const;

    QString id() const;
    QVariant key() const;

    /*!
      \brief describes role names.
     */
    enum Roles {
        TitleRole = Qt::UserRole + 1, /*!< \brief Title of place. */
        LatitudeRole,
        LongitudeRole,
        CountryRole,
        CityRole,
        AddressRole,
        TypeRole, /*!< \brief Type of place. */
        IconRole, /*!< \brief Icon of place's type. */
        DistanceRole, /*!< \brief Distance from user to place. */
        IDRole /*!< \brief Id of PlaceItem. */
    };

private:
    QString m_placeId;
    QString m_title;
    QString m_latitude;
    QString m_longitude;
    QString m_country;
    QString m_city;
    QString m_address;
    QString m_type;
    QString m_icon;
    QString m_distance;
};

#endif // PLACEITEM_H
