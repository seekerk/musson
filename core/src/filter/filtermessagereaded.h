#ifndef FILTERMESSAGEREADED_H
#define FILTERMESSAGEREADED_H

#include "filter/filter.h"
#include "musson_global.h"

struct MessageIDs
{
    QString accountId;
    QString messageId;
};

class musson_EXPORT FilterMessageReaded : public Filter
{
    Q_OBJECT
    bool enabled;
public:
    explicit FilterMessageReaded(QObject *parent = 0);
    int filterableType() const;
    bool isFiltered(QVariant value) const;
    QActionGroup* createActions();

private:
    mutable QList<MessageIDs> notReaded;

private slots:
    void actionToggled(bool state);
};

#endif // FILTERMESSAGEREADED_H
