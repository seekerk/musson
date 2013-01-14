/**
  \file message.h
  \brief Message class description
  \author Musson team
  \date 2011-03-31
  */
#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QString>
#include <QMetaType>
#include <QDomDocument>
#include <QSharedData>

#include "utils/qrefreshlist.h"
#include "datatypes/baseobject.h"
#include "musson_global.h"

#define NODE_MESSAGE_ROOT "message"
#define NODE_MESSAGE_ID "id"
#define NODE_MESSAGE_THREAD_ID "threadId"
#define NODE_MESSAGE_SENDER_ID "senderId"
#define NODE_MESSAGE_SENDER_NAME "senderName"
#define NODE_MESSAGE_RECEPIENT_ID "recepientId"
#define NODE_MESSAGE_RECEPIENT_NAME "recepientName"
#define NODE_MESSAGE_RECIPIENT_LIST "recepientList"
#define NODE_MESSAGE_RECEPIENT "recepient"
#define NODE_MESSAGE_TEXT "text"
#define NODE_MESSAGE_TIME "time"
#define NODE_MESSAGE_TITLE "title"
#define NODE_MESSAGE_STATUS "status"
#define NODE_MESSAGE_SENDED "isSended"
#define NODE_MESSAGE_RECEIVED "isReceived"
#define NODE_ARRAY "array"
#define NODE_STRUCT "struct"

//! Structure for representing of Recepient List
/*!
 \brief

 \class Recepient message.h "src/datatypes/message.h"
*/
struct Recepient {
    QString id; /*!< \brief The ID of recepient */
    QString name; /*!< \brief The name of recepient */
};

class MessageData;

/*!
 \brief A class for message information storage.
 */
/*!
 \brief

 \class Message message.h "src/datatypes/message.h"
*/
class musson_EXPORT Message : public BaseObject
{
    QSharedDataPointer<MessageData> d;
public:
    /*!
     \brief

     \fn Message
    */
    Message();
    /*!
     \brief

     \fn Message
     \param
    */
    Message(const Message&);
    /*!
     \brief

     \fn ~Message
    */
    ~Message();

    /*!
     \brief

     \fn subject
    */
    const QString subject() const;

    /*!
     \brief

     \fn subjectFromBody
     \param messageBody
    */
    QString subjectFromBody(QString messageBody) const;

    // store data to XML format
    /*!
     \brief

     \fn toQDomElement
     \param doc
    */
    QDomElement toQDomElement(QDomDocument doc);

    // store data to XML format
    /*!
     \brief

     \fn toQDomElement
     \param doc
     \param src
    */
    static QDomElement toQDomElement(QDomDocument doc, Message src);

    // load message list from XML struct
    /*!
     \brief

     \fn fromQDomElement
     \param fr
     \param accountId
    */
    static Message fromQDomElement(QDomElement fr, QString accountId);

    /*!
     \brief

     \fn operator =
     \param
    */
    Message& operator=(const Message&);

    /*!
      \brief
      \fn operator==
    */
    bool operator==(const Message& m) const;

    bool operator!=(const Message& m) const { return !(*this == m); }

    /*!
     \brief

     \fn copyData
     \param dst
     \param src
    */
    static void copyData(Message *dst, const Message *src);

    /*!
      \brief Generate key based on account and message IDs.

      \param accountId The ID of account.

      \param messageId The ID of message.

      \return The string key.
      */
    static QString readMessageKey(const QString& accountId, const QString& messageId);

    /*!
      \brief Generate key based on account and message IDs.

      \param accountId The ID of account.

      \param messageId The ID of message.

      \return The string key.
      */
    static QString deleteMessageKey(const QString& accountId, const QString& messageId);

    /*!
      \brief Compare two messages by date
      Can be used as predicate for template functions
      Not using operator< because it is not only way to compare messages
      \param mess1 First message
      \param mess2 Secon message
      \return true if mess1 < mess2, false otherwise
      */
    static bool compareByDate(const Message& mess1, const Message& mess2);

    // Getters
    const QString messageId() const;
    const QString senderId() const;
    const QString senderName() const;
    const QString threadId() const;
    QList<Recepient>& recepientList();
    const QList<Recepient>& recepientListConst() const;
    const QString text() const;
    const QString title() const;
    const QString time() const;
    bool isReaded() const;
    bool isSended() const;
    bool isReceived() const;

    // Setters
    void setMessageId(const QString& v);
    void setSenderId(const QString& v);
    void setSenderName(const QString& v);
    void setThreadId(const QString& v);
    void setRecepientList(const QList<Recepient>& v);
    void setText(const QString& v);
    void setTitle(const QString& v);
    void setTime(const QString& v);
    void setReaded(bool v = true);
    void setSended(bool v = true);
    void setReceived(bool v = true);
};

/*! Returns the hash value of message */
uint qHash(const Message& message);

/*! \typedef MessageList
 Structure for stacking process of message uploading
 \sa QRefreshList
*/
/*!
 \brief

 \typedef MessageList*/
typedef QRefreshList<Message> MessageList;

Q_DECLARE_METATYPE(Message)
Q_DECLARE_METATYPE(MessageList)

#endif // MESSAGE_H
