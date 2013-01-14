#ifndef SERVICEFILTER_H
#define SERVICEFILTER_H

#include <QObject>
#include <QVariant>
#include <QSet>
#include <QList>

#include "manager/servicemanager.h"
#include "musson_global.h"

class ServiceManager;
class DriverInfo;
class QWidget;
class QToolBar;
class QActionGroup;
class FilterPanel;

class musson_EXPORT ServiceFilter : public QObject
{
    Q_OBJECT
    ServiceManager* mServiceManager; //!< Service manager

    mutable QSet<QString> mCacheServiceNames; //!< Names of active services
    bool mSplit; //!< Each panel has own state of filter
    bool mDisableEventHandling; //!< Disable receiving signals from panels
                                //!< Possibly unnessessary

    bool isServiceActive(const DriverInfo& driver) const;

public:
    explicit ServiceFilter(ServiceManager *mgr, bool split, QObject *parent = 0);

    void registerFilterInterface(QObject* interface);
    void fillPanel();

signals:
    void serviceSelected(DriverInfo driverInfo);
    void itemAdded(const QIcon& icon, const DriverInfo& driver, bool checked = 0);
    void itemAdded(const QString& string, const DriverInfo& driver, bool checked = 0);
    void checkItem(int index);
    void reset();

public slots:
    void updateAccounts(QString str, AccountList accountList);
    void selectService(DriverInfo driverInfo, int index = -1);
};

#endif // SERVICEFILTER_H
