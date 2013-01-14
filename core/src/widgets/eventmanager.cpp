#include "eventmanager.h"

EventManager::EventManager(QObject *parent) :
    QObject(parent)
{
    m_eventsModel = new ListModel(new EventItem, this);
    m_eventsDayModel = new ListModel(new EventItem, this);
    m_singleEvent = new ListModel(new EventItem, this);
    m_eventsCount = 0;
    m_isEventsUpdate = false;
    m_curDate = m_tmpDate = QDate::currentDate();
    m_curDay = m_tmpDay = m_curDate.day();
    m_curMonth = m_tmpMonth = m_curDate.month();
    m_curYear = m_tmpYear = m_curDate.year();
    m_firstDayOfMonth = QDate(m_curYear, m_curMonth, 1);
    m_weekDayOfFirst = m_firstDayOfMonth.dayOfWeek();
    m_mainTitle = m_firstDayOfMonth.longMonthName(m_tmpMonth, QDate::StandaloneFormat) + " " + QString::number(m_tmpYear, 10);
    qDebug() << m_tmpDay << " " << m_tmpMonth << " " << m_tmpYear << endl;
}

void EventManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;
    connect(m_sm, SIGNAL(updateEventList(Friend, EventList, bool)),
            this, SLOT(gotEvents(Friend ,EventList, bool)));
}

void EventManager::initEvents()
{
    this->refreshEvents(false);
}

ServiceManager* EventManager::sm()
{
    return m_sm;
}

ListModel* EventManager::eventsModel()
{
    return m_eventsModel;
}

ListModel* EventManager::eventsDayModel()
{
    return m_eventsDayModel;
}

int EventManager::eventsCount() const
{
    return m_eventsCount;
}

int EventManager::eventsOfDayCount() const
{
    return m_eventsOfDayCount;
}

bool EventManager::isEventsUpdate() const
{
    return m_isEventsUpdate;
}

QDate EventManager::firstDayOfMonth() const
{
    return m_firstDayOfMonth;
}

int EventManager::weekDayOfFirst() const
{
    return m_weekDayOfFirst;
}

int EventManager::day() const
{
    return m_tmpDay;
}

int EventManager::month() const
{
    return m_tmpMonth;
}

int EventManager::year() const
{
    return m_tmpYear;
}

void EventManager::setDay(int d)
{
    if (d != m_tmpDay)
        this->m_tmpDay = d;
}

void EventManager::setMonth(int m)
{
    if (m != m_tmpMonth)
        this->m_tmpMonth = m;
}

void EventManager::setYear(int y)
{
    if (y != m_tmpYear)
        this->m_tmpYear = y;
}

QString EventManager::dateTitle() const
{
    return m_dateTitle;
}

QString EventManager::mainTitle() const
{
    return m_mainTitle;
}

QDate EventManager::currentDate() const
{
    return m_tmpDate;
}

ListModel* EventManager::singleEvent()
{
    return m_singleEvent;
}

bool EventManager::createEvent(QString title, QString description, QString startTime,
                               QString endTime, QString location, QString privacy){

    bool status = m_sm->createEvent(title, description, startTime, endTime, location, privacy);
    this->refreshEvents(true);
    return status;
}

QVariant EventManager::owner()
{
    QVariant v;
    v.setValue(m_owner);
    return v;
}

void EventManager::setOwner(QVariant owner)
{
    if (owner.canConvert<Friend>()) {
        m_owner = owner.value<Friend>();

        if (!m_owner.isEmpty()) {
            m_sm->getEvents(m_owner, true, true);
        } else {
            m_sm->getEvents(false, true);
        }
        emit ownerChanged();
    } else {
        Friend fd;
        m_owner = fd;
        m_sm->getEvents(false, true);
    }
}

void EventManager::refreshEvents()
{
    this->refreshEvents(true);
}

void EventManager::refreshEvents(bool isNeedUpdate)
{
    qDebug() << "\n\n !!!owner!!! \n\n" << m_owner.ownerId() << endl;
    if (!m_isEventsUpdate) {
        m_isEventsUpdate = true;
        emit isEventsUpdateChanged();
        if (!m_owner.isEmpty()) {
            m_sm->getEvents(m_owner, isNeedUpdate, true);
        } else {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): two" << endl;
            m_sm->getEvents(isNeedUpdate, true);
        }
    }
}

