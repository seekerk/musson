//! \file filtermanager.h
//! \author Kirill Ivashov
//! \brief Main class for filtering by datatypes

#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include <algorithm>

#include <QObject>
#include <QList>
#include <QHash>
#include <QMetaType>
#include <QVariant>

#include "filter/filter.h"
#include "musson_global.h"

class QActionGroup;
class Filter;

//! Filter Manager
/*!
 * Filter manager object responsible for filtering different datatype such as
 * Friend, Album, Message, etc.
 */
class musson_EXPORT FilterManager : public QObject
{
    Q_OBJECT

    typedef QList<Filter*> FilterList;
    typedef QHash<int, FilterList> FilterMap;
    typedef QList<int> FilterTypeList;

    FilterMap m_filters;
    FilterTypeList m_types;

public:
    //! A constructor
    explicit FilterManager(QObject *parent = 0);

    //! Register new filterable type
    //! \param T Type to register. Should be declared with Q_DECLARE_METATYPE() macro
    template <typename T> void registerFilterableType();

    //! Register new filterable type by type identifier
    //! \param type Type to register
    void registerFilterableType(int type);

    //! Check is type registered
    //! \param type Type to check
    //! \return true is type registered, false otherwise
    bool isTypeRegistered(int type) const;

    //! Add new filter
    //! \param Descendant of Filter class with overrided methods
    void addFilter(Filter* filter);

    //! Check, whether this value filtered by at least one of filters
    //! \param Value to check
    //! \return true if value pass filters, false if value filtered
    template <typename T> bool checkValue(const T& t) const;

    //! Check wheter this value filtered by at least one of filters
    //! \param Value to check as variant
    //! \return true if value pass filters, false if value filtered
    bool checkValue(const QVariant& value) const;

    //! Remove values that doesn't pass at least one filter
    //! This function modifies argument!
    //! \param QList<T> with values
    template <typename T> void filterList(QList<T>& container) const;

    //! Create list of QActionGroups that specifies user interface for given type
    //! \param T Type
    //! \return QList
    template <typename T> QList<QActionGroup*> createActions();

    //! Connect given slot to type update
    //! This is experimental feature
    template <typename T> void connectType(const QObject* object, const char* method, Qt::ConnectionType connType = Qt::AutoConnection);

signals:
    void stateChanged(int type = -1);
};

template <typename T>
void FilterManager::registerFilterableType()
{
    registerFilterableType(qMetaTypeId<T>());
}

template <typename T>
bool FilterManager::checkValue(const T& t) const
{
    return checkValue(QVariant::fromValue(t));
}

template <typename T> void FilterManager::filterList(QList<T>& container) const
{
    if (!container.empty()) {
        QMutableListIterator<T> iter(container);
        while (iter.hasNext()) {
            if (!checkValue(iter.next()))
                iter.remove();
        }
    }
}

template <typename T>
QList<QActionGroup*> FilterManager::createActions()
{
    int type = qMetaTypeId<T>();
    Q_ASSERT(isTypeRegistered(type));

    FilterList& filters = m_filters[type];
    QList<QActionGroup*> ret;

    foreach (Filter* filter, filters) {
        ret.append(filter->createActions());
    }

    return ret;
}

template <typename T>
void FilterManager::connectType(const QObject* object, const char* method, Qt::ConnectionType connType)
{
    int type = qMetaTypeId<T>();
    Q_ASSERT(isTypeRegistered(type));

    FilterList& filters = m_filters[type];
    foreach (Filter* filter, filters) {
        connect(filter, SIGNAL(stateChanged()), object, method, connType);
    }
}

#endif // FILTERMANAGER_H
