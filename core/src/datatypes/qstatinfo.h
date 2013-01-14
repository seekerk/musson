#ifndef QSTATINFO_H
#define QSTATINFO_H

#include <QMetaType>
#include <QSharedDataPointer>
#include "musson_global.h"

class QStatInfo;

Q_DECLARE_METATYPE(QStatInfo)

// QStatInfoData defined in QStatInfo.cpp
class QStatInfoData;

class musson_EXPORT QStatInfo
{
    QSharedDataPointer<QStatInfoData> d;
public:
    //! Default constructor
    QStatInfo();

    //! Copy constructor
    QStatInfo(const QStatInfo& other);

    ~QStatInfo();

    //! Gets total amount of requests
    int requests() const;

    //! Sets total amount of requests
    void setRequests(int val);

    //! Increment total amount of requests
    void addRequest() {this->addRequest(1);}

    //! Append requests to total amount
    void addRequest(int val);

    //! Gets total amount of downloads
    long download() const;

    //! Sets total amount of downloads
    void setDownload(long val);

    void addDownload(long val);

    long upload() const;

    void setUpload(long val);

    void addUpload(long val);

    int errors() const;

    void setErrors(int val);

    void addError() {this->addError(1);}

    void addError(int val);

    const QString& accountId() const;

    void setAccountId(const QString& accountId);

    QStatInfo& operator+=(const QStatInfo& val);

    QStatInfo& operator=(const QStatInfo& val);

    void store();

    void clean();

    QStatInfo getTotal();
};

#endif // QSTATINFO_H
