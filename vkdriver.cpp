#include "vkdriver.h"
#include <QDebug>
#include <QVariantMap>
#include "json.h"
#include "webauth.h"

VKdriver::VKdriver()
{
}

void VKdriver::init()
{
    DriverTemplate::init();
    connect(&webauth, SIGNAL(success(QUrl)), SLOT(authorized(QUrl)));
    connect(&webauth, SIGNAL(error(QString)), SIGNAL(error(QString)));
}

void VKdriver::checkConnection()
{
    if (manager == NULL)
        init();

    QUrl url("https://api.vk.com/method/getUserSettings");
    url.addQueryItem("access_token", this->accessToken);

    QNetworkReply *resp = manager->get(QNetworkRequest(url));
    connect (resp, SIGNAL(finished()), this, SLOT(gotResponse()));

    tasks.append(Task(TaskType::checkConnection, url, resp));
}

void VKdriver::gotResponse()
{
    QList<Task> processedTasks;
    QList<Task> futureTasks;

    foreach (Task task, tasks)
    {
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": task" << task.url().toString();
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": task" << (task.reply()->isFinished() ? "finished" : "not finished");

        if (!task.isFinished())
            continue;

        processedTasks.append(task);

        QString respVal(task.reply()->readAll());
        qDebug() << __PRETTY_FUNCTION__ << __LINE__ << respVal;
        bool ok;
        QVariantMap result = QtJson::parse(respVal, ok).toMap();

        if (!ok)
        {
            qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "error!!!";
            emit error("An error occurred during parsing");
            continue;
        }

        if (result.contains("error"))
        {
            int errorCode = result["error"].toMap()["error_code"].toInt();

            // checks that authorization was needed
            if (task.type() == TaskType::checkConnection && errorCode == 113)
            {
                authorize();
                futureTasks.append(Task(task.type(), TaskType::authorize, task.url()));
            } else {
                qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "error_code=" << errorCode;
                emit error(result["error"].toMap()["error_msg"].toString());
            }
        }
    }

    foreach (Task task, processedTasks)
    {
        tasks.removeOne(task);
    }

    foreach (Task task, futureTasks)
    {
        tasks.append(task);
    }

    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": total tasks=" << tasks.length();
}

void VKdriver::authorize()
{
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": start authorization";
    QUrl url("https://oauth.vk.com/authorize");
    url.addQueryItem("client_id", QString(appId));
    url.addQueryItem("scope", "friends,photos");
    url.addQueryItem("display", "touch");
    url.addQueryItem("redirect_uri", "https://oauth.vk.com/blank.html");
    url.addQueryItem("response_type", "token");

    webauth.setSuccessRegExp(QRegExp("https://oauth\.vk\.com/blank\.html.+access_token=.+"));
    webauth.setErrorRegExp(QRegExp(".+error=.+"));
    webauth.load(url);
    webauth.show();
}

void VKdriver::authorized(QUrl url)
{
    this->accessToken = url.queryItemValue("access_token");
    this->userId = url.queryItemValue("user_id");
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": got token" << this->accessToken;
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": user ID" << this->userId;

    processRequests(TaskType::authorize);
}

void VKdriver::processRequests(TaskType::TaskType type)
{
    QList<Task> removedTasks;
    QList<Task> addedTasks;

    foreach (Task task, tasks)
    {
        if (task.requiredTask() == type && task.reply() != NULL)
        {
            removedTasks.append(task);
            QUrl url(task.url());
            if (type == TaskType::authorize)
            {
                url.removeQueryItem("access_token");
                url.addQueryItem("access_token", this->accessToken);
            }

            QNetworkReply *reply = manager->get(QNetworkRequest(url));
            connect (reply, SIGNAL(finished()), this, SLOT(gotResponse()));
            addedTasks.append(Task(task.type(), task.url(), reply));
        }
    }

    foreach (Task task, removedTasks)
    {
        tasks.removeOne(task);
    }

    foreach (Task task, addedTasks)
    {
        tasks.append(task);
    }
}
