#ifndef COMMENTITEM_H
#define COMMENTITEM_H

/**
  \file commentitem.h
  \brief Comment delegate for ListModel.

  Delegate for Comment datatype from musson-core library.

  \author Musson team

  \date 2011-10-03
  */

#include <QObject>
#include <datatypes/listmodel.h>
#include <datatypes/friend.h>
#include <datatypes/photocomments.h>
#include "musson_global.h"

/*!
 \brief A class for presenting type from musson-core.
 */
class musson_EXPORT CommentItem : public ListItem
{
public:
    /*!
     \brief Empty constructor.

     \param parent object
     */
    CommentItem(QObject *parent = 0) : ListItem(parent) {};

    /*!
     \brief Creates an instance of CommentItem class based on Album.

     \param instance of Comment class
     \param parent object
     */
    explicit CommentItem(PhotoComment comment, Friend author, QObject *parent = 0);
    ~CommentItem();

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
      \brief Get avatar of comment sender.

      \return avatar of comment sender
     */
    const QString& senderIcon() const;

    /*!
      \brief Get name of sender.

      \return name of sender
     */
    const QString& senderName() const;

    QVariant author() const;

    /*!
      \brief Get text of comment message.

      \return text of comment message
     */
    const QString& text() const;

    /*!
      \brief Get date when comment is sent.

      \return date when comment is sent
     */
    const QString& time() const;

    /*!
      \brief Get id of photo.

      \return id of photo
     */
    const QString& photoId() const;

    /*!
      \brief Get id of CommentItem.

      \return id of CommentItem
     */
    QString id() const;

    /*!
      \brief Get key value.

      It is using for sorting CommentItem.

      \return variant with key value.
     */
    QVariant key() const;

    /*!
      \brief describes role names.
     */
    enum Roles {
        TextRole = Qt::UserRole + 1, /*!< \brief Text of comment message. */
        SenderIconRole, /*!< \brief Avatar of comment sender. */
        SenderNameRole, /*!< \brief Name of comment sender. */
        AuthorRole,
        TimeRole, /*!< \brief Time of sending comment. */
        DataRole,
        PhotoIdRole, /*!< \brief Id of photo. */
        IDRole /*!< \brief Id of CommentItem. */
    };

private:
    QString m_text;
    QString m_time;
    QString m_commentId;
    QString m_senderIcon;
    QString m_senderName;
    QString m_photoId;
    Friend m_author;
};

#endif // COMMENTITEM_H
