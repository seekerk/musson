#include "mfiltersmanager.h"
#include "datatypes/driveritem.h"

MFiltersManager::MFiltersManager(QObject *parent) :
    QObject(parent)
{
    m_servicesModel = new MStringListModel;
    m_serviceItems = new ListModel(new DriverItem);
    m_currentTab = -1;
    m_serviceIndex = 0;
    m_serviceFilter = NULL;
}

MFiltersManager::~MFiltersManager()
{
    delete m_servicesModel;
    delete m_serviceItems;
}

void MFiltersManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;
    connect(m_sm, SIGNAL(updateAccounts(QString,AccountList)),
            this, SLOT(updateAccounts(QString,AccountList)));
}

void MFiltersManager::initFilters()
{
    AccountList list = m_sm->getAccounts();
    this->updateAccounts(QString::null, list);
    m_serviceFilter = m_sm->serviceFilter();
}

ServiceManager* MFiltersManager::sm()
{
    return m_sm;
}

int MFiltersManager::currentTab() const
{
    return m_currentTab;
}

void MFiltersManager::setCurrentTab(int index)
{
    if (m_currentTab != index) {
        m_currentTab = index;
        emit currentTabChanged();
    }
}

int MFiltersManager::serviceIndex() const
{
    return m_serviceIndex;
}

void MFiltersManager::selectService(QString serviceName, int index)
{
    m_serviceIndex = index;
    emit serviceIndexChanged();
    if (this->m_serviceFilter == NULL)
        this->initFilters();

    if (index <= 0) {
        m_serviceFilter->selectService(DriverInfo(), index);
        return;
    }

    foreach (DriverInfo info, m_sm->getDrivers()) {
        if (info.name() == serviceName) {
            m_serviceFilter->selectService(info, index);
            break;
        }
    }
}

void MFiltersManager::updateAccounts(QString, AccountList list)
{
    // STEP 1. Collect unique service names
    m_serviceNames.clear();
    m_serviceNames.append(tr("All networks"));
    for (int i = 0; i < list.length(); i++) {
        DriverInfo info = list.at(i).getDriverInfo();
        if (!m_serviceNames.contains(info.name())) {
            m_serviceNames.append(info.name());
        }
    }

    // STEP 2. Remove old services
    bool needClean = true;
    while(needClean)
    {
        needClean = false;
        for (int i = 0; i < m_serviceItems->rowCount(); i++)
        {
            if (!m_serviceNames.contains(m_serviceItems->data(m_serviceItems->index(i),
                                                              DriverItem::NameRole).toString()))
            {
                m_serviceItems->removeRow(i);
                needClean = true;
            }
        }
    }

    // STEP 3. Add new services
    for (int i = 0; i < list.length(); i++) {
        DriverInfo info = list.at(i).getDriverInfo();

        bool isNew = true;
        for (int i = 0; i < m_serviceItems->rowCount(); i++)
        {
            qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "Compare" << m_serviceItems->data(m_serviceItems->index(i), DriverItem::NameRole).toString() << "and" << info.name();
            if (m_serviceItems->data(m_serviceItems->index(i), DriverItem::NameRole).toString().compare(info.name()) == 0)
            {
                isNew = false;
                break;
            }
        }
        if (isNew)
        {
            m_serviceItems->appendRow(new DriverItem(info));
        }
    }

    m_servicesModel->setStringList(m_serviceNames);
    emit servicesModelChanged();
}

MStringListModel* MFiltersManager::servicesModel()
{
    return m_servicesModel;
}

ListModel* MFiltersManager::serviceIconsModel() const
{
    return m_serviceItems;
}

void MFiltersManager::refreshFilters()
{
    AccountList list = m_sm->getAccounts();
    this->updateAccounts(QString::null, list);
}
