#ifndef MFILTERSMANAGER_H
#define MFILTERSMANAGER_H

#include <QObject>
#include <datatypes/mstringlistmodel.h>
#include <manager/servicemanager.h>
#include <filter/servicefilter.h>
#include <datatypes/account.h>
#include <datatypes/driverinfo.h>
#include "musson_global.h"
#include "datatypes/listmodel.h"

class musson_EXPORT MFiltersManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(MStringListModel* servicesModel READ servicesModel NOTIFY servicesModelChanged)
    Q_PROPERTY(ListModel* serviceIconsModel READ serviceIconsModel NOTIFY servicesModelChanged)
    Q_PROPERTY(int currentTab READ currentTab WRITE setCurrentTab NOTIFY currentTabChanged)
    Q_PROPERTY(int serviceIndex READ serviceIndex NOTIFY serviceIndexChanged)
public:
    explicit MFiltersManager(QObject *parent = 0);
    ~MFiltersManager();

    ServiceManager *sm();
    void setServiceManager(ServiceManager* sm);

    int currentTab() const;
    void setCurrentTab(int index);

    int serviceIndex() const;

    MStringListModel* servicesModel();
    ListModel* serviceIconsModel() const;

signals:
    void servicesModelChanged();
    void currentTabChanged();
    void serviceIndexChanged();

public slots:
    void updateAccounts(QString accountName, AccountList name);
    void selectService(QString serviceName, int index);

    void initFilters();
    void refreshFilters();

private:
    ServiceManager* m_sm;
    MStringListModel *m_servicesModel;
    QStringList m_serviceNames;
    ServiceFilter *m_serviceFilter;

    /**
      \brief list of unique account services.
      */
    ListModel *m_serviceItems;

    int m_currentTab;
    int m_serviceIndex;
};

#endif // MFILTERSMANAGER_H
