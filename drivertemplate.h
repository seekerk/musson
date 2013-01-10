#ifndef DRIVERTEMPLATE_H
#define DRIVERTEMPLATE_H

#include <QObject>
#include <QIcon>
#include <QNetworkAccessManager>
#include <QList>
#include "task.h"
#include "webauth.h"

class DriverTemplate : public QObject
{
    Q_OBJECT
public:
    explicit DriverTemplate(QObject *parent = 0);

    // driver name
    virtual QString getDriverName() = 0;

    // display name
    virtual QString getDriverDisplayName() = 0;

    // driver icon
    virtual QIcon getDriverIcon() = 0;

    virtual void init() { manager = new QNetworkAccessManager(this); }

    // check connection
    virtual void checkConnection() = 0;

protected:
    QNetworkAccessManager *manager;

    QList<Task> tasks;

    WebAuth webauth;
signals:
    void error(QString text);

public slots:

};

#endif // DRIVERTEMPLATE_H
