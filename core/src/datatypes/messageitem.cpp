#include "messageitem.h"
#include <QDateTime>

MessageItem::MessageItem(Message message, QString author, QString service, QObject *parent) :
    ListItem(parent)
{
    m_message.setValue(message);
    m_status = message.isReaded();
    m_messageId = message.messageId();
    m_accountId = message.accountId();
    m_title = message.subject();
    m_body = message.text().trimmed();
    m_date = message.time();
    m_author = author;
    m_service = service;
}

MessageItem::~MessageItem()
{

}

QHash<int, QByteArray> MessageItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TitleRole] = "title";
    names[DateRole] = "date";
    names[DataRole] = "message";
    names[StatusRole] = "status";
    names[BodyRole] = "body";
    names[IDRole] = "messageId";
    names[AuthorRole] = "author";
    names[ServiceRole] = "service";
    names[AccountIdRole] = "accountId";
    return names;
}

const QString& MessageItem::title() const
{
    return m_title;
}

const QString& MessageItem::date() const
{
    return m_date;
}

const QVariant& MessageItem::message() const
{
    return m_message;
}

const bool& MessageItem::status() const
{
    return m_status;
}

const QString& MessageItem::body() const
{
    return m_body;
}

const QString& MessageItem::author() const
{
    return m_author;
}

const QString& MessageItem::service() const
{
    return m_service;
}

const QString& MessageItem::accountId() const
{
    return m_accountId;
}

QString MessageItem::id() const
{
    return m_messageId;
}

QVariant MessageItem::key() const
{
    QDateTime dt = QDateTime::fromString(m_date, "dd.MM.yyyy hh:mm:ss");
    QVariant v;
    v.setValue(dt);
    return v;
}

QVariant MessageItem::data(int role) const
{
    switch(role) {
    case TitleRole:
        return title();
        break;
    case DateRole:
        return date();
        break;
    case DataRole:
        return message();
        break;
    case StatusRole:
        return status();
        break;
    case BodyRole:
        return body();
        break;
    case AuthorRole:
        return author();
        break;
    case ServiceRole:
        return service();
        break;
    case IDRole:
        return id();
        break;
    case AccountIdRole:
        return accountId();
        break;
    default:
        return QVariant();
    }
}
