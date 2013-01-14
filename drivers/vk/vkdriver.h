#ifndef VKDRIVER_H
#define VKDRIVER_H

#include "drivertemplate.h"

#define appId "1899304"

class VKdriver : public DriverTemplate
{
    Q_OBJECT

private:
    // access token used to sign requests
    QString accessToken;

    QString userId;

public:
    VKdriver();

    QString getDriverName() {return "vkontakte";}

    QString getDriverDisplayName() { return "ВКонтакте"; }

    QString getDriverIcon() { return ""; }

    void checkConnection();

    void init();

private:
    void authorize();

    void processRequests(TaskType::TaskType type);

private slots:
    void gotResponse();
    void authorized(QUrl url);
};

#endif // VKDRIVER_H
