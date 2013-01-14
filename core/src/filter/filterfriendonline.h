#ifndef FILTERFRIENDONLINE_H
#define FILTERFRIENDONLINE_H

#include "filter/filter.h"
#include "musson_global.h"

class musson_EXPORT FilterFriendOnline : public Filter
{
    Q_OBJECT
    bool enabled;

public:
    explicit FilterFriendOnline(QObject *parent = 0);
    int filterableType() const;
    bool isFiltered(QVariant value) const;
    QActionGroup* createActions();

private slots:
    void onlineActionToggled(bool state);

};

#endif // FILTERFRIENDONLINE_H
