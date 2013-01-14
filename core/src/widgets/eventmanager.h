#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QObject>
#include <QDebug>

#include <manager/servicemanager.h>

#include <datatypes/listmodel.h>
#include <datatypes/eventitem.h>
#include <datatypes/event.h>

#include "musson_global.h"

#define TWELVE_HOURS_IN_SECS 43200

class musson_EXPORT EventManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel* eventsModel READ eventsModel NOTIFY eventsModelChanged)
    Q_PROPERTY(ListModel* eventsDayModel READ eventsDayModel NOTIFY eventsDayModelChanged)
    Q_PROPERTY(ListModel* singleEvent READ singleEvent NOTIFY singleEventChanged)
    Q_PROPERTY(bool isEventsUpdate READ isEventsUpdate NOTIFY isEventsUpdateChanged)
    Q_PROPERTY(int eventsCount READ eventsCount NOTIFY eventsCountChanged)
    Q_PROPERTY(int eventsOfDayCount READ eventsOfDayCount NOTIFY eventsOfDayCountChanged)
    Q_PROPERTY(QDate firstDayOfMonth READ firstDayOfMonth NOTIFY firstDayOfMonthChanged)
    Q_PROPERTY(int weekDayOfFirst READ weekDayOfFirst NOTIFY weekDayOfFirstChanged)
    Q_PROPERTY(int day READ day WRITE setDay NOTIFY dayChanged)
    Q_PROPERTY(int month READ month WRITE setMonth NOTIFY monthChanged)
    Q_PROPERTY(int year READ year WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(QString dateTitle READ dateTitle NOTIFY dateTitleChanged)
    Q_PROPERTY(QString mainTitle READ mainTitle NOTIFY dateChanged)
    Q_PROPERTY(QDate currentDate READ currentDate)
    Q_PROPERTY(QVariant owner READ owner WRITE setOwner NOTIFY ownerChanged)

public:
    explicit EventManager(QObject *parent = 0);

    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);
    ListModel* eventsModel();
    ListModel* eventsDayModel();
    ListModel* singleEvent();

    int eventsCount() const;
    bool isEventsUpdate() const;
    QDate firstDayOfMonth() const;
    int weekDayOfFirst() const;
    int day() const;
    int month() const;
    int year() const;
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);
    int eventsOfDayCount() const;
    QString dateTitle() const;
    QString mainTitle() const;
    QDate currentDate() const;
    QVariant owner();
    void setOwner(QVariant owner);

    int m_curDay;
    int m_tmpDay;
    int m_curMonth;
    int m_tmpMonth;
    int m_curYear;
    int m_tmpYear;
    QDate m_curDate;
    QDate m_tmpDate;
    QDate m_firstDayOfMonth;
    int m_weekDayOfFirst;
    QString m_dateTitle;
    QString m_mainTitle;
    ListModel* m_singleEvent;

signals:
    void eventsModelChanged();
    void isEventsUpdateChanged();
    void eventsDayModelChanged();
    void weekDayOfFirstChanged();
    void firstDayOfMonthChanged();
    void dayChanged();
    void monthChanged();
    void yearChanged();
    void dateChanged();
    void dateTitleChanged();
    void singleEventChanged();
    void ownerChanged();
    void eventsOfDayCountChanged();
    void eventsCountChanged();

public slots:
    bool createEvent(QString title, QString description, QString startTime,
                     QString endTime, QString location, QString privacy);
    void gotEvents(Friend, EventList list, bool isNeedUpdate);
    void refreshEvents();
    void refreshEvents(bool isNeedUpdate);
    void initEvents();
    QString dateToIsoFormat(int day, int month, int year, int hours, int mins);
    QString dateToIsoFormat(QString timeInUint);
    void eventsOfDay(QDate date);
    int getDayOfMonth(QDate date, int offset);
    void updateDate(int month);
    QDate getDateOfMonth(QDate date, int offset);
    bool compareDates(QDate date);
    QString getColorOfDay(QDate date, int offset);
    bool isToday(QDate date, int offset);
    void updateDateForTitleInt(int day);
    void updateDateForTitle(QDate date);
    void getEventInfo(int i);
    void getEventInfoFromFullList(int i);

private:
    int m_eventsCount;
    int m_eventsOfDayCount;
    bool m_isEventsUpdate;

    QString m_eventId;

    ListModel* m_eventsModel;
    ListModel* m_eventsDayModel;
    EventList m_internalEventsList;
    Friend m_owner;

    ServiceManager *m_sm;
};

#endif // EVENTMANAGER_H

