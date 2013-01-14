#ifndef DRIVERMANAGER_H
#define DRIVERMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>
#include "datatypes/listmodel.h"
#include "datatypes/driveritem.h"
#include <datatypes/driverinfo.h>
#include "musson_global.h"

class musson_EXPORT DriverManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel *model READ model NOTIFY modelChanged)
    Q_PROPERTY(QString selected READ selected WRITE setSelected NOTIFY selectedChanged)
public:
    explicit DriverManager(QObject *parent = 0);
    ~DriverManager();

    ServiceManager* sm();
    void setServiceManager(ServiceManager* sm);
    ListModel* model();

    const QString& selected() const;
    void setSelected(const QString& selectedId);

signals:
    void modelChanged();
    void selectedChanged();

public slots:
    DriverInfo* getSelectedDriver();

private:
    ListModel* m_model;
    ServiceManager* m_sm;
    QString m_selectedId;
};



#endif // DRIVERMANAGER_H
