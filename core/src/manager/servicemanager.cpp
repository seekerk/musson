#include "servicemanager.h"
#include <QDir>
#include <QPluginLoader>

#include "utils/coresettings.h"
#include "interface.h"
#include "filter/servicefilter.h"
#include "filter/filterfriendonline.h"
#include "filter/filterfriendgender.h"
#include "filter/filtermessagereaded.h"
#include "filter/filtermessagetype.h"

ServiceManager::ServiceManager(QObject *parent) :
    QObject(parent)
{

    createFilter();
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


ServiceManager::~ServiceManager()
{
    this->accounts.clear();
}


AccountList ServiceManager::getAccounts()
{
    return this->accounts;
}

Account ServiceManager::account(QString accountId)
{
    foreach(Account acc, accounts)
    {
        if (acc.accountId() == accountId)
            return acc;
    }

    return Account();
}


void ServiceManager::createFilter()
{
    serviceFilter_ = new ServiceFilter(this, false, this);
    serviceFilter_->setObjectName("Main service filter");

    filterManager_ = new FilterManager(this);
    filterManager_->setObjectName("Main filter manager");

    filterManager_->registerFilterableType<Friend>();
    filterManager_->registerFilterableType<Album>();
    filterManager_->registerFilterableType<Message>();
    filterManager_->registerFilterableType<Photo>();
    filterManager_->registerFilterableType<Audio>();
    filterManager_->registerFilterableType<Video>();

    filterManager_->addFilter(new FilterFriendOnline(filterManager_));
    filterManager_->addFilter(new FilterFriendGender(filterManager_));
    filterManager_->addFilter(new FilterMessageReaded(filterManager_));
    filterManager_->addFilter(new FilterMessageType(this, filterManager_));
}

FilterManager *ServiceManager::filterManager() const
{
    return filterManager_;
}


ServiceFilter *ServiceManager::serviceFilter() const
{
    return serviceFilter_;
}


bool ServiceManager::hasAccount(QString accountId)
{
    foreach (Account acc, accounts)
    {
        if (acc.accountId() == accountId)
            return true;
    }

    return false;
}

Friend ServiceManager::getMyProfile(bool isNeedUpdate)
{
    Friend fd;
    if (this->accounts.length() > 0)
    {
        int start = 0;

        for (; start <this->accounts.length(); start++)
            if(!this->accounts.at(start).isDisabled())
                break;

        if (start >= this->accounts.length())
            return fd;

        fd = this->accounts.at(start).getProfile(isNeedUpdate);

        if (start + 1 < this->accounts.length())
        {
            for (int i = start + 1; i < this->accounts.length(); i++)
            {
                if (this->accounts.at(i).isDisabled()) continue;
                fd.addProfile(this->accounts.at(i).getProfile(isNeedUpdate), false);
            }
        }
    }
    return fd;
}
