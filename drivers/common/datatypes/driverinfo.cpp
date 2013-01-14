#include "driverinfo.h"

DriverInfo::DriverInfo(QObject *parent) :
    QObject(parent)
{
}

DriverInfo::DriverInfo(const DriverInfo &src) :
    QObject(src.parent())
{
    this->mServiceName = src.mServiceName;
    this->mName = src.mName;
    this->mIcon = src.mIcon;
    this->mLibrary = src.mLibrary;
}

bool DriverInfo::isEmpty() const
{
    return this->mServiceName.isEmpty() && this->mName.isEmpty() && this->mIcon.isNull() && this->mLibrary.isEmpty();
}

DriverInfo& DriverInfo::operator =(const DriverInfo& src)
{
    this->mName = src.mName;
    this->mServiceName = src.mServiceName;
    this->mIcon = src.mIcon;
    this->mLibrary = src.mLibrary;

    return *this;
}

QString DriverInfo::name() const {return mName;}
void DriverInfo::setName(const QString &v) {mName = v;}

QString DriverInfo::serviceName() const {return mServiceName;}
void DriverInfo::setServiceName(const QString &v) {mServiceName = v;}

QString DriverInfo::icon() const {return mIcon;}
void DriverInfo::setIcon(const QString &v) {mIcon = v;}

QString DriverInfo::library() const {return mLibrary;}
void DriverInfo::setLibrary(const QString &v) {mLibrary = v;}
