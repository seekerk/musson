#include "filter/filtermanager.h"

#ifdef DEBUG
#include <QDebug>
#endif

FilterManager::FilterManager(QObject *parent) :
    QObject(parent)
{
}

void FilterManager::registerFilterableType(int type)
{
    if (isTypeRegistered(type)) {
#ifdef DEBUG
        qWarning() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Type " << QMetaType::typeName(type) << " has already been registered";
#endif
        return;
    }
    m_types.append(type);
}

void FilterManager::addFilter(Filter *filter)
{
    int type = filter->filterableType();

    if (!isTypeRegistered(type))
        registerFilterableType(type);

    connect(filter, SIGNAL(stateChanged(int)), SIGNAL(stateChanged(int)));
    m_filters[type].append(filter);
}

bool FilterManager::isTypeRegistered(int type) const
{
    return !m_types.empty() && m_types.contains(type);
}

bool FilterManager::checkValue(const QVariant& value) const
{
    // Get Qt metatype from QVariant
    int type = value.type();
    if (type == QVariant::UserType) {
        type = value.userType();
    }

    if (!isTypeRegistered(type)) {
#ifdef DEBUG
        qWarning() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Type" << type << QMetaType::typeName(type) << "not registered";
#endif
        return true;
    }

    const FilterList& filters = m_filters[type];
    foreach (const Filter* filter, filters) {
        if (filter->isFiltered(value)) {
            return false;
        }
    }

    return true;
}
