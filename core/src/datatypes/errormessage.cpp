#include "errormessage.h"
#include "qxmlrpc/client.h"

class ErrorMessagePrivate : public QSharedData
{
public:
    ErrorMessagePrivate();
    ErrorMessagePrivate(const ErrorMessagePrivate& other);
    ~ErrorMessagePrivate();

    QString code;
    QString text;
    QString title;
    QString request;
    QString response;
    QString functionName;
    QString moduleName;
    QString className;
    QString comment;
    QString authorMail;
    QString authorMessage;
    QString bugLine;

    int bugId;

};

ErrorMessagePrivate::ErrorMessagePrivate()
    :QSharedData()
{
}

ErrorMessagePrivate::ErrorMessagePrivate(const ErrorMessagePrivate &other)
    : QSharedData(other)
{
    this->code = other.code;
    this->text = other.text;
    this->title = other.title;
    this->request = other.request;
    this->response = other.response;
    this->functionName = other.functionName;
    this->moduleName = other.moduleName;
    this->className = other.className;
    this->bugId = other.bugId;
    this->comment = other.comment;
    this->authorMail = other.authorMail;
    this->authorMessage = other.authorMessage;
    this->bugLine = other.bugLine;
}

ErrorMessagePrivate::~ErrorMessagePrivate()
{
}

ErrorMessage::ErrorMessage()
    :QObject(0), client(0)
{
    d = new ErrorMessagePrivate;
}

ErrorMessage::ErrorMessage(QString className, QString function, QString code, QString text)
    :QObject(0), client(0)
{
    d = new ErrorMessagePrivate;
    d->className = className;
    d->functionName = function;
    d->code = code;
    d->text = text;
}

ErrorMessage::ErrorMessage(const ErrorMessage & src)
    :QObject(src.parent()), d(src.d)
{
    this->client = src.client;
}

ErrorMessage::~ErrorMessage()
{
}

ErrorMessage& ErrorMessage::operator =(const ErrorMessage &src)
{
    d = src.d;
    return *this;
}

QString ErrorMessage::bugTitle() const
{
    QString ret(d->moduleName);

    if (!d->className.isEmpty())
    {
        if (!ret.isEmpty())
            ret.append(":");
        ret.append(d->className);
    }

    if (!d->functionName.isEmpty())
    {
        if (!ret.isEmpty())
            ret.append(":");
        ret.append(d->functionName);
    }

    if (!d->code.isEmpty())
    {
        if (!ret.isEmpty())
            ret.append(":");
        ret.append(d->code);
    }

    if (!d->title.isEmpty())
    {
        if (!ret.isEmpty())
            ret.append(":");
        ret.append(d->title);
    }

    if (ret.isEmpty())
        ret = "new bug";

    return ret;
}

QString ErrorMessage::bugBody(bool useUserData) const
{
    QString ret;
    if (useUserData)
    { // this data send to bugzilla
        if (!d->authorMail.isEmpty())
        {
            ret.append("\n\nReporter:\n");
            ret.append(d->authorMail);
        }

        if (!d->authorMessage.isEmpty())
        {
            ret.append("\n\nReporter comment:\n");
            ret.append(d->authorMessage);
        }

        // add platform type if it known
    #ifdef Q_WS_MAEMO_5
        ret.append("\n\nPlatform:\nMaemo 5");
    #endif

    #ifdef MEEGO_EDITION_HARMATTAN
        ret.append("\n\nPlatform:\nHarmattan");
    #endif

        if (!d->bugLine.isEmpty())
        {
            ret.append("\n\nBug line:\n");
            ret.append(d->bugLine);
        }

    } else {
        // this data shows to user
        ret.append(d->text);

        QString notice(tr(" (please replace private data)"));

        if (!d->request.isEmpty())
        {
            ret.append(tr("\n\nRequest%1:\n").arg(notice));
            ret.append(d->request);
        }

        if (!d->response.isEmpty())
        {
            ret.append(tr("\n\nResponse%1:\n").arg(notice));
            ret.append(d->response);
        }

        if (!d->comment.isEmpty())
        {
            ret.append(tr("\n\nComments%1:\n").arg(notice));
            ret.append(d->comment);
        }
    }

    return ret;
}

void ErrorMessage::sendBug()
{
    if (this->client == NULL)
        this->client = new xmlrpc::Client(this);
    this->client->setHost("oss.fruct.org", 80, "/bugzilla/xmlrpc.cgi");

    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << this->client;
    connect (this->client, SIGNAL(failed(int,int,QString)), this, SLOT(error(int,int,QString)));
    connect (this->client, SIGNAL(done(int,QVariant)), this, SLOT(gotResponse(int, QVariant)));

    QMap <QString, xmlrpc::Variant>loginList;
    loginList["login"] = xmlrpc::Variant(QString("scribo.bugs@oss.fruct.org"));
    loginList["password"] = xmlrpc::Variant(QString("bugsforfree"));

    xmlrpc::Variant param(loginList);

    this->status = "login";
    this->client->request("User.login", param);
}

