/**
  \file event.h
  \brief Event class description
  \author Musson team
  \date 2012-03-09
  */

#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QMetaType>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QSharedData>

#include "musson_global.h"
#include "utils/qrefreshlist.h"
#include "datatypes/baseobject.h"
class Event;

typedef QRefreshList<Event> EventList;

#define NODE_EVENT_ROOT "event"
#define NODE_EVENT_ID "id"
#define NODE_EVENT_OWNER_ID "ownerId"
#define NODE_EVENT_TITLE "title"
#define NODE_EVENT_DESCRIPTION "description"
#define NODE_EVENT_START_TIME "startTime"
#define NODE_EVENT_END_TIME "endTime"
#define NODE_EVENT_UPDATE_TIME "updateTime"
#define NODE_EVENT_LOCATION "location"
#define NODE_EVENT_CREATOR "creator"
#define NODE_EVENT_PRIVACY "privacy"
#define NODE_EVENT_ICON "icon"
#define NODE_EVENT_ICON_URL "iconUrl"
#define NODE_EVENT_RSVP_STATUS "rsvpStatus"

class EventData;

class musson_EXPORT Event : public BaseObject
{
    QSharedDataPointer<EventData> d;
public:
    Event();
    Event(const Event &);
    Event &operator=(const Event &);
    ~Event();

    /*!
      \brief store Eventlist data to XML format

      \param doc

      \return document in XML Format
      */
    QDomElement toQDomElement(QDomDocument& doc) const;

    /*!
     \brief store eventlist data to XML format

     \param doc

     \param src - event that need to cashing

     \return document in XML Format
     */
    static QDomElement toQDomElement(QDomDocument& doc, const Event& src);

    /*!
     \brief load eventlist from XML struct

     \param fr - XML struct with event data

     \param accountId

     \param eventId

     \return created Event
    */
    static Event fromQDomElement(const QDomElement& fr, const QString& srvcId, const QString& accountId, const QString& friendId);

    /*!
      Merges two lists, creates metaprofiles for duplicate items.

      \param src1 The first list

      \param src2 The second list

      \return New event list
     */
    static EventList mergeLists(EventList src1, EventList src2);

    /*!
      \brief Generate key based on account and event IDs.

      \param accountId The ID of account.

      \param eventId The ID of event.

      \return The string key.
      */
    static QString deleteEventKey(const QString &accountId, const QString &eventId);

    // Getters
    const QString title() const;
    const QString description() const;
    const QString startTime() const;
    const QString endTime() const;
    const QString updateTime() const;
    const QString location() const;
    const QString creator() const;
    const QString privacy() const;
    const QString icon() const;
    const QString rsvpStatus() const;

    // Setters
    void setTitle(const QString& v);
    void setDescription(const QString& v);
    void setStartTime(const QString& v);
    void setEndTime(const QString& v);
    void setUpdateTime(const QString& v);
    void setLocation(const QString& v);
    void setCreator(const QString& v);
    void setPrivacy(const QString& v);
    void setRsvpStatus(const QString& v);
};
Q_DECLARE_METATYPE(Event)
Q_DECLARE_METATYPE(EventList)

#endif // EVENT_H
