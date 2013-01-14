#include "servicemanager.h"
#include <QDir>
#include <QPluginLoader>

#include "utils/coresettings.h"
#include "interface.h"

ServiceManager::ServiceManager(QObject *parent) :
    QObject(parent)
{
}

QList<DriverInfo> ServiceManager::getDrivers()
{
    if (!cachedDriverList.isEmpty())
    {
        return this->cachedDriverList;
    }

    QDir dir;
    dir.setPath(mSettings->getDriversDir());            // Search dirs in driversPath
    dir.setNameFilters(QStringList(QString("*."LIB_SUFFIX)));

    foreach (QString strFileName, dir.entryList(QDir::Files)) {
        QPluginLoader loader (dir.absoluteFilePath(strFileName));
        loader.load();
        qDebug() << "load" << dir.absoluteFilePath(strFileName) << "is" << (loader.isLoaded() ? "successful" : "failed");
        if (!loader.isLoaded())
            continue;
        DriverInterface* drv = qobject_cast<DriverInterface*>(loader.instance());

        if (drv == NULL)
            continue;

        DriverInfo drvInfo = drv->getInfo();
        drvInfo.setLibrary(strFileName);

        this->cachedDriverList.append(drvInfo);
    }

    return this->cachedDriverList;
}
