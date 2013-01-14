#include "baseobject.h"

#include "baseobjectprivate.h"

#include <QFileInfo>

BaseObject::BaseObject()
{
    d = new BaseObjectPrivate;
}

BaseObject::BaseObject(const BaseObject &other)
    : d(other.d)
{}

BaseObject::~BaseObject()
{}

bool BaseObject::isEmpty() const {return d->isEmpty;}

BaseObject& BaseObject::operator=(const BaseObject& other)
{
    d = other.d;
    return *this;
}

const QString BaseObject::generateFileName(const QString &path) const
{
    return generateFileName(path, this->iconUrl());
}

const QString BaseObject::generateFileName(const QString &path, const QString &obj) const
{
    if (obj.isEmpty())
        return QString::null;

    QString icon = NULL;
    QFileInfo fi(obj);
    QString ext = fi.suffix();
    icon = path + QString(obj.toUtf8().toBase64()) + (ext.isEmpty() ? QString() : QString(".") + ext);
    return icon;
}

//Getters
const QString BaseObject::serviceId() const {return d->serviceId;}
const QString BaseObject::accountId() const {return d->accountId;}
const QString BaseObject::ownerId() const {return d->ownerId;}
const QString BaseObject::objectId() const {return d->objectId;}
const QString BaseObject::iconUrl() const {return d->icon_url;}
bool BaseObject::isPrivate() const {return d->isPrivate;}

//Setters
void BaseObject::setServiceId(const QString &v) {d->serviceId = v; d->isEmpty = false;}
void BaseObject::setAccountId(const QString &v) {d->accountId = v; d->isEmpty = false;}
void BaseObject::setOwnerId(const QString &v) {d->ownerId = v; d->isEmpty = false;}
void BaseObject::setObjectId(const QString &v) {d->objectId = v; d->isEmpty = false;}
void BaseObject::setIconUrl(const QString &v) {d->icon_url = v; d->isEmpty = false;}
void BaseObject::setPivate(bool v) {d->isPrivate = v; d->isEmpty = false;}
