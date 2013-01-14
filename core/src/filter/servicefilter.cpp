#include "filter/servicefilter.h"

#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QIcon>

#include "manager/servicemanager.h"
#include "datatypes/driverinfo.h"

ServiceFilter::ServiceFilter(ServiceManager* mgr, bool split, QObject *parent) :
    QObject(parent), mServiceManager(mgr), mSplit(split), mDisableEventHandling(false)
{
    connect(mServiceManager, SIGNAL(updateAccounts(QString,AccountList)), SLOT(updateAccounts(QString,AccountList)));
}

void ServiceFilter::selectService(DriverInfo driverInfo, int index)
{
    if (!mDisableEventHandling) {
        mDisableEventHandling = true;
        if (index >= 0)
            emit checkItem(index);
        emit serviceSelected(driverInfo);
        mDisableEventHandling = false;
    }
}

bool ServiceFilter::isServiceActive(const DriverInfo &driver) const
{
    if (mCacheServiceNames.empty()) {
        foreach (const Account account, mServiceManager->getAccounts()) {
            // Don't show icon for disabled account
            if (!account.isDisabled())
                mCacheServiceNames.insert(account.serviceName());
        }
    }

    return mCacheServiceNames.contains(driver.serviceName());
}

void ServiceFilter::updateAccounts(QString, AccountList)
{
    mCacheServiceNames.clear();
    fillPanel();
}

void ServiceFilter::registerFilterInterface(QObject* interface)
{
    bool res = true;
    res = res && connect(this, SIGNAL(itemAdded(QIcon,DriverInfo,bool)), interface, SLOT(addItem(QIcon,DriverInfo,bool)));
    res = res && connect(this, SIGNAL(itemAdded(QString,DriverInfo,bool)), interface, SLOT(addItem(QString,DriverInfo,bool)));
    res = res && connect(this, SIGNAL(reset()), interface, SLOT(clear()));
    res = res && connect(interface, SIGNAL(itemSelected(DriverInfo, int)), this, SLOT(selectService(DriverInfo, int)));
    res = res && connect(this, SIGNAL(checkItem(int)), interface, SLOT(checkItem(int)));

    if (res)
        fillPanel();
    else
        qWarning() << "ServiceFilter::registerFilterInterface: wrong interface";
}

void ServiceFilter::fillPanel()
{
    emit reset();
#ifndef Q_WS_MAEMO_5
    emit itemAdded(QIcon(":/res/no_filter.png"), DriverInfo(), true);
#else
    emit itemAdded(QIcon(":/res/no_filter_w.png"), DriverInfo(), true);
#endif

    foreach (DriverInfo driver, mServiceManager->getDrivers()) {
        if (!isServiceActive(driver)) continue;

        if (!driver.icon().isNull())
            emit itemAdded(driver.icon(), driver);
        else
            emit itemAdded(driver.name(), driver);
    }
}
