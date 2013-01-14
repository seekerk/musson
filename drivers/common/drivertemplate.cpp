#include "drivertemplate.h"

DriverTemplate::DriverTemplate(QObject *parent) :
    QObject(parent)
{
    manager = NULL;
}

DriverInfo DriverTemplate::getInfo()
{
    DriverInfo ret;
    ret.setName(this->getDriverDisplayName());
    ret.setServiceName(this->getDriverName());
    ret.setIcon(this->getDriverIcon());

    return ret;
}
