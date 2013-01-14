#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

/**
  \file messageitem.h
  \brief Message delegate for ListModel.

  Delegate for Message datatype from musson-core library.

  \author Musson team

  \date 2011-10-03
  */

#include <QObject>
#include <datatypes/listmodel.h>
#include <datatypes/message.h>
#include <QIcon>
#include "musson_global.h"

/*!
 \brief A class for presenting message type from musson-core.
 */
class musson_EXPORT MessageItem : public ListItem
{
    Q_OBJECT
public:
    /*!
     \brief Empty constructor.

     \param parent object
     */
    MessageItem(QObject *parent = 0) : ListItem(parent) {};

    /*!
     \brief Creates an instance of MessageItem class based on Message.

     \param instance of Message class
     \param author of message
     \param service name
     \param parent object
     */
    explicit MessageItem(Message message, QString author, QString service, QObject* parent = 0);
    ~MessageItem();

    /*!
      \brief Get data from delegate based on defined role.

      \param data role
      \return data based on role
     */
    QVariant data(int role) const;

    /*!
      \brief Get hashtable of role names.

      \return hashtable of role names
     */
    QHash<int, QByteArray> roleNames() const;

    /*!
      \brief Get title of message.

      \return title of message
     */
    const QString& title() const;

    /*!
      \brief Get text of message.

      \return text of message
     */
    const QString& body() const;

    /*!
      \brief Get date of message.

      \return date of message
     */
    const QString& date() const;

    /*!
      \brief Get status of message.

      \return true if message is readed, else false
     */
    const bool& status() const;

    /*!
      \brief Get instance of Message which is base for MessageItem.

      \return variant with instance of Message
     */
    const QVariant& message() const;

    /*!
      \brief Get author of message.

      \return author of message
     */
    const QString& author() const;

    /*!
      \brief Get service name of message.

      \return service name of message
     */
    const QString& service() const;

    /*!
      \brief Get id of account for message.

      \return id of account
     */
    const QString& accountId() const;

    /*!
      \brief Get id of AlbumItem.

      \return id of AlbumItem
     */
    QString id() const;

    /*!
      \brief Get key value.

      It is using for sorting AccountItem.

      \return variant with key value.
     */
    QVariant key() const;

    /*!
      \brief describes role names.
     */
    enum Roles {
        DataRole = Qt::UserRole + 1, /*!< \brief Instance of Message which is base for MessageItem. */
        TitleRole, /*!< \brief Title of message. */
        DateRole, /*!< \brief Date of message. */
        StatusRole, /*!< \brief Status of message. */
        IDRole, /*!< \brief Id of MessageItem. */
        AccountIdRole, /*!< \brief Id of account for album. */
        BodyRole, /*!< \brief Text of message. */
        AuthorRole, /*!< \brief Author of message. */
        ServiceRole /*!< \brief Service name for message. */
    };

private:
    QString m_service;
    QString m_messageId;
    QVariant m_message;
    QString m_author;
    QString m_title;
    QString m_body;
    QString m_date;
    QString m_accountId;
    bool m_status;
};

#endif // MESSAGEITEM_H
