#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>
#include "musson_global.h"

class musson_EXPORT NetworkManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(QString port READ port WRITE setPort NOTIFY portChanged)
public:
    explicit NetworkManager(QObject *parent = 0);
    ServiceManager* sm();
    void setServiceManager(ServiceManager* sm);

    int type();
    void setType(int type);

    const QString& host() const;
    void setHost(const QString& host);

    const QString& port() const;
    void setPort(const QString& port);

signals:
    void typeChanged();
    void portChanged();
    void hostChanged();

private:
    ServiceManager* m_sm;
    QString m_host;
    QString m_port;
    int m_type;

};

#endif // NETWORKMANAGER_H
