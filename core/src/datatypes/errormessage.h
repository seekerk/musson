#ifndef ERRORMESSAGE_MSA_H
#define ERRORMESSAGE_MSA_H

/**
  \file errormessage.h
  \author Kulakov Kirill
  \date 2011-03-30
  */

#include <QObject>
#include <QMetaType>
#include <QSharedData>
#include <QVariant>
#include "musson_global.h"
/**
  \brief This error caused when send() function generates an exception.
  */
#define APP_LIBRARY_EXCEPTION_CODE "001"
#define APP_LIBRARY_WRONG_RESPONSE "002"
#define APP_FILE_READ_ERROR "003"
#define APP_FUNCTION_WRONG_ARGS "004"

class ErrorMessage;
class ErrorMessagePrivate;

namespace xmlrpc {
    class Client;
}

/**
  \brief Error message class.

  This class describes error message: code, text and comment.
  */
class musson_EXPORT ErrorMessage : public QObject
{
    Q_OBJECT

    QSharedDataPointer<ErrorMessagePrivate> d;
public:
    /**
      \brief The default constructor.

      */
    ErrorMessage();

    /**
      \brief The copy constructor.
      */
    ErrorMessage(const ErrorMessage&);

    /**
      \brief The constructor with inistial values of error code and message.

      \param className The name of class where error caused.

      \param code The name of function which produce error.

      \param code The error code.

      \param text The error text.
      */
    ErrorMessage(QString className, QString function, QString code, QString text);

    ~ErrorMessage();

    /*!
     \brief

     \fn operator =
     \param
    */
    ErrorMessage& operator=(const ErrorMessage&);

    /**
      \brief The error code.
      */
    QString code() const;

    void setCode(const QString& v);

    /**
      \brief The error title specified by user.
      */
    QString title() const;

    void setTitle(const QString& v);

    /**
      \brief The error text.
      */
    QString text() const;

    void setText(const QString& v);

    /**
      \brief additional comments corresponded this error.
      */
    QString comment() const;

    void setComment(const QString& v);

    /**
      \brief The name of API class whitch produce error.
      */
    QString className() const;

    void setClassName(const QString& v);

    /**
      \brief The name of module whitch produce error.
      */
    QString moduleName() const;

    void setModuleName(const QString& v);

    /**
      \brief The name of function whitch produce error.
      */
    QString functionName() const;

    void setFunctionName(const QString& v);

    /**
      \brief The request content which produce error.
      */
    QString request() const;

    void setRequest(const QString& v);

    /**
      \brief The exact response content.
      */
    QString response() const;

    void setResponse(const QString& v);

    /**
      \brief The e-mail of error author.
      */
    QString authorMail() const;

    void setAuthorMail(const QString& v);

    /**
      \brief User message.
      */
    QString authorMessage() const;

    void setAuthorMessage(const QString& v);

    /**
      \brief The source code line assigned to the bug.
      */
    QString bugLine() const;

    void setBugLine(const QString& v);

    /**
      \brief The Id of bug in Bugzilla or 0.
      */
    int bugId() const;

    /**
      \brief Creates error title based on error content.
      */
    QString bugTitle() const;

    /**
      \brief Creates error body based on error content.
      */
    QString bugBody(bool useUserData) const;

    /**
      \brief Sends bug to project Bugzilla.
      */
    Q_INVOKABLE void sendBug();

signals:
    /**
      \brief Sends error message if \a sendBug() was failed.
      */
    void error(QString msgText, ErrorMessage msg);

    void success(bool isCreated, ErrorMessage msg);

private slots:
    void gotResponse(int reqId, QVariant val);
    void error(int, int, QString msg);

private:
    // the status of bug post process
    QString status;

    // the xmlrpc client
    xmlrpc::Client *client;
};

Q_DECLARE_METATYPE(ErrorMessage)

#endif // QERRORMSG_MSA_H
