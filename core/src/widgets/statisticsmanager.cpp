#include "statisticsmanager.h"

StatisticsManager::StatisticsManager(QObject *parent) :
    QObject(parent)
{
    m_model = new ListModel(new StatItem, this);
    m_totalRequests = 0;
    m_totalDownload = 0;
    m_totalUpload = 0;
    m_totalErrors = 0;
    m_requests = 0;
    m_download = 0;
    m_upload = 0;
    m_errors = 0;
}

StatisticsManager::~StatisticsManager()
{
    delete m_model;
}

void StatisticsManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;
    updateStats();
}

ServiceManager* StatisticsManager::sm()
{
    return m_sm;
}

ListModel* StatisticsManager::model()
{
    return m_model;
}

int StatisticsManager::totalRequests() const
{
    return m_totalRequests;
}

long StatisticsManager::totalDownload() const
{
    return m_totalDownload;
}

long StatisticsManager::totalUpload() const
{
    return m_totalUpload;
}

int StatisticsManager::totalErrors() const
{
    return m_totalErrors;
}

int StatisticsManager::requests() const
{
    return m_requests;
}

long StatisticsManager::download() const
{
    return m_download;
}

long StatisticsManager::upload() const
{
    return m_upload;
}

int StatisticsManager::errors() const
{
    return m_errors;
}

void StatisticsManager::updateStats()
{
    QList<QStatInfo> stats = m_sm->getStats();

    ListModel* m = new ListModel(new StatItem);
    delete m_model;
    m_model = m;

    for (int i = 0; i < stats.length(); i++) {
        m_requests += stats.at(i).requests();
        m_download += stats.at(i).download();
        m_upload += stats.at(i).upload();
        m_errors += stats.at(i).errors();

        QStatInfo ttlInfo = stats[i].getTotal();
        m_totalRequests += ttlInfo.requests();
        m_totalDownload += ttlInfo.download();
        m_totalUpload += ttlInfo.upload();
        m_totalErrors += ttlInfo.errors();

        m_model->appendRow(new StatItem(stats.at(i), ttlInfo));
    }

    emit requestsChanged();
    emit downloadChanged();
    emit uploadChanged();
    emit errorsChanged();
    emit totalRequestsChanged();
    emit totalDownloadChanged();
    emit totalUploadChanged();
    emit totalErrorsChanged();

    emit modelChanged();
}
