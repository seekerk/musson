#ifndef FILTERFRIENDGENDER_H
#define FILTERFRIENDGENDER_H

#include "filter/filter.h"
#include "musson_global.h"

class musson_EXPORT FilterFriendGender : public Filter
{
    Q_OBJECT

    bool maleFiltered;
    bool femaleFiltered;
public:
    explicit FilterFriendGender(QObject *parent = 0);
    int filterableType() const;
    bool isFiltered(QVariant value) const;
    QActionGroup* createActions();

private slots:
    void maleFilterChanged(bool);
    void femaleFilterChanged(bool);

};

#endif // FILTERFRIENDGENDER_H