void EventManager::gotEvents(Friend, EventList list, bool isLastUpdate)
{
    if (isLastUpdate)
        m_isEventsUpdate = false;
    else
        m_isEventsUpdate = true;
    emit isEventsUpdateChanged();

    ListModel* tmp = new ListModel(new EventItem);
    delete m_eventsModel;
    m_eventsCount = 0;
    m_eventsModel = tmp;

    m_internalEventsList = list;
    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "(): events got" << endl;

    m_eventsCount = list.length();
    for (int i = 0; i < list.length(); i++) {
        m_eventsModel->appendRow(new EventItem(list.at(i)));
    }

    emit eventsModelChanged();
    emit firstDayOfMonthChanged();
    emit eventsCountChanged();
}

QString EventManager::dateToIsoFormat(int day, int month,
                                      int year, int hours, int mins)
{
    QDate tmp(year, month, day);
    QTime time(hours, mins, 0, 0);
    QDateTime* date = new QDateTime(tmp);
    date->setTime(time);
    qDebug() << __FUNCTION__ << date->toString(Qt::ISODate) << endl;
    return date->toString(Qt::ISODate);
}

QString EventManager::dateToIsoFormat(QString timeInUint)
{
    QDateTime dateToIso;
    bool ok;
    uint time = timeInUint.toUInt(&ok,10);
    if (ok) {
        dateToIso.setTime_t(time);
        dateToIso.addSecs(-43200);
        return dateToIso.toString("dd.MM.yyyy hh:mm:ss")/*.left(10)*/;
    }
    else {
        qDebug() << __FUNCTION__ << "Incorrect transform from Uint" << endl;
        return "";
    }
}


/*void EventManager::eventsOfDay(QDate date)
{
    QString tmpDayDateStr = date.toString(Qt::ISODate);
    QDateTime dayDate;
    QModelIndex index;
    QString tmpStartTime;
    QVariant tmp;
    bool ok;
    ListModel* tmpModel = new ListModel(new EventItem);
    delete m_eventsDayModel;
    m_eventsDayModel = tmpModel;

    EventItem* tmpItem = new EventItem;



    if (m_eventsCount > 0) {
        for (int i = 0; i < m_eventsCount; i++) {
            index = m_eventsModel->index(i,0);
            tmp = m_eventsModel->data(index, EventItem::StartTimeRole);
            tmpStartTime = tmp.toString();
            dayDate = QDateTime::fromTime_t(tmpStartTime.toUInt(&ok,10));
            if (ok) {
                tmpStartTime = dayDate.toString(Qt::ISODate);
                if (QString::compare(tmpDayDateStr.left(10), tmpStartTime.left(10)) == 0) {
                    qDebug() << __FUNCTION__ << "dates: " << tmpDayDateStr.left(10) << tmpStartTime.left(10) <<endl;
                    qDebug() <<__FUNCTION__<<"2"<<endl;
                    tmpItem->setTitle(m_eventsModel->data(index, EventItem::TitleRole));
                    qDebug() <<__FUNCTION__<<"3"<<endl;
                    tmpItem->setDescription(m_eventsModel->data(index, EventItem::DescriptionRole));
                    qDebug() <<__FUNCTION__<<"4"<<endl;
                    tmpItem->setStartTime(m_eventsModel->data(index, EventItem::StartTimeRole));
                    qDebug() <<__FUNCTION__<<"5"<<endl;
                    tmpItem->setEndTime(m_eventsModel->data(index, EventItem::EndTimeRole));
                    qDebug() <<__FUNCTION__<<"6"<<endl;
                    tmpItem->setLocation(m_eventsModel->data(index, EventItem::LocationRole));
                    qDebug() <<__FUNCTION__<<"7"<<endl;
                    m_eventsDayModel->appendRow(tmpItem);
                    qDebug() <<__FUNCTION__<<"8"<<endl;
                }
            }
            else {
                qDebug() <<"incorrect transform"<<endl;
            }
        }
    }
    emit eventsDayModelChanged();
    //emit firstDayOfMonthChanged();
}*/


