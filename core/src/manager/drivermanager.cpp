#include "drivermanager.h"

DriverManager::DriverManager(QObject *parent) :
    QObject(parent)
{
    m_model = new ListModel(new DriverItem);
}

DriverManager::~DriverManager()
{
    delete m_model;
}

void DriverManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;

    QList<DriverInfo> drivers = m_sm->getDrivers();
    for (int i = 0; i < drivers.length(); i++) {
        m_model->appendRow(new DriverItem(drivers.at(i)));
    }
    emit modelChanged();
}

ServiceManager *DriverManager::sm()
{
    return m_sm;
}

ListModel* DriverManager::model()
{
    return m_model;
}

const QString& DriverManager::selected() const
{
    return m_selectedId;
}

void DriverManager::setSelected(const QString &selectedId)
{
    if (m_selectedId != selectedId) {
        m_selectedId = selectedId;
        emit selectedChanged();
    }
}

DriverInfo* DriverManager::getSelectedDriver()
{
    ListItem* item = m_model->find(m_selectedId);
    if (!item)
        return new DriverInfo();
    QVariant v = item->data(DriverItem::DataRole);

    if (v.canConvert<DriverInfo>()) {
        DriverInfo info = v.value<DriverInfo>();
        return new DriverInfo(info);
    }
    return new DriverInfo();
}
