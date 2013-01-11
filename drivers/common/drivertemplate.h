#ifndef DRIVERTEMPLATE_H
#define DRIVERTEMPLATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QList>
#include <QDebug>
#include "task.h"
#include "webauth.h"
#include "interface.h"

class DriverTemplate : public QObject, public DriverInterface
{
    Q_OBJECT
    Q_INTERFACES(DriverInterface)

public:
    explicit DriverTemplate(QObject *parent = 0);

    virtual void init() { manager = new QNetworkAccessManager(this); }

    // driver name
    virtual QString getDriverName() = 0;

    // display name
    virtual QString getDriverDisplayName() = 0;

    // driver icon
    virtual QIcon getDriverIcon() = 0;

    // check connection
    virtual void checkConnection() = 0;


protected:
    QNetworkAccessManager *manager;

    QList<Task> tasks;

    WebAuth webauth;
signals:
    void error(QString text);

};

#endif // DRIVERTEMPLATE_H
