#ifndef WEBAUTH_H
#define WEBAUTH_H

#include <QWebView>
#include <QObject>

class WebAuth : public QWebView
{
    Q_OBJECT

private:
    QRegExp successRegExp;
    QRegExp errorRegExp;

public:
    WebAuth();
    void setSuccessRegExp(QRegExp exp);
    void setErrorRegExp(QRegExp exp);

public slots:
    void isAuthComplete(QUrl url);

signals:
    void error(QString msg);
    void success(QUrl url);
};

#endif // WEBAUTH_H
