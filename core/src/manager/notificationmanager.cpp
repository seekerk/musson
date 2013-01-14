#include "notificationmanager.h"

NotificationManager::NotificationManager(QObject *parent) :
    QObject(parent)
{
}

void NotificationManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;

    connect(m_sm, SIGNAL(errorOccured(QString,QTransport::Action,ErrorMessage,bool)),
            this, SLOT(gotErrorMsg(QString,QTransport::Action,ErrorMessage,bool)));
}

ServiceManager* NotificationManager::sm()
{
    return m_sm;
}

void NotificationManager::gotErrorMsg(QString errorMsg, Driver::Action, ErrorMessage msg, bool )
{
    m_bug = msg;

    switch (m_bug.code().toInt()) {
    case 103:
    case 206:
    case 208:
    case 210:
        showNotification(errorMsg);
        break;
    default:
        emit bugAuthorChanged();
        emit bugTitleChanged();
        emit bugBodyChanged();
        emit bugCommentChanged();
        emit showBugReport();
    }
}

void NotificationManager::createNotification(const QString& errorMsg)
{
    emit showNotification(errorMsg);
}

QString NotificationManager::bugTitle() const
{
    return m_bug.bugTitle();
}

void NotificationManager::setBugTitle(const QString &title)
{
    if (title != m_bug.title()) {
        m_bug.setTitle(title);
        emit bugTitleChanged();
    }
}

const QString NotificationManager::bugAuthor() const
{
    return m_bug.authorMail();
}

void NotificationManager::setBugAuthor(const QString &email)
{
    if (m_bug.authorMail() != email) {
        m_bug.setAuthorMail(email);
        emit bugAuthorChanged();
    }
}

const QString NotificationManager::bugComment() const
{
    return m_bug.comment();
}

void NotificationManager::setBugComment(const QString &comment)
{
    if (m_bug.comment() != comment) {
        m_bug.setComment(comment);
        emit bugCommentChanged();
    }
}

const QString NotificationManager::bugBody() const
{
    return m_bug.bugBody(false);
}

void NotificationManager::setBugBody(const QString &body)
{
    if (m_bug.text() != body) {
        m_bug.setAuthorMessage(body);
        emit bugBodyChanged();
    }
}

void NotificationManager::sendBug()
{
    ErrorMessage *newMsg = new ErrorMessage(m_bug);
    connect(newMsg, SIGNAL(error(QString,ErrorMessage)), SLOT(bugSendFailure(QString,ErrorMessage)));
    connect(newMsg, SIGNAL(success(bool, ErrorMessage)), SLOT(bugSendSuccess(bool, ErrorMessage)));
    newMsg->sendBug();
}

void NotificationManager::bugSendFailure(QString msg, ErrorMessage)
{
    emit showResultBugSending(msg);
}

void NotificationManager::bugSendSuccess(bool isCreate, ErrorMessage msg)
{
    QString message;
    message.append(tr("The bug with id=") + QString::number(msg.bugId()));
    message.append(tr(" was ") + (isCreate ? tr("created") : tr("updated")));

    emit showResultBugSending(message);
}
