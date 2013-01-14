#include "qstatinfo.h"

#include <QSharedData>
#include <QDebug>
#include <QDomDocument>
#include <QDir>
#include <QMutex>

#include "utils/coresettings.h"

#define FILE_STATS_DATA "stats.xml"
#define NODE_STATS_ROOT "stats"
#define NODE_STATS_ERRORS "errors"
#define NODE_STATS_REQUESTS "requests"
#define NODE_STATS_DOWNLOAD "download"
#define NODE_STATS_UPLOAD "upload"

class QStatInfoData : public QSharedData
{
public:
    QStatInfoData();
    QStatInfoData(const QStatInfoData& other);
    ~QStatInfoData() {}

    QString accountId; /*!< The ID of account. */
    int requests;     /*!< total count of requests */
    long download;  /*!< total amount of download data in bytes */
    long upload;    /*!< total amount of upload data in bytes */
    int errors;     /*!< total count of errors */

    bool isTotalLoaded;
    int totalRequests;
    long totalDownload;
    long totalUpload;
    int totalErrors;

    QMutex mutex;
};

QStatInfoData::QStatInfoData()
    :QSharedData(),
      requests(0), download(0), upload(0), errors(0), isTotalLoaded(false), totalRequests(0), totalDownload(0), totalUpload(0), totalErrors(0)
{
}

QStatInfoData::QStatInfoData(const QStatInfoData &other)
    :QSharedData(other)
{
    this->accountId = other.accountId;
    this->requests = other.requests;
    this->download = other.download;
    this->upload = other.upload;
    this->errors = other.errors;

    this->isTotalLoaded = other.isTotalLoaded;
    this->totalDownload = other.totalDownload;
    this->totalErrors = other.totalErrors;
    this->totalRequests = other.totalRequests;
    this->totalUpload = other.totalUpload;
}

QStatInfo::QStatInfo()
{
    d = new QStatInfoData();
}

QStatInfo::QStatInfo(const QStatInfo &other)
    : d(other.d)
{

}

QStatInfo::~QStatInfo()
{

}

int QStatInfo::requests() const
{
    return d->requests;
}

void QStatInfo::setRequests(int val)
{
    d->mutex.lock();
    d->requests = val;
    d->mutex.unlock();
}

void QStatInfo::addRequest(int val)
{
    d->mutex.lock();
    d->requests += val;
    d->mutex.unlock();
}

long QStatInfo::download() const
{
    return d->download;
}

void QStatInfo::setDownload(long val)
{
    d->mutex.lock();
    d->download = val;
    d->mutex.unlock();
}

void QStatInfo::addDownload(long val)
{
    d->mutex.lock();
    d->download += val;
    d->mutex.unlock();
}

long QStatInfo::upload() const
{
    return d->upload;
}

void QStatInfo::setUpload(long val)
{
    d->mutex.lock();
    d->upload = val;
    d->mutex.unlock();
}

void QStatInfo::addUpload(long val)
{
    d->mutex.lock();
    d->upload += val;
    d->mutex.unlock();
}

int QStatInfo::errors() const
{
    return d->errors;
}

void QStatInfo::setErrors(int val)
{
    d->mutex.lock();
    d->errors = val;
    d->mutex.unlock();
}

void QStatInfo::addError(int val)
{
    d->mutex.lock();
    d->errors += val;
    d->mutex.unlock();
}

const QString& QStatInfo::accountId() const
{
    return d->accountId;
}

void QStatInfo::setAccountId(const QString &accountId)
{
    d->mutex.lock();
    d->accountId = accountId;
    d->mutex.unlock();
}

QStatInfo& QStatInfo::operator +=(const QStatInfo& other)
{
    this->addRequest(other.requests());
    this->addUpload(other.upload());
    this->addDownload(other.download());
    this->addError(other.errors());

    qDebug() << __FILE__ << __LINE__ << __FUNCTION__<< "download=" << d->download << "upload=" << d->upload << "requests=" << d->requests << "errors=" << d->errors;
    return *this;
}

