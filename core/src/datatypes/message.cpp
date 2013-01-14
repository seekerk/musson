#include "message.h"

#include <QRegExp>
#include <QHash>

#define SUBJECT_LENGTH 30
#define DEFAULT_SUBJECT "..."

class MessageData : public QSharedData
{
public:
    MessageData();
    MessageData(const MessageData& other);
    ~MessageData();

    QString senderName; /*!< TODO */
    QString threadId; /*!< TODO */

    QList<Recepient> recepientList; /*!< TODO */

    // message body
    QString text; /*!< TODO */

    // message title
    QString title; /*!< TODO */

    // date
    QString time; /*!< TODO */

    bool isReaded; /*!< True if message was readed, otherwise - false. */

    bool isSended; /*!< True if user sends this message, otherwise - false. */

    bool isReceived; /*!< True if user receive this message from other users, otherwise - false. */
};

MessageData::MessageData()
    : isReaded(true), isSended(false), isReceived(false)
{
}

MessageData::MessageData(const MessageData& other)
    : QSharedData(other)
{
    senderName = other.senderName;
    threadId = other.threadId;
    recepientList = other.recepientList;
    text = other.text;
    title = other.title;
    time = other.time;
    isReaded = other.isReaded;
    isSended = other.isSended;
    isReceived = other.isReceived;
}

MessageData::~MessageData()
{
}


Message::Message()
    :BaseObject()
{
    d = new MessageData;
}

Message::Message(const Message &src)
    : BaseObject(src), d(src.d)
{
}

Message::~Message()
{

}

Message Message::fromQDomElement(QDomElement fr, QString accountId)
{
    Message ret = Message();

    ret.setAccountId(accountId);

    if (fr.elementsByTagName(NODE_MESSAGE_ID).count() > 0)
        ret.setMessageId(fr.elementsByTagName(NODE_MESSAGE_ID).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_MESSAGE_THREAD_ID).count() > 0)
        ret.setThreadId(fr.elementsByTagName(NODE_MESSAGE_THREAD_ID).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_MESSAGE_SENDER_ID).count() > 0)
        ret.setSenderId(fr.elementsByTagName(NODE_MESSAGE_SENDER_ID).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_MESSAGE_SENDER_NAME).count() > 0)
        ret.setSenderName(fr.elementsByTagName(NODE_MESSAGE_SENDER_NAME).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_MESSAGE_TEXT).count() > 0)
        ret.setText(fr.elementsByTagName(NODE_MESSAGE_TEXT).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_MESSAGE_TIME).count() > 0)
        ret.setTime(fr.elementsByTagName(NODE_MESSAGE_TIME).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_MESSAGE_TITLE).count() > 0)
        ret.setTitle(fr.elementsByTagName(NODE_MESSAGE_TITLE).at(0).firstChild().toText().data());
    if (fr.elementsByTagName(NODE_MESSAGE_STATUS).count() > 0)
        ret.setReaded(fr.elementsByTagName(NODE_MESSAGE_STATUS).at(0).firstChild().toText().data().startsWith('1'));
    if (fr.elementsByTagName(NODE_MESSAGE_SENDED).count() > 0)
        ret.setSended(fr.elementsByTagName(NODE_MESSAGE_SENDED).at(0).firstChild().toText().data().startsWith('1'));
    if (fr.elementsByTagName(NODE_MESSAGE_RECEIVED).count() > 0)
        ret.setReceived(fr.elementsByTagName(NODE_MESSAGE_RECEIVED).at(0).firstChild().toText().data().startsWith('1'));

    if (fr.elementsByTagName(NODE_ARRAY).count() > 0)
    {
        QDomNode recepientListNode = fr.elementsByTagName(NODE_ARRAY).at(0);
        int recQuantity = recepientListNode.toElement().attribute("quantity").toInt();
        for (int i = 0; i < recQuantity; i++) {
            Recepient rec;
            QDomElement recNode = recepientListNode.childNodes().at(i).toElement();

            if (recNode.elementsByTagName(NODE_MESSAGE_RECEPIENT_ID).count() > 0)
                rec.id = recNode.elementsByTagName(NODE_MESSAGE_RECEPIENT_ID).at(0).firstChild().toText().data();
            if (recNode.elementsByTagName(NODE_MESSAGE_RECEPIENT_NAME).count() > 0)
                rec.name = recNode.elementsByTagName(NODE_MESSAGE_RECEPIENT_NAME).at(0).firstChild().toText().data();

            ret.recepientList().append(rec);
        }
    }
    return ret;
}

QDomElement Message::toQDomElement(QDomDocument doc)
{
    return Message::toQDomElement(doc, *this);
}

