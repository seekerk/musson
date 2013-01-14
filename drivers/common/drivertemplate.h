#ifndef DRIVERTEMPLATE_H
#define DRIVERTEMPLATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QList>
#include <QDebug>
#include "task.h"
#include "webauth.h"
#include "interface.h"
#include "datatypes/driverinfo.h"

class DriverTemplate : public QObject, public DriverInterface
{
    Q_OBJECT
    Q_INTERFACES(DriverInterface)

public:
    explicit DriverTemplate(QObject *parent = 0);

    virtual void init() { manager = new QNetworkAccessManager(this); }

    // return information about driver without path to library
    virtual DriverInfo getInfo();

protected:
    QNetworkAccessManager *manager;

    QList<Task> tasks;

    WebAuth webauth;
signals:
    void error(QString text);

};

#endif // DRIVERTEMPLATE_H
