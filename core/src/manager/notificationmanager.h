#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>
#include <datatypes/errormessage.h>
#include "musson_global.h"
#include "interface.h"

class musson_EXPORT NotificationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(QString bugTitle READ bugTitle WRITE setBugTitle NOTIFY bugTitleChanged)
    Q_PROPERTY(QString bugBody READ bugBody WRITE setBugBody NOTIFY bugBodyChanged)
    Q_PROPERTY(QString bugComment READ bugComment WRITE setBugComment NOTIFY bugCommentChanged)
    Q_PROPERTY(QString bugAuthor READ bugAuthor WRITE setBugAuthor NOTIFY bugAuthorChanged)

public:
    explicit NotificationManager(QObject *parent = 0);
    ServiceManager* sm();
    void setServiceManager(ServiceManager* sm);

    QString bugTitle() const;
    void setBugTitle(const QString& title);
    const QString bugBody() const;
    void setBugBody(const QString& body);
    const QString bugComment() const;
    void setBugComment(const QString& comment);
    const QString bugAuthor() const;
    void setBugAuthor(const QString& email);

signals:
    void showNotification(const QString &text);
    void showBugReport();
    void showResultBugSending(const QString &text);
    void bugTitleChanged();
    void bugBodyChanged();
    void bugCommentChanged();
    void bugAuthorChanged();

private slots:
    void bugSendFailure(QString msg, ErrorMessage err);
    void bugSendSuccess(bool isCreate, ErrorMessage msg);

public slots:
    void createNotification(const QString& errorMsg);
    void gotErrorMsg(QString errorMsg, Driver::Action acc, ErrorMessage msg, bool isMajor);
    void sendBug();

private:
    ServiceManager* m_sm;
    ErrorMessage m_bug;

};

#endif // NOTIFICATIONMANAGER_H
