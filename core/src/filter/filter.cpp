#include "filter.h"

Filter::Filter(QObject *parent) :
    QObject(parent)
{
}

void Filter::emitStateChanged()
{
    emit stateChanged(filterableType());
}
