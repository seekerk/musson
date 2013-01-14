#include "statitem.h"

StatItem::StatItem(QStatInfo info, QStatInfo totalInfo, QObject *parent) :
    ListItem(parent)
{
    m_accountId = info.accountId();
    m_requests = info.requests();
    m_totalRequests = totalInfo.requests();
    m_download = info.download();
    m_totalDownload = totalInfo.download();
    m_upload = info.upload();
    m_totalUpload = totalInfo.upload();
    m_errors = info.errors();
    m_totalErrors = totalInfo.errors();
}

StatItem::~StatItem()
{

}

QHash<int, QByteArray> StatItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[RequestsRole] = "requests";
    names[TotalRequestsRole] = "totalRequests";
    names[DownloadRole] = "download";
    names[TotalDownloadRole] = "totalDownload";
    names[UploadRole] = "upload";
    names[TotalUploadRole] = "totalUpload";
    names[ErrorsRole] = "errors";
    names[TotalErrorsRole] = "totalErrors";
    names[IDRole] = "accountId";
    return names;
}

int StatItem::requests() const
{
    return m_requests;
}

int StatItem::totalRequests() const
{
    return m_totalRequests;
}

int StatItem::errors() const
{
    return m_errors;
}

int StatItem::totalErrors() const
{
    return m_totalErrors;
}

long long StatItem::download() const
{
    return m_download;
}

long long StatItem::totalDownload() const
{
    return m_totalDownload;
}

long long StatItem::upload() const
{
    return m_upload;
}

long long StatItem::totalUpload() const
{
    return m_totalUpload;
}

QString StatItem::id() const
{
    return m_accountId;
}

QVariant StatItem::key() const
{
    return m_accountId;
}

QVariant StatItem::data(int role) const
{
    switch(role) {
    case RequestsRole:
        return requests();
        break;
    case TotalRequestsRole:
        return totalRequests();
        break;
    case ErrorsRole:
        return errors();
        break;
    case TotalErrorsRole:
        return totalErrors();
        break;
    case DownloadRole:
        return download();
        break;
    case TotalDownloadRole:
        return totalDownload();
        break;
    case UploadRole:
        return upload();
        break;
    case TotalUploadRole:
        return totalUpload();
        break;
    case IDRole:
        return id();
        break;
    default:
        return QVariant();
    }
}
