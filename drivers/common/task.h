#ifndef TASK_H
#define TASK_H

#include <QNetworkReply>
#include <QUrl>
#include <QSharedDataPointer>

#include "interface.h"

class TaskData;

class Task
{

private:
    QSharedDataPointer<TaskData> d;

public:
    // constructor for started tasks
    explicit Task(Driver::Action type, QUrl url, QNetworkReply *reply);
    explicit Task(Driver::Action type, Driver::Action reqTask, QUrl url);
    Task(const Task &task);

    ~Task();

    Task& operator =(const Task& other);
    bool operator ==(const Task& other) const;

    bool isFinished() const;

    // url
    QUrl url() const;
    void setUrl(QUrl url);

    Driver::Action type() const;
    void setType(Driver::Action type);

    QNetworkReply* reply() const;
    void setReply(QNetworkReply* reply);

    Driver::Action requiredTask() const;
    void setRequiredTask(Driver::Action reqType);
};

#endif // TASK_H
