#include "filtermodel.h"

#include <cassert>

#include <QtCore/QDebug>

#include "manager/servicemanager.h"
#include "filter/servicefilter.h"
#include "filter/filtermanager.h"
#include "datatypes/driverinfo.h"

class StringFilter : public FilterModel::Filter
{
public:
    StringFilter(const QString& str);
    void setString(const QString& str);
    bool filterAcceptsValue(const QVariant& value);

private:
    QString string;
};

class DriverFilter : public FilterModel::Filter
{
public:
    DriverFilter(const DriverInfo& info, ServiceManager* mgr);
    void setDriver(const DriverInfo& info);
    bool filterAcceptsValue(const QVariant &value);

private:
    QString getServiceId(const QVariant& data) const;

    DriverInfo driverInfo;
    ServiceManager* sm;
};


FilterModel::FilterModel(QObject* parent) :
    QSortFilterProxyModel(parent)
{
}

FilterModel::FilterModel(ServiceManager* smgr, QObject* parent) :
    QSortFilterProxyModel(parent), m_ServiceManager(smgr),
    m_filterMgr(smgr->filterManager()), m_serviceFilter(smgr->serviceFilter()),
    m_stringFilter(0), m_driverFilter(0), m_enableFilterManager(true),
    m_alwaysShowRole(-1)
{
    setFilterRole(Qt::UserRole);
    setDynamicSortFilter(true);

    connect(m_filterMgr, SIGNAL(stateChanged()), SLOT(invalidate()));
    connect(m_serviceFilter, SIGNAL(serviceSelected(DriverInfo)), SLOT(setFilteredDriver(DriverInfo)));
}

FilterModel::~FilterModel()
{
    qDeleteAll(m_filters.begin(), m_filters.end());
}

bool FilterModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    if (m_alwaysShowRole != -1 && index.data(m_alwaysShowRole).value<bool>())
        return true;

    QVariant data = index.data(filterRole());

    assert(m_filterMgr != 0);

    // Filter by filter manager
    bool passFilterManager = false;
    if (!m_enableFilterManager || m_filterMgr->checkValue(data))
        passFilterManager = true;

    // Filter by custom filters
    bool passCustomFilter = true;
    if (passFilterManager) {
        foreach (Filter* filter, m_filters) {
            QVariant data = index.data(filter->filterRole());
            if (!filter->filterAcceptsValue(data)) {
                passCustomFilter = false;
                break;
            }
        }
    }

    return passCustomFilter && passFilterManager && QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}

void FilterModel::setFilteredDriver(const DriverInfo& driver)
{
    if (m_driverFilter == 0) {
        m_driverFilter = createDriverFilter(driver);
        addFilter(m_driverFilter);
    }

    DriverFilter* filter = static_cast<DriverFilter*>(m_driverFilter);
    filter->setDriver(driver);
    invalidate();
}

void FilterModel::setFilterString(const QString& str)
{
    if (m_stringFilter == 0) {
        m_stringFilter = new StringFilter(str);
        addFilter(m_stringFilter);
    }

    StringFilter* filter = static_cast<StringFilter*>(m_stringFilter);
    filter->setString(str);
    invalidate();
}

void FilterModel::addFilter(FilterModel::Filter* filter)
{
    m_filters.append(filter);
    invalidate();
}

void FilterModel::removeFilter(FilterModel::Filter* filter)
{
    if (filter != 0) {
        m_filters.removeAll(filter);
        invalidate();
    }
}

FilterModel::Filter::Filter()
    : filterRole_(Qt::DisplayRole)
{}

FilterModel::Filter::~Filter()
{}

int FilterModel::Filter::filterRole() const
{
    return filterRole_;
}

void FilterModel::Filter::setFilterRole(int role)
{
    filterRole_ = role;
}

FilterModel::Filter* FilterModel::createStringFilter(const QString& str)
{
    // StringFilter should use display role
    return new StringFilter(str);
}

FilterModel::Filter* FilterModel::createDriverFilter(const DriverInfo& driver)
{
    DriverFilter* filter = new DriverFilter(driver, m_ServiceManager);
    filter->setFilterRole(filterRole()); // Driver filter should use custon filter role (Message, Friend, ...)
    return filter;
}

// String filter
StringFilter::StringFilter(const QString &str)
    : string(str)
{}

bool StringFilter::filterAcceptsValue(const QVariant& value)
{
    if (value.canConvert<QString>())
        return value.toString().contains(string, Qt::CaseInsensitive);
    else
        return true;
}

void StringFilter::setString(const QString& str)
{
    string = str;
}


// Driver filter
DriverFilter::DriverFilter(const DriverInfo& info, ServiceManager *mgr)
    : driverInfo(info), sm(mgr)
{}

bool DriverFilter::filterAcceptsValue(const QVariant& value)
{
    QString serviceId = getServiceId(value);

    if (driverInfo.isEmpty() || serviceId.isNull() || serviceId == driverInfo.serviceName())
        return true;
    else
        return false;

}

void DriverFilter::setDriver(const DriverInfo& info)
{
    driverInfo = info;
}

QString DriverFilter::getServiceId(const QVariant& data) const
{
    if (data.canConvert<Friend>()) {
        return data.value<Friend>().serviceId();
    } else if (data.canConvert<Album>()) {
        QString accountId = data.value<Album>().accountId();
        if (sm->hasAccount(accountId))
            return sm->account(accountId).serviceName();
        return QString();
    } else if (data.canConvert<Message>()) {
        QString accountId = data.value<Message>().accountId();
        if (sm->hasAccount(accountId))
            return sm->account(accountId).serviceName();
        return QString();
    }

    // Doesn't have service id
    return QString();
}

void FilterModel::setFilterManagerEnabled(bool isEnabled)
{
    m_enableFilterManager = isEnabled;
    invalidate();
}

void FilterModel::setAlwaysShowRole(int role)
{
    m_alwaysShowRole = role;
}
