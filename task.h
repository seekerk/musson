#ifndef TASK_H
#define TASK_H

#include <QNetworkReply>
#include <QUrl>
#include <QSharedDataPointer>

namespace TaskType {
    enum TaskType {
        checkConnection,
        authorize
    };
}

class TaskData;

class Task
{

private:
    QSharedDataPointer<TaskData> d;

public:
    // constructor for started tasks
    explicit Task(TaskType::TaskType type, QUrl url, QNetworkReply *reply);
    explicit Task(TaskType::TaskType type, TaskType::TaskType reqTask, QUrl url);
    Task(const Task &task);

    ~Task();

    Task& operator =(const Task& other);
    bool operator ==(const Task& other) const;

    bool isFinished() const;

    // url
    QUrl url() const;
    void setUrl(QUrl url);

    TaskType::TaskType type() const;
    void setType(TaskType::TaskType type);

    QNetworkReply* reply() const;
    void setReply(QNetworkReply* reply);

    TaskType::TaskType requiredTask() const;
    void setRequiredTask(TaskType::TaskType reqType);
};

#endif // TASK_H
