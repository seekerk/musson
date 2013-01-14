#ifndef PLACE_H
#define PLACE_H

#include <QString>
#include <QMetaType>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QSharedData>

#include "utils/qrefreshlist.h"
#include "checkin.h"
class Place;

typedef QRefreshList<Place> PlaceList;

#define FILE_PLACE_CHECKINS_DATA "checkins_%1.xml"
#define NODE_PLACE_CHECKINS_ROOT "checkins"

#define NODE_PLACE_ROOT "place"
#define NODE_PLACE_ID "pid"
#define NODE_PLACE_TITLE "title"
#define NODE_PLACE_LATITUDE "latitude"
#define NODE_PLACE_LONGITUDE "longitude"
#define NODE_PLACE_TYPE "type"
#define NODE_PLACE_COUNTRY "country"
#define NODE_PLACE_CITY "city"
#define NODE_PLACE_ADDRESS "address"
#define NODE_PLACE_CREATED "created"
#define NODE_PLACE_CHECKINS "checkins"
#define NODE_PLACE_UPDATED "updated"

class PlaceData;

class Place
{
    QSharedDataPointer<PlaceData> d;

public:
    Place();
    Place(const Place &src);
    ~Place();

//    QDomElement toQDomElement(QDomDocument& doc) const;
//    static QDomElement toQDomElement(QDomDocument& doc, const Place& src);
//    static Place fromQDomElement(const QDomElement& fr, const QString& accountId);

    /*!
     \brief load place checkins from cache file.

     \return list of Checkins
     */
    CheckinList getCheckinList() const;

    /*!
     \brief load place checkins from cache file.

     \param list - list of checkin that need to store

     \param accountId

     \param placeId
     */
    static void setCheckinList(const CheckinList& list, const QString& accountId, const QString& placeId);

    Place& operator=(const Place&);

    // Getters
    const QString placeId() const;
    const QString title() const;
    const QString latitude() const;
    const QString longitude() const;
    const QString type() const;
    const QString country() const;
    const QString city() const;
    const QString address() const;
    const QString created() const;
    const QString checkins() const;
    const QString updated() const;
    const QString icon() const;
    const QString accountId() const;
    const QString distance() const;

    // Setters
    void setPlaceId(const QString& v);
    void setTitle(const QString& v);
    void setLatitude(const QString& v);
    void setLongitude(const QString& v);
    void setType(const QString& v);
    void setCountry(const QString& v);
    void setCity(const QString& v);
    void setAddress(const QString& v);
    void setCreated(const QString& v);
    void setCheckins(const QString& v);
    void setUpdated(const QString& v);
    void setIcon(const QString& v);
    void setAccountId(const QString& v);
    void setDistance(const QString& v);
};

Q_DECLARE_METATYPE(Place)
Q_DECLARE_METATYPE(PlaceList)

#endif // PLACE_H
