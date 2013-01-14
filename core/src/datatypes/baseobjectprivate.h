#ifndef BASEOBJECTPRIVATE_H
#define BASEOBJECTPRIVATE_H

#include <QSharedData>
#include <QString>
#include "musson_global.h"

/**
  \brief Base class for private shared data. Contains common properties and methods.
  */
class musson_EXPORT BaseObjectPrivate: public QSharedData
{
public:

    QString serviceId; /*<! ID of service */
    QString accountId; /**<! User account id. */
    QString ownerId; /**<! Object owner id. */
    QString objectId; /**<! Object id. */
    QString icon_url;  /**<! path to thumbnail (icon) */
    bool isPrivate; /**<! True if object has restricted access and can't be download from service. */
    bool isEmpty; /**<! True if nothing changed in this object, otherwise - false. */

    BaseObjectPrivate()
        : QSharedData(), isPrivate(false), isEmpty(true)
    {}

    BaseObjectPrivate(const BaseObjectPrivate& other)
        : QSharedData(other)
    {
        this->serviceId = other.serviceId;
        this->accountId = other.accountId;
        this->ownerId = other.ownerId;
        this->objectId = other.objectId;
        this->icon_url = other.icon_url;
        this->isPrivate = other.isPrivate;
        this->isEmpty = other.isEmpty;
    }

    ~BaseObjectPrivate()
    {}
};

#endif // BASEOBJECTPRIVATE_H
