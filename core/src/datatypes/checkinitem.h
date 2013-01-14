#ifndef CHECKINITEM_H
#define CHECKINITEM_H

/**
  \file checkinitem.h
  \brief Checkin delegate for ListModel.

  Delegate for Checkin datatype from musson-core library.

  \author Musson team

  \date 2011-10-17
  */

#include <QObject>
#include <datatypes/listmodel.h>
#include <datatypes/checkin.h>
#include "musson_global.h"

/*!
 \brief A class for presenting type from musson-core.
 */
class musson_EXPORT CheckinItem : public ListItem
{
public:
    /*!
     \brief Empty constructor.

     \param parent object
     */
    CheckinItem(QObject *parent = 0) : ListItem(parent) {};

    /*!
     \brief Creates an instance of CheckinItem class based on Checkin.

     \param instance of Checkin class
     \param parent object
     */
    explicit CheckinItem(Checkin checkin, QObject *parent = 0);
    ~CheckinItem();

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

    const QString& senderIcon() const;
    const QString& senderName() const;
    const QString& text() const;
    const QString& time() const;
    const QString& latitude() const;
    const QString& longitude() const;
    const QString& placeId() const;
    const QString& placeTitle() const;
    const QString& placeCountry() const;
    const QString& placeCity() const;
    const QString& placeAddress() const;
    const QString& placeType() const;
    const QString& placeIcon() const;

    /*!
      \brief Get id of CheckinItem.

      \return id of CheckinItem
     */
    QString id() const;

    /*!
      \brief Get key value.

      It is using for sorting CheckinItem.

      \return variant with key value.
     */
    QVariant key() const;

    /*!
      \brief describes role names.
     */
    enum Roles {
        TextRole = Qt::UserRole + 1, /*!< \brief Text of checkin message. */
        SenderIconRole, /*!< \brief Avatar of checkin sender. */
        SenderNameRole, /*!< \brief Name of checkin sender. */
        LatitudeRole,
        LongitudeRole,
        TimeRole, /*!< \brief Time of checkining. */
        DataRole,
        PlaceIdRole, /*!< \brief Id of place. */
        PlaceTitleRole,
        PlaceCountryRole,
        PlaceCityRole,
        PlaceAddressRole,
        PlaceTypeRole,
        PlaceIconRole,
        IDRole /*!< \brief Id of CheckinItem. */
    };
private:
    QString m_text;
    QString m_time;
    QString m_checkinId;
    QString m_senderIcon;
    QString m_senderName;
    QString m_latitude;
    QString m_longitude;
    QString m_placeId;
    QString m_placeTitle;
    QString m_placeCountry;
    QString m_placeCity;
    QString m_placeAddress;
    QString m_placeType;
    QString m_placeIcon;
};

#endif // CHECKINITEM_H
