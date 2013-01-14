#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include <QtGui/QSortFilterProxyModel>
#include "musson_global.h"

class ServiceManager;
class FilterManager;
class DriverInfo;
class ServiceFilter;
class QVariant;

class musson_EXPORT FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    class Filter
    {
    public:
        Filter();
        virtual ~Filter();
        virtual bool filterAcceptsValue(const QVariant& value) = 0;
        int filterRole() const;
        void setFilterRole(int role);

    private:
        int filterRole_;
    };

public:
    FilterModel(QObject* parent = 0);
    FilterModel(ServiceManager* smgr, QObject* parent = 0);
    ~FilterModel();

    // Factory functions for creating filters
public:
    Filter* createStringFilter(const QString& str);
    Filter* createDriverFilter(const DriverInfo& driver);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

public slots:
    //! Set filter by driver
    /*!
      \param driver
     */
    void setFilteredDriver(const DriverInfo& driver);

    //! Add case-insensitive filter by string
    /*!
      \param str filter string
     */
    void setFilterString(const QString& str);

    //! Add custom filter
    /*!
      \param filter Custom filter. Function takes ownership on this parameter
     */
    void addFilter(Filter* filter);

    //! Remove custom filter. This function doesn't delete filter
    /*!
      \param filter Custom filter or 0 (then function does nothing)
     */
    void removeFilter(Filter* filter);

    void setFilterManagerEnabled(bool isEnabled);

    //! Role that would be user to determine whether element should be show always
    void setAlwaysShowRole(int role);

private:
    ServiceManager* m_ServiceManager;
    FilterManager* m_filterMgr;
    ServiceFilter* m_serviceFilter;

    Filter* m_stringFilter;
    Filter* m_driverFilter;

    QList<Filter*> m_filters;

    bool m_enableFilterManager;
    int m_alwaysShowRole;
};

#endif // FILTERMODEL_H
