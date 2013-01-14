#include "commentitem.h"

CommentItem::CommentItem(PhotoComment comment, Friend author, QObject *parent)
    : ListItem(parent)
{
    m_commentId = comment.commentId();
    m_text = comment.text();
    m_time = comment.time();
    m_senderIcon = author.icon();
    m_senderName = author.name();
    m_author = author;
    m_photoId = comment.photoId();
}

CommentItem::~CommentItem()
{
}

QHash<int, QByteArray> CommentItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[SenderNameRole] = "senderName";
    names[SenderIconRole] = "senderIcon";
    names[AuthorRole] = "author";
    names[DataRole] = "data";
    names[TextRole] = "body";
    names[TimeRole] = "time";
    names[IDRole] = "accountId";
    names[PhotoIdRole] = "photoId";
    return names;
}

const QString& CommentItem::senderName() const
{
    return m_senderName;
}

const QString& CommentItem::senderIcon() const
{
    return m_senderIcon;
}

const QString& CommentItem::text() const
{
    return m_text;
}

const QString& CommentItem::time() const
{
    return m_time;
}

const QString& CommentItem::photoId() const
{
    return m_photoId;
}

QVariant CommentItem::author() const
{
    QVariant v;
    v.setValue(m_author);
    return v;
}

QString CommentItem::id() const
{
    return m_commentId;
}

QVariant CommentItem::key() const
{
    QDateTime dt = QDateTime::fromString(m_time, "dd.MM.yyyy hh:mm:ss");
    QVariant v;
    v.setValue(dt);
    return v;
}

QVariant CommentItem::data(int role) const
{
    switch(role) {
    case SenderNameRole:
        return senderName();
        break;
    case SenderIconRole:
        return senderIcon();
        break;
    case TextRole:
        return text();
        break;
    case TimeRole:
        return time();
        break;
    case AuthorRole:
        return author();
        break;
    case PhotoIdRole:
        return photoId();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}