void ErrorMessage::gotResponse(int reqId, QVariant val)
{
    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "response type=" << val.typeName() << "value=" << val;

    // login -> search bug
    if (this->status.compare("login") == 0)
    {
        bool isError = false;
        // check response
        if (val.canConvert<QVariantMap>())
        {
            QVariantMap ret = val.toMap();
            if (!ret.contains("id"))
                isError = true;
        } else
            isError = true;

        if (isError)
        {
            error(reqId, 0, "Wrong response from server");
            return;
        }

        qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__;
        QMap<QString, xmlrpc::Variant> params;
        params["product"] = xmlrpc::Variant(QString("TestProduct"));
        params["summary"] = xmlrpc::Variant(this->bugTitle());

        xmlrpc::Variant prm(params);
        this->status = "search";

        this->client->request("Bug.search", prm);
        return;
    }

    if (this->status.compare("search") == 0)
    {
        bool isError = false;

        bool isExists = false;

        QVariantList bugs;

        // check response
        if (val.canConvert<QVariantMap>())
        {
            QVariantMap ret = val.toMap();
            if (!ret.contains("bugs"))
                isError = true;
            else {
                bugs = ret.value("bugs").toList();
                if (bugs.length() > 0)
                    isExists = true;
            }
        } else
            isError = true;

        if (isError)
        {
            error(reqId, 0, "Wrong response from server");
            return;
        }

        // create new bug if it not exists
        if (!isExists)
        {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__;
            QMap<QString, xmlrpc::Variant> params;
            params["product"] = xmlrpc::Variant(QString("TestProduct"));
            params["component"] = xmlrpc::Variant(QString("TestComponent"));
            params["version"] = xmlrpc::Variant(QString("unspecified"));
            params["summary"] = xmlrpc::Variant(this->bugTitle());
            params["description"] = xmlrpc::Variant(this->bugBody(true));

            xmlrpc::Variant prm(params);
            this->status = "add";

            this->client->request("Bug.create", prm);
            return;
        }else{
            QVariantMap bugVals = bugs.first().toMap();
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "bug exists:" << bugVals;
            d->bugId = bugVals.value("id").toInt();
            QString bugStatus = bugVals.value("status").toString();

            if (bugStatus.compare("RESOLVED") != 0)
            {
                // add comment of existing bug
                QMap<QString, xmlrpc::Variant> params;
                params["id"] = xmlrpc::Variant(d->bugId);
                params["comment"] = xmlrpc::Variant(this->bugBody(true));

                xmlrpc::Variant prm(params);
                this->status = "addComment";

                this->client->request("Bug.add_comment", prm);
                return;
            } else {
                emit this->success(false, *this);
                this->deleteLater();
                return;
            }
        }
    }

    if (this->status.compare("add") == 0)
    {
        bool isError = false;

        // check response
        if (val.canConvert<QVariantMap>())
        {
            QVariantMap ret = val.toMap();
            if (!ret.contains("id"))
                isError = true;
            d->bugId = ret.value("id").toInt();
            emit this->success(true, *this);
            this->deleteLater();
            return;
        } else
            isError = true;

        if (isError)
        {
            error(reqId, 0, "Wrong response from server");
            return;
        }

    }

    if (this->status.compare("addComment") == 0)
    {
        bool isError = false;

        // check response
        if (val.canConvert<QVariantMap>())
        {
            QVariantMap ret = val.toMap();
            if (!ret.contains("id"))
                isError = true;
            emit this->success(false, *this);
            this->deleteLater();
            return;
        } else
            isError = true;

        if (isError)
        {
            error(reqId, 0, "Wrong response from server");
            return;
        }

    }
}

void ErrorMessage::error(int, int, QString msg)
{
    qDebug() << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << msg;
    emit error(msg, *this);
    this->deleteLater();

}

// setters
void ErrorMessage::setAuthorMail(const QString &v){d->authorMail = v;}
void ErrorMessage::setAuthorMessage(const QString &v) {d->authorMessage = v;};
void ErrorMessage::setClassName(const QString &v){d->className = v;}
void ErrorMessage::setCode(const QString &v){d->code = v;}
void ErrorMessage::setComment(const QString &v){d->comment = v;}
void ErrorMessage::setFunctionName(const QString &v){d->functionName = v;}
void ErrorMessage::setModuleName(const QString &v){d->moduleName = v;}
void ErrorMessage::setRequest(const QString &v){d->request = v;}
void ErrorMessage::setResponse(const QString &v){d->response = v;}
void ErrorMessage::setText(const QString &v){d->text = v;}
void ErrorMessage::setTitle(const QString &v){d->title = v;}
void ErrorMessage::setBugLine(const QString &v){d->bugLine = v;}

// getters
QString ErrorMessage::authorMail() const {return d->authorMail;}
QString ErrorMessage::authorMessage() const {return d->authorMessage;}
QString ErrorMessage::className() const {return d->className;}
QString ErrorMessage::code() const {return d->code;}
QString ErrorMessage::comment() const {return d->comment;}
QString ErrorMessage::functionName() const {return d->functionName;}
QString ErrorMessage::moduleName() const {return d->moduleName;}
QString ErrorMessage::request() const {return d->request;}
QString ErrorMessage::response() const {return d->response;}
QString ErrorMessage::text() const {return d->text;}
QString ErrorMessage::title() const {return d->title;}
QString ErrorMessage::bugLine() const {return d->bugLine;}
int ErrorMessage::bugId() const {return d->bugId;}