QDomElement Message::toQDomElement(QDomDocument doc, Message src)
{
    QDomElement ret = doc.createElement(NODE_MESSAGE_ROOT);

    // ID
    if (!src.messageId().isEmpty())
    {
        QDomElement node = doc.createElement(NODE_MESSAGE_ID);
        QDomText t = doc.createTextNode(src.messageId());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // thread id
    if (!src.threadId().isEmpty())
    {
        QDomElement node = doc.createElement(NODE_MESSAGE_THREAD_ID);
        QDomText t = doc.createTextNode(src.threadId());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // sender id
    if (!src.senderId().isEmpty())
    {
        QDomElement node = doc.createElement(NODE_MESSAGE_SENDER_ID);
        QDomText t = doc.createTextNode(src.senderId());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // sender name
    if (!src.senderName().isEmpty())
    {
        QDomElement node = doc.createElement(NODE_MESSAGE_SENDER_NAME);
        QDomText t = doc.createTextNode(src.senderName());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // recepient list
    if (src.recepientListConst().count() > 0)
    {
        QDomElement node = doc.createElement(NODE_ARRAY);
        node.setAttribute("name", NODE_MESSAGE_RECIPIENT_LIST);
        node.setAttribute("quantity", src.recepientListConst().count());
        for (int i = 0; i < src.recepientListConst().count(); i++) {
            QDomNode node2 = doc.createElement(NODE_STRUCT);
            node2.toElement().setAttribute("name", NODE_MESSAGE_RECEPIENT);

            QDomElement idItem = doc.createElement(NODE_MESSAGE_RECEPIENT_ID);
            QDomElement nameItem = doc.createElement(NODE_MESSAGE_RECEPIENT_NAME);

            QDomText t1 = doc.createTextNode(src.recepientListConst().at(i).id);
            QDomText t2 = doc.createTextNode(src.recepientListConst().at(i).name);

            idItem.appendChild(t1);
            nameItem.appendChild(t2);
            node.appendChild(node2);
            node2.appendChild(idItem);
            node2.appendChild(nameItem);
        }
        ret.appendChild(node);
    }

    // time
    if (!src.time().isEmpty())
    {
        QDomElement node = doc.createElement(NODE_MESSAGE_TIME);
        QDomText t = doc.createTextNode(src.time());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // text
    if (!src.text().isEmpty())
    {
        QDomElement node = doc.createElement(NODE_MESSAGE_TEXT);
        QDomText t = doc.createTextNode(src.text());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // title
    if (!src.title().isEmpty())
    {
        QDomElement node = doc.createElement(NODE_MESSAGE_TITLE);
        QDomText t = doc.createTextNode(src.title());
        ret.appendChild(node);
        node.appendChild(t);
    }

    // status
    if (!src.isReaded())
    {
        QDomElement node = doc.createElement(NODE_MESSAGE_STATUS);
        QDomText t = doc.createTextNode(src.isReaded() ? "1" : "0");
        ret.appendChild(node);
        node.appendChild(t);
    }

    // isSended
    if (src.isSended())
    {
        QDomElement node = doc.createElement(NODE_MESSAGE_SENDED);
        QDomText t = doc.createTextNode(src.isSended() ? "1" : "0");
        ret.appendChild(node);
        node.appendChild(t);
    }

    // isReceived
    if (src.isReceived())
    {
        QDomElement node = doc.createElement(NODE_MESSAGE_RECEIVED);
        QDomText t = doc.createTextNode(src.isReceived() ? "1" : "0");
        ret.appendChild(node);
        node.appendChild(t);
    }

    return ret;
}

const QString Message::subject() const
{
    if (this->title().isEmpty() || this->title().trimmed().compare(DEFAULT_SUBJECT) == 0)
    {
        return subjectFromBody(this->text());
    }
    return this->title();
}

// Choose subject for message as begin of body
QString Message::subjectFromBody(QString messageBody) const
{
    messageBody = messageBody.simplified();
    if (messageBody.isEmpty()) {
        return DEFAULT_SUBJECT;
    }

    if (messageBody.length() <= SUBJECT_LENGTH) {
        return messageBody;
    }

    messageBody.truncate(SUBJECT_LENGTH);
    messageBody.truncate(messageBody.lastIndexOf(QRegExp("\\s")));
    messageBody = messageBody.trimmed();
    messageBody.append(DEFAULT_SUBJECT);
    return messageBody;
}

Message& Message::operator =(Message const &src)
{
    BaseObject::operator =(src);
    d = src.d;
    return *this;
}

// Getters
const QString Message::messageId() const { return objectId(); }
const QString Message::senderId() const { return ownerId(); }
const QString Message::senderName() const { return d->senderName; }
const QString Message::threadId() const { return d->threadId; }
QList<Recepient>& Message::recepientList() { return d->recepientList; }
const QList<Recepient>& Message::recepientListConst() const { return d->recepientList; }
const QString Message::text() const { return d->text; }
const QString Message::title() const { return d->title; }
const QString Message::time() const { return d->time; }
bool Message::isReaded() const { return d->isReaded; }
bool Message::isSended() const { return d->isSended; }
bool Message::isReceived() const { return d->isReceived; }

// Setters
void Message::setMessageId(const QString& v) { setObjectId(v); }
void Message::setSenderId(const QString& v) { setOwnerId(v); }
void Message::setSenderName(const QString& v) { d->senderName = v; }
void Message::setThreadId(const QString& v) { d->threadId = v; }
void Message::setRecepientList(const QList<Recepient>& v) { d->recepientList = v; }
void Message::setText(const QString& v) { d->text = v; }
void Message::setTitle(const QString& v) { d->title = v; }
void Message::setTime(const QString& v) { d->time = v; }
void Message::setReaded(bool v) { d->isReaded = v; }
void Message::setSended(bool v) { d->isSended = v; }
void Message::setReceived(bool v) { d->isReceived = v; }

QString Message::readMessageKey(const QString &accountId, const QString &messageId)
{
    return QString("readMessage_%1_%2").arg(accountId).arg(messageId);
}

QString Message::deleteMessageKey(const QString &accountId, const QString &messageId)
{
    return QString("deleteMessage_%1_%2").arg(accountId).arg(messageId);
}

bool Message::operator==(const Message &m) const
{
    return accountId() == m.accountId()
            && objectId() == m.objectId();
}

uint qHash(const Message& message)
{
    return qHash(qMakePair(message.messageId(), message.accountId()));
}

bool Message::compareByDate(const Message& mess1, const Message& mess2)
{
    // TODO: this string should be somewhere in public header
    static const char* format = "dd.MM.yyyy HH:mm:ss";
    QDateTime time1 = QDateTime::fromString(mess1.time(), format);
    QDateTime time2 = QDateTime::fromString(mess2.time(), format);
    return time1 < time2;
}
