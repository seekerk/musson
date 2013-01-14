#ifndef EVENTITEM_H
#define EVENTITEM_H

/**
  \file eventitem.h
  \brief Event delegate for ListModel.

  Delegate for Event datatype from mysocials-core library.

  \author MySocials team

  \date 2011-11-18
  */

#include <QObject>
#include <datatypes/listmodel.h>
#include <datatypes/event.h>
#include "musson_global.h"

/*!
 \brief A class for presenting type from mysocials-core.
 */
class musson_EXPORT EventItem : public ListItem
{
public:
    /*!
     \brief Empty constructor.

     \param parent object
     */
    EventItem(QObject *parent = 0) : ListItem(parent) {}

    /*!
     \brief Creates an instance of EventItem class based on Event.

     \param instance of Event class
     \param parent object
     */
    explicit EventItem(Event event, QObject *parent = 0);
    ~EventItem();

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

    //const QString& senderIcon() const;
    //const QString& senderName() const;
    const QString& text() const;
    const QString& title() const;
    const QString& description() const;
    const QString& startTime() const;
    const QString& endTime() const;
    const QString& updateTime() const;
    const QString& location() const;
    const QString& creator() const;
    const QString& privacy() const;
    const QString& icon() const;
    const QString& iconUrl() const;
    const QString& rsvpStatus() const;
    void setTitle(QVariant a);
    void setDescription(QVariant a);
    void setStartTime(QVariant a);
    void setEndTime(QVariant a);
    void setLocation(QVariant a);
    void setIcon(QVariant a);
    void setCreator(QVariant a);


    /*!
      \brief Get id of EventItem.

      \return id of EventItem
    */
    QString id() const;

    /*!
      \brief Get key value.

      It is using for sorting EventItem.

      \return variant with key value.
     */
    QVariant key() const;

    /*!
      \brief describes role names.
    */
    enum Roles {
        //SenderIconRole, /*!< \brief Avatar of event owner. */
        //SenderNameRole, /*!< \brief Name of checkin owner. */
        TitleRole,
        DescriptionRole,
        StartTimeRole,
        EndTimeRole,
        UpdateTimeRole,
        CreatorRole,
        LocationRole,
        PrivacyRole,
        IconUrlRole,
        IconRole,
        RsvpStatusRole,
        IDRole          /*!< \brief Id of EventItem. */
    };
private:
    QString m_time;
    QString m_eventId;
    //QString m_senderIcon;
    //QString m_senderName;
    QString m_title;
    QString m_description;
    QString m_startTime;
    QString m_endTime;
    QString m_updateTime;
    QString m_location;
    QString m_creator;
    QString m_privacy;
    QString m_icon;
    QString m_iconUrl;
    QString m_rsvpStatus;
};

#endif // EVENTITEM_H