void EventManager::eventsOfDay(QDate date)
{
    qDebug() << __FUNCTION__ << endl;
    QString tmpDayDateStr = date.toString(Qt::ISODate);
    QDateTime dayDate;
    QString tmpStartTime;
    bool ok;
    ListModel* tmpModel = new ListModel(new EventItem);
    delete m_eventsDayModel;
    m_eventsOfDayCount = 0;
    m_eventsDayModel = tmpModel;

    for (int i = 0; i < m_internalEventsList.length(); i++) {
        tmpStartTime = m_internalEventsList.at(i).startTime();
        qDebug() << "start time from list" << tmpStartTime << endl;
        dayDate = QDateTime::fromTime_t(tmpStartTime.toUInt(&ok,10));
        dayDate.addSecs(43200);
        if (ok) {
            tmpStartTime = dayDate.toString(Qt::ISODate);
            qDebug() << "start time from time t" << dayDate.toString(Qt::ISODate) << endl;
            if (QString::compare(tmpDayDateStr.left(10), tmpStartTime.left(10)) == 0) {
                qDebug() << __FUNCTION__ << "dates: " << tmpDayDateStr.left(10) << tmpStartTime.left(10) <<endl;
                m_eventsDayModel->appendRow(new EventItem(m_internalEventsList.at(i)));
                m_eventsOfDayCount++;
            }
        }
        else {
            qDebug() <<"incorrect transform"<<endl;
        }
    }
    emit eventsDayModelChanged();
    emit firstDayOfMonthChanged();
    emit weekDayOfFirstChanged();
    emit eventsOfDayCountChanged();
}


int EventManager::getDayOfMonth(QDate date, int offset)
{
    QDate tmpDate;
    tmpDate = date.addDays(offset);
    return tmpDate.day();
}

QDate EventManager::getDateOfMonth(QDate date, int offset)
{
    QDate tmpDate;
    tmpDate = date.addDays(offset);
    return tmpDate;
}

QString EventManager::getColorOfDay(QDate date, int offset)
{
    QDate tmpDate;
    tmpDate = date.addDays(offset);
    if (tmpDate.month() == date.month())
        return "black";
    else
        return "#848284";
}

bool EventManager::compareDates(QDate date)
{
    QString tmpDayDateStr = date.toString(Qt::ISODate);
    QDateTime dayDate;
    QString tmpStartTime;
    bool ok;

    for (int i = 0; i < m_internalEventsList.length(); i++) {
        tmpStartTime = m_internalEventsList.at(i).startTime();
        //qDebug() << "timestamp (origin): " << tmpStartTime << endl;
        //qDebug() << "timestamp (after transp): " << tmpStartTime.toUInt(&ok,10) << endl;
        dayDate.setTime_t(tmpStartTime.toUInt(&ok,10));
        dayDate.addSecs(2*TWELVE_HOURS_IN_SECS);
        if (ok) {
            tmpStartTime = dayDate.toString(Qt::ISODate);
            if (QString::compare(tmpDayDateStr.left(10), tmpStartTime.left(10)) == 0)
                    return true;
        }
        else {
            qDebug() <<"incorrect transform"<<endl;
        }
    }
    return false;
}

void EventManager::updateDateForTitle(QDate date)
{
    m_tmpDate = date;
    m_tmpDay = m_tmpDate.day();
    m_tmpMonth = m_tmpDate.month();
    m_tmpYear = m_tmpDate.year();
    m_firstDayOfMonth = QDate(m_tmpYear, m_tmpMonth, 1);
    m_weekDayOfFirst = m_firstDayOfMonth.dayOfWeek();

    m_dateTitle = m_tmpDate.toString("dd.MM.yyyy");
    m_mainTitle = m_firstDayOfMonth.longMonthName(m_tmpMonth, QDate::StandaloneFormat) + " " + QString::number(m_tmpYear, 10);

    emit firstDayOfMonthChanged();
    emit weekDayOfFirstChanged();
    emit dayChanged();
    emit monthChanged();
    emit yearChanged();
    emit dateChanged();
    emit dateTitleChanged();
}

