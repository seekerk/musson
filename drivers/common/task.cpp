#include "task.h"

class TaskData : public QSharedData
{
public:
    Driver::Action type;
    QNetworkReply *reply;

    // used if task needed to have completed other task.
    Driver::Action requiredTask;
    QUrl url;

    TaskData()
        : QSharedData(), reply(NULL), requiredTask(Driver::authorize)
    {}

    TaskData(const TaskData &other)
        : QSharedData(other)
    {
        this->type = other.type;
        this->reply = other.reply;
        this->requiredTask = other.requiredTask;
        this->url = other.url;
    }

    ~ TaskData()
    {}
};

Task::Task(Driver::Action type, QUrl url, QNetworkReply *reply)
{
    d = new TaskData;
    d->type = type;
    d->url = url;
    d->reply = reply;
}

Task::Task(Driver::Action type, Driver::Action reqTask, QUrl url)
{
    d = new TaskData;
    d->type = type;
    d->requiredTask = reqTask;
    d->url = url;
}

Task::Task(const Task& task)
    : d(task.d)
{}

Task::~Task()
{}

bool Task::operator ==(const Task& other) const
{
    return (d->reply == other.reply() && d->type == other.type() &&
            d->requiredTask == other.requiredTask() && d->url == other.url());
}

Task& Task::operator =(const Task& other)
{
    d = other.d;
    return *this;
}

bool Task::isFinished() const
{
    return (d->reply == NULL ? false : d->reply->isFinished());
}

QUrl Task::url() const { return d->url; }
void Task::setUrl(QUrl url) { d->url = url; }

Driver::Action Task::type() const { return d->type; }
void Task::setType(Driver::Action type) { d->type = type; }

Driver::Action Task::requiredTask() const { return d->requiredTask; }
void Task::setRequiredTask(Driver::Action reqType) { d->requiredTask = reqType; }

QNetworkReply* Task::reply() const { return d->reply; }
void Task::setReply(QNetworkReply *reply) { d->reply = reply; }
