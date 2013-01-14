#include "geomanager.h"

GeoManager::GeoManager(QObject *parent) :
    QObject(parent)
{
    m_placesModel = new ListModel(new PlaceItem, this);
    m_checkinsModel = new ListModel(new CheckinItem, this);
    m_checkinsPlaceModel = new ListModel(new CheckinItem, this);
    m_checkinsFriendModel = new ListModel(new CheckinItem, this);
    m_placesCount = 0;
    m_checkinsCount = 0;
    m_isGeoUpdate = false;
}

void GeoManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;
    connect(m_sm, SIGNAL(updatePlaceList(PlaceList)),
            this, SLOT(gotPlaces(PlaceList)));
    connect(m_sm, SIGNAL(updateCheckinList(QString,CheckinList, int)),
            this, SLOT(gotCheckins(QString,CheckinList, int)));
}

ServiceManager* GeoManager::sm()
{
    return m_sm;
}

ListModel* GeoManager::placesModel()
{
    return m_placesModel;
}

ListModel* GeoManager::checkinsModel()
{
    return m_checkinsModel;
}

ListModel *GeoManager::checkinsPlaceModel()
{
    return m_checkinsPlaceModel;
}

ListModel *GeoManager::checkinsFriendModel()
{
    return m_checkinsFriendModel;
}

int GeoManager::checkinsCount() const
{
    return m_checkinsCount;
}

int GeoManager::placesCount() const
{
    return m_placesCount;
}

bool GeoManager::isGeoUpdate() const
{
    return m_isGeoUpdate;
}

bool GeoManager::createPlace(QString title,QString latitude, QString longitude,
                     QString type, QString address, QString comment, bool isCheckin){

    return m_sm->createPlace(title, latitude, longitude, type, address, comment, isCheckin);
}

bool GeoManager::checkin(QString placeId, QString comment){

    return m_sm->checkIn(placeId,comment);
}

void GeoManager::searchPlaces(QString text, QString latitude, QString longitude, QString radius)
{
    m_sm->searchPlaces(text, latitude, longitude, radius);
}

void GeoManager::gotPlaces(PlaceList list)
{
    ListModel* tmp = new ListModel(new PlaceItem);
    delete m_placesModel;
    m_placesModel = tmp;
    m_placesCount = list.length();
    for (int i = 0; i < list.length(); i++) {
        m_placesModel->appendRow(new PlaceItem(list.at(i)));
    }
    emit placesModelChanged();
}

void GeoManager::refreshCheckins(QString latitude, QString longitude, QString place, QString uid, const int type)
{
    m_sm->getCheckins(place, latitude, longitude, uid, type, true);
}

void GeoManager::gotCheckins(QString placeId, CheckinList list, const int type)
{

    ListModel* tmp = new ListModel(new CheckinItem);

    m_checkinsCount = list.length();
    m_placeId = placeId;

    if(type == 0){
        delete m_checkinsFriendModel;
        m_checkinsFriendModel = tmp;

        for (int i = 0; i < list.length(); i++) {
            m_checkinsFriendModel->appendRow(new CheckinItem(list.at(i)));
        }
        emit checkinsFriendModelChanged();
    } else if(type == 1){

        delete m_checkinsModel;
        m_checkinsModel = tmp;

        for (int i = 0; i < list.length(); i++) {
            m_checkinsModel->appendRow(new CheckinItem(list.at(i)));
        }
        emit checkinsModelChanged();
    } else if(type == 2){
        delete m_checkinsPlaceModel;
        m_checkinsPlaceModel = tmp;

        for (int i = 0; i < list.length(); i++) {
            m_checkinsPlaceModel->appendRow(new CheckinItem(list.at(i)));
        }
        emit checkinsPlaceModelChanged();
    }

}



