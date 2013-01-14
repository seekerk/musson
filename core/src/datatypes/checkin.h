#ifndef CHECKIN_H
#define CHECKIN_H

#include <QString>
#include <QMetaType>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QSharedData>

#include "utils/qrefreshlist.h"

class Checkin;

typedef QRefreshList<Checkin> CheckinList;

#define NODE_CHECKIN_ROOT "checkin"
#define NODE_CHECKIN_ID "id"
#define NODE_CHECKIN_OWNER_ID "uid"
#define NODE_CHECKIN_OWNER_NAME "uname"
#define NODE_CHECKIN_DATE "date"
#define NODE_CHECKIN_LATITUDE "latitude"
#define NODE_CHECKIN_LONGITUDE "longitude"
#define NODE_CHECKIN_TEXT "text"
#define NODE_CHECKIN_PLACE_ID "place_id"
#define NODE_CHECKIN_PLACE_TYPE "palce_type"
#define NODE_CHECKIN_PLACE_TITLE "palce_title"
#define NODE_CHECKIN_PLACE_COUNTRY "place_country"
#define NODE_CHECKIN_PLACE_CITY "place_city"
#define NODE_CHECKIN_PLACE_ADDRESS "palce_address"
#define NODE_CHECKIN_PLACE_ICON "place_icon"

class CheckinData;

class Checkin
{
    QSharedDataPointer<CheckinData> d;
public:
    Checkin();
    Checkin(const Checkin &);
    Checkin &operator=(const Checkin &);
    ~Checkin();

    /*!
      \brief store checkinlist data to XML format

      \param doc

      \return document in XML Format
      */
    QDomElement toQDomElement(QDomDocument& doc) const;

    /*!
     \brief store checkinlist data to XML format

     \param doc

     \param src - checkin that need to cashing

     \return document in XML Format
     */
    static QDomElement toQDomElement(QDomDocument& doc, const Checkin& src);

    /*!
     \brief load checkinlist from XML struct

     \param fr - XML struct with checkin data

     \param accountId

     \param placeId

     \return created Checkin
    */
    static Checkin fromQDomElement(const QDomElement& fr, const QString& accountId, const QString& placeId);


    // Getters
    const QString checkinId() const;
    const QString ownerId() const;
    const QString ownerName() const;
    const QString ownerPhoto() const;
    const QString date() const;
    const QString latitude() const;
    const QString longitude() const;
    const QString text() const;
    const QString placeId() const;
    const QString placeCountry() const;
    const QString placeCity() const;
    const QString placeAddress() const;
    const QString placeType() const;
    const QString placeTitle() const;
    const QString placeIcon() const;
    const QString accountId() const;

    // Setters
    void setCheckinId(const QString& v);
    void setOwnerId(const QString& v);
    void setOwnerName(const QString& v);
    void setOwnerPhoto(const QString& v);
    void setDate(const QString& v);
    void setLatitude(const QString& v);
    void setLongitude(const QString& v);
    void setText(const QString& v);
    void setPlaceId(const QString& v);
    void setPlaceCountry(const QString& v);
    void setPlaceCity(const QString& v);
    void setPlaceAddress(const QString& v);
    void setPlaceType(const QString& v);
    void setPlaceTitle(const QString& v);
    void setPlaceIcon(const QString& v);
    void setAccountId(const QString& v);


};
Q_DECLARE_METATYPE(Checkin)
Q_DECLARE_METATYPE(CheckinList)

#endif // CHECKIN_H
