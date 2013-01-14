#ifndef STATISTICSMANAGER_H
#define STATISTICSMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>
#include <datatypes/listmodel.h>
#include <datatypes/statitem.h>
#include <datatypes/qstatinfo.h>
#include "musson_global.h"

class musson_EXPORT StatisticsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel *model READ model NOTIFY modelChanged)
    Q_PROPERTY(int totalRequests READ totalRequests NOTIFY totalRequestsChanged)
    Q_PROPERTY(long totalDownload READ totalDownload NOTIFY totalDownloadChanged)
    Q_PROPERTY(long totalUpload READ totalUpload NOTIFY totalUploadChanged)
    Q_PROPERTY(int totalErrors READ totalErrors NOTIFY totalErrorsChanged)
    Q_PROPERTY(int requests READ requests NOTIFY requestsChanged)
    Q_PROPERTY(long download READ download NOTIFY downloadChanged)
    Q_PROPERTY(long upload READ upload NOTIFY uploadChanged)
    Q_PROPERTY(int errors READ errors NOTIFY errorsChanged)
public:
    explicit StatisticsManager(QObject *parent = 0);
    ~StatisticsManager();

    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);
    ListModel* model();

    int requests() const;
    int totalRequests() const;
    long download() const;
    long totalDownload() const;
    long upload() const;
    long totalUpload() const;
    int errors() const;
    int totalErrors() const;

signals:
    void modelChanged();
    void requestsChanged();
    void totalRequestsChanged();
    void downloadChanged();
    void totalDownloadChanged();
    void uploadChanged();
    void totalUploadChanged();
    void errorsChanged();
    void totalErrorsChanged();

public slots:
    void updateStats();

private:
    ListModel* m_model;
    ServiceManager *m_sm;
    int m_totalRequests;
    int m_requests;
    long m_totalDownload;
    long m_download;
    long m_totalUpload;
    long m_upload;
    int m_totalErrors;
    int m_errors;

};

#endif // STATISTICSMANAGER_H
