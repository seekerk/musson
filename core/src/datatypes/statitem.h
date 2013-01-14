#ifndef STATITEM_H
#define STATITEM_H

/**
  \file statitem.h
  \brief QStatInfo delegate for ListModel.

  Delegate for QStatInfo datatype from musson-core library.

  \author Musson team

  \date 2011-10-03
  */

#include <QObject>
#include <datatypes/listmodel.h>
#include <datatypes/qstatinfo.h>
#include "musson_global.h"

/**
 * @brief  A class for presenting qstatinfo type from musson-core.
 *
 */
class musson_EXPORT StatItem : public ListItem
{
    Q_OBJECT
public:
    /**
    * @brief Empty constructor.
    *
    * @param parent Parent object.
    */
    StatItem(QObject *parent = 0) : ListItem(parent) {};

    /**
     * @brief Creates an instance of StatItem based on QStatInfo.
     *
     * @param info Statistics information.
     * @param totalInfo Total statistics information.
     * @param parent Parent object.
     */
    explicit StatItem(QStatInfo info, QStatInfo totalInfo, QObject* parent = 0);

    /**
     * @brief Destructor.
     *
     */
    ~StatItem();

    /**
     * @brief Get data based on defined role.
     *
     * @param role Data role.
     */
    QVariant data(int role) const;

    /**
     * @brief Get hashtable of role names.
     *
     * @return Hashtable <role index, role name>.
     */
    QHash<int, QByteArray> roleNames() const;

    /**
     * @brief Get count of requests.
     *
     * @return Count of requests.
     */
    int requests() const;

    /**
     * @brief Get total count of requests.
     *
     * @return Total count of requests.
     */
    int totalRequests() const;

    /**
     * @brief Get size of downloaded data.
     *
     * @return Size of downloaded data.
     */
    long long download() const;

    /**
     * @brief Get size of downloaded data in total.
     *
     * @return Size of downloaded data in total.
     */
    long long totalDownload() const;

    /**
     * @brief Get size of uploaded data.
     *
     * @return Size of uploaded data.
     */
    long long upload() const;

    /**
     * @brief Get size of uploaded data in total.
     *
     * @return Size of uploaded data in total.
     */
    long long totalUpload() const;

    /**
     * @brief Get count of errors.
     *
     * @return Count of errors.
     */
    int errors() const;

    /**
     * @brief Get total count of errors.
     *
     * @return Total count of errors.
     */
    int totalErrors() const;

    /**
     * @brief Get id of StatItem.
     *
     * @return Id of StatItem.
     */
    QString id() const;

    /**
     * @brief Get keu value of StatItem
     * It is using for sorting items.
     *
     * @return key value
     */
    QVariant key() const;

    /**
     * @brief Describes roles.
     *
     */
    enum Roles {
        RequestsRole = Qt::UserRole + 1, /**< Count of requests. */
        TotalRequestsRole, /**< Total count of requests. */
        DownloadRole, /**< Size of downloaded data. */
        TotalDownloadRole, /**< Size of downloaded data in total. */
        UploadRole, /**< Size of uploaded data. */
        TotalUploadRole, /**< Size of uploaded data in total. */
        ErrorsRole, /**< Count of errors. */
        TotalErrorsRole, /**< Total count of requests. */
        IDRole /**< Id of StatItem. */
    };

private:
    int m_requests;
    int m_totalRequests;
    long long m_download;
    long long m_totalDownload;
    long long m_upload;
    long long m_totalUpload;
    int m_errors;
    int m_totalErrors;
    QString m_accountId;
};

#endif // STATITEM_H
