#ifndef GEOMANAGER_H
#define GEOMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>

#include <datatypes/listmodel.h>
#include <datatypes/placeitem.h>
#include <datatypes/checkinitem.h>
#include <datatypes/checkin.h>
#include <datatypes/place.h>
#include "musson_global.h"

class musson_EXPORT GeoManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel* placesModel READ placesModel NOTIFY placesModelChanged)
    Q_PROPERTY(ListModel* checkinsModel READ checkinsModel NOTIFY checkinsModelChanged)
    Q_PROPERTY(ListModel* checkinsPlaceModel READ checkinsPlaceModel NOTIFY checkinsPlaceModelChanged)
    Q_PROPERTY(ListModel* checkinsFriendModel READ checkinsFriendModel NOTIFY checkinsFriendModelChanged)
    Q_PROPERTY(bool isGeoUpdate READ isGeoUpdate NOTIFY isGeoUpdateChanged)
    Q_PROPERTY(int placesCount READ placesCount)
    Q_PROPERTY(int checkinsCount READ checkinsCount)

public:
    explicit GeoManager(QObject *parent = 0);

    enum CheckinType
    {
        FriendsType = 0,
        NearbyType,
        PlaceType
    };
    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);

    ListModel* placesModel();
    ListModel* checkinsModel();
    ListModel* checkinsPlaceModel();
    ListModel* checkinsFriendModel();

    int placesCount() const;
    int checkinsCount() const;
    bool isGeoUpdate() const;

signals:
    void placesModelChanged();
    void checkinsModelChanged();
    void checkinsPlaceModelChanged();
    void checkinsFriendModelChanged();
    void isPlacesUpdateChanged();
    void isCheckinsUpdateChanged();
    void isGeoUpdateChanged();

public slots:
    void searchPlaces(QString text, QString latitude, QString longitude, QString radius);
    void refreshCheckins(QString latitude, QString longitude, QString place, QString uid, const int type);

    bool createPlace(QString title,QString latitude, QString longitude,
                     QString type, QString address, QString comment, bool isCheckin);
    bool checkin(QString placeId, QString comment);

    void gotPlaces(PlaceList list);
    void gotCheckins(QString placeId, CheckinList list, const int type);

private:
    int m_checkinsCount;
    int m_placesCount;
    bool m_isGeoUpdate;

    QString m_placeId;

    ListModel* m_placesModel;
    ListModel* m_checkinsModel;
    ListModel* m_checkinsPlaceModel;
    ListModel* m_checkinsFriendModel;

    ServiceManager *m_sm;
};
#endif // GEOMANAGER_H
