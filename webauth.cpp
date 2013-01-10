#include <QDebug>
#include "webauth.h"

WebAuth::WebAuth()
{
    QObject::connect(this, SIGNAL(urlChanged(QUrl)), SLOT(isAuthComplete(QUrl)));
}

void WebAuth::setSuccessRegExp(QRegExp exp)
{
    this->successRegExp = exp;
}

void WebAuth::setErrorRegExp(QRegExp exp)
{
    this->errorRegExp = exp;
}

void WebAuth::isAuthComplete(QUrl url)
{
    if (this->successRegExp.exactMatch(url.toString()))
    {
        url = url.toString().replace("#","?");
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "url=" << url.toString();
        emit success(url);
        close();
        return;
    }

    if (this->errorRegExp.exactMatch(url.toString()))
    {
        url = url.toString().replace("#","?");
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "url=" << url.toString();
        emit error(url.queryItemValue("error_description"));
        close();
        return;
    }
}
