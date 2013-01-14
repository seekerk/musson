#ifndef FEEDITEM_H
#define FEEDITEM_H

/**
  \file feeditem.h
  \brief QEventFeed delegate for ListModel.

  Delegate for QEventFeed datatype from musson-core library.

  \author Musson team

  \date 2011-10-10
  */

#include "datatypes/listmodel.h"
#include "datatypes/qattachment.h"
#include "datatypes/friend.h"
#include "datatypes/qeventfeed.h"
#include "musson_global.h"

class musson_EXPORT FeedItem : public ListItem
{
public:
    FeedItem(QObject *parent = 0) : ListItem(parent) {};
    explicit FeedItem(QEventFeed feed, QEventFeed::FeedType type, Friend owner, QObject *parent = 0);

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
      \brief Get key value.

      It is using for sorting DriverItem.

      \return variant with key value.
     */
    QVariant key() const;

    /*!
      \brief Get id of DriverItem.

      \return id of DriverItem
     */
    inline QString id() const { return m_ownerId; };

    /*!
      \brief describes role names.
     */
    enum Roles {
        NameRole = Qt::UserRole + 1, /*!< \brief Owner name. */
        IconRole, /*!< \brief Avatar of owner. */
        TextRole, /*!< \brief Text of feed. */
        CreatedRole, /*!< \brief Time of creating feed. */
        PhotoRole, /*!< \brief Photo for photofeed. */
        PhotoDataRole, /*!< \brief Instance of Photo class for photo feed. */
        LinkRole, /*!< \brief Link to object. */
        TypeRole, /*!< \brief Type of feed. */
        DataRole,
        IDRole /*!< \brief Id of DriverItem. */
    };

    const QVariant feed() const;
    const QVariant photoData() const;

    const QString& ownerName() const;
    const QString& ownerIcon() const;
    const QString& text() const;
    const QString& created() const;
    const QString& photo() const;
    const QString& link() const;
    int feedType() const;

private:
    QVariant m_feed;
    Photo m_photoData;
    QString m_ownerName;
    QString m_ownerIcon;
    QString m_created;
    QString m_text;
    QString m_ownerId;
    QString m_photo;
    QString m_link;
    int m_feedType;
};

#endif // FEEDITEM_H
