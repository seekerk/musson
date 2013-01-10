#include "task.h"

class TaskData : public QSharedData
{
public:
    TaskType::TaskType type;
    QNetworkReply *reply;

    // used if task needed to have completed other task.
    TaskType::TaskType requiredTask;
    QUrl url;

    TaskData()
        : QSharedData(), reply(NULL), requiredTask(TaskType::authorize)
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

Task::Task(TaskType::TaskType type, QUrl url, QNetworkReply *reply)
{
    d = new TaskData;
    d->type = type;
    d->url = url;
    d->reply = reply;
}

Task::Task(TaskType::TaskType type, TaskType::TaskType reqTask, QUrl url)
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

TaskType::TaskType Task::type() const { return d->type; }
void Task::setType(TaskType::TaskType type) { d->type = type; }

TaskType::TaskType Task::requiredTask() const { return d->requiredTask; }
void Task::setRequiredTask(TaskType::TaskType reqType) { d->requiredTask = reqType; }

QNetworkReply* Task::reply() const { return d->reply; }
void Task::setReply(QNetworkReply *reply) { d->reply = reply; }