void EventManager::updateDateForTitleInt(int day)
{
    m_tmpDate = m_tmpDate.addDays(day);
    m_tmpDay = m_tmpDate.day();
    m_tmpMonth = m_tmpDate.month();
    m_tmpYear = m_tmpDate.year();
    m_firstDayOfMonth = QDate(m_tmpYear, m_tmpMonth, 1);
    m_weekDayOfFirst = m_firstDayOfMonth.dayOfWeek();

    m_dateTitle = m_tmpDate.toString("dd.MM.yyyy");
    m_mainTitle = m_firstDayOfMonth.longMonthName(m_tmpMonth, QDate::StandaloneFormat) + " " + QString::number(m_tmpYear, 10);

    emit firstDayOfMonthChanged();
    emit weekDayOfFirstChanged();
    emit dayChanged();
    emit monthChanged();
    emit yearChanged();
    emit dateChanged();
    emit dateTitleChanged();
}

void EventManager::updateDate(int month)
{
    m_tmpDate = m_tmpDate.addMonths(month);
    m_tmpDay = m_tmpDate.day();
    m_tmpMonth = m_tmpDate.month();
    m_tmpYear = m_tmpDate.year();
    m_firstDayOfMonth = QDate(m_tmpYear, m_tmpMonth, 1);
    m_weekDayOfFirst = m_firstDayOfMonth.dayOfWeek();

    m_mainTitle = m_firstDayOfMonth.longMonthName(m_tmpMonth, QDate::StandaloneFormat) + " " + QString::number(m_tmpYear, 10);

    emit firstDayOfMonthChanged();
    emit weekDayOfFirstChanged();
    emit dayChanged();
    emit monthChanged();
    emit yearChanged();
    emit dateChanged();
}

bool EventManager::isToday(QDate date, int offset)
{
    QDate tmpDate;
    tmpDate = date.addDays(offset);
    m_curDate = QDate::currentDate();
    return tmpDate == m_curDate;
}

void EventManager::getEventInfoFromFullList(int i)
{
    ListModel* tmpModel = new ListModel(new EventItem);
    EventItem* tmpItem = new EventItem;
    QModelIndex index = m_eventsModel->index(i,0);
    delete m_singleEvent;
    m_singleEvent = tmpModel;
    tmpItem->setTitle(m_eventsModel->data(index, EventItem::TitleRole));
    tmpItem->setDescription(m_eventsModel->data(index, EventItem::DescriptionRole));
    tmpItem->setStartTime(m_eventsModel->data(index, EventItem::StartTimeRole));
    tmpItem->setEndTime(m_eventsModel->data(index, EventItem::EndTimeRole));
    tmpItem->setLocation(m_eventsModel->data(index, EventItem::LocationRole));
    tmpItem->setIcon(m_eventsModel->data(index, EventItem::IconRole));
    tmpItem->setCreator(m_eventsModel->data(index, EventItem::CreatorRole));
    m_singleEvent->appendRow(tmpItem);
    emit singleEventChanged();
}

void EventManager::getEventInfo(int i)
{
    ListModel* tmpModel = new ListModel(new EventItem);
    EventItem* tmpItem = new EventItem;
    QModelIndex index = m_eventsDayModel->index(i,0);
    delete m_singleEvent;
    m_singleEvent = tmpModel;
    tmpItem->setTitle(m_eventsDayModel->data(index, EventItem::TitleRole));
    tmpItem->setDescription(m_eventsDayModel->data(index, EventItem::DescriptionRole));
    tmpItem->setStartTime(m_eventsDayModel->data(index, EventItem::StartTimeRole));
    tmpItem->setEndTime(m_eventsDayModel->data(index, EventItem::EndTimeRole));
    tmpItem->setLocation(m_eventsDayModel->data(index, EventItem::LocationRole));
    tmpItem->setIcon(m_eventsDayModel->data(index, EventItem::IconRole));
    tmpItem->setCreator(m_eventsDayModel->data(index, EventItem::CreatorRole));
    m_singleEvent->appendRow(tmpItem);
    emit singleEventChanged();
}

/*
void EventManager::getEventInfo(int index)
{
    ListModel* tmpModel = new ListModel(new EventItem);
    delete m_singleEvent;
    m_singleEvent = tmpModel;
    m_singleEvent->appendRow(new EventItem(m_internalEventsList.at(index)));
    emit singleEventChanged();
}
*/

