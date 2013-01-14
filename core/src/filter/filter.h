//! \file filter.h
//! \author Kirill Ivashov
//! \brief Base class for Filter implementations

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QList>
#include "musson_global.h"

class QActionGroup;

//! Base class for Filter implementations. To implement filter
//! inherit this class and override three virtual methods
class musson_EXPORT Filter : public QObject
{
    Q_OBJECT
public:
    //! Defauld constructor
    //! \param parent Should be FilterManager
    //! \todo Reparent Filter to FilterManager when registering filter
    explicit Filter(QObject *parent = 0);

    //! Get type that filter supports
    //! Should use function qMetaTypeId() to get type identifier
    //! \sa qMetaTypeId()
    //! return Qt type identifier
    virtual int filterableType() const = 0;

    //! Check is value in QVariant filtered
    //! \param value QVariant containing value
    virtual bool isFiltered(QVariant value) const = 0;

    //! Return QActionGroup that specifies user interface to filter.
    //! This function should be used by FilterManager
    virtual QActionGroup* createActions() = 0;

protected:
    //! Wrapper for signal stateChanged()
    void emitStateChanged();

signals:
    void stateChanged(int type = -1);
};

#endif // FILTER_H