QStatInfo& QStatInfo::operator=(const QStatInfo& other)
{
    d = other.d;
    return *this;
}

void QStatInfo::store()
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__<< "download=" << d->download << "upload=" << d->upload << "requests=" << d->requests << "errors=" << d->errors;

    if (d->download == 0 &&
            d->upload == 0 &&
            d->requests == 0 &&
            d->errors == 0)
        return;

    // load previous stats
    QStatInfo total = getTotal();

    total += *this;
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__<< "download=" << total.download() << "upload=" << total.upload() << "requests=" << total.requests() << "errors=" << total.errors();

    // store total stats
    QDomDocument doc("MyDoc");
    QDomElement ret = doc.createElement(NODE_STATS_ROOT);
    doc.appendChild(ret);

    //requests
    if (total.requests() > 0)
    {
        QDomElement node = doc.createElement(NODE_STATS_REQUESTS);
        QDomText t = doc.createTextNode(QString::number(total.requests()));
        ret.appendChild(node);
        node.appendChild(t);
    }

    //download
    if (total.download() > 0)
    {
        QDomElement node = doc.createElement(NODE_STATS_DOWNLOAD);
        QDomText t = doc.createTextNode(QString::number(total.download()));
        ret.appendChild(node);
        node.appendChild(t);
    }

    //upload
    if (total.upload() > 0)
    {
        QDomElement node = doc.createElement(NODE_STATS_UPLOAD);
        QDomText t = doc.createTextNode(QString::number(total.upload()));
        ret.appendChild(node);
        node.appendChild(t);
    }

    //errors
    if (total.errors() > 0)
    {
        QDomElement node = doc.createElement(NODE_STATS_ERRORS);
        QDomText t = doc.createTextNode(QString::number(total.errors()));
        ret.appendChild(node);
        node.appendChild(t);
    }

    QDir dir;

    dir.setPath(dir.cleanPath(CoreSettings::getAccountDir(d->accountId)));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QFile file(dir.absolutePath() + "/" + QString(FILE_STATS_DATA));
    qDebug() << file.fileName();
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
        qDebug() << "can't open file: " + file.errorString();
    }
    //file.write(output.toUtf8());
    file.write(doc.toByteArray(1));
    file.close();
}

QStatInfo QStatInfo::getTotal()
{
    if (!d->isTotalLoaded && !d->accountId.isEmpty())
    {
        QFile infile(CoreSettings::getAccountDir(d->accountId) + QString(FILE_STATS_DATA));
        if (infile.open(QIODevice::ReadOnly))
        {
            QDomDocument doc;
            doc.setContent(&infile);
            infile.close();

            if (doc.elementsByTagName(NODE_STATS_REQUESTS).count() > 0)
                d->totalRequests = (doc.elementsByTagName(NODE_STATS_REQUESTS).at(0).toElement().firstChild().toText().data().toInt());

            if (doc.elementsByTagName(NODE_STATS_DOWNLOAD).count() > 0)
                d->totalDownload = (doc.elementsByTagName(NODE_STATS_DOWNLOAD).at(0).toElement().firstChild().toText().data().toLong());

            if (doc.elementsByTagName(NODE_STATS_UPLOAD).count() > 0)
                d->totalUpload = (doc.elementsByTagName(NODE_STATS_UPLOAD).at(0).toElement().firstChild().toText().data().toLong());

            if (doc.elementsByTagName(NODE_STATS_ERRORS).count() > 0)
                d->totalErrors = (doc.elementsByTagName(NODE_STATS_ERRORS).at(0).toElement().firstChild().toText().data().toInt());

            d->isTotalLoaded = true;
        }
    }

    QStatInfo ret;
    ret.setAccountId(d->accountId);
    ret.setDownload(d->totalDownload);
    ret.setErrors(d->totalErrors);
    ret.setRequests(d->totalRequests);
    ret.setUpload(d->totalUpload);
    return ret;
}

void QStatInfo::clean()
{
    d->mutex.lock();
    d->upload = 0;
    d->download = 0;
    d->requests = 0;
    d->errors = 0;
    d->mutex.unlock();
}
