#ifndef MSTRINGLISTMODEL_H
#define MSTRINGLISTMODEL_H

/**
  \file mstringlistmodel.h
  \brief Wrapper of QStringListModel for QML.

  \author Musson team

  \date 2011-10-03
  */

#include <QStringListModel>
#include "musson_global.h"

class musson_EXPORT MStringListModel : public QStringListModel
{
    Q_OBJECT
public:
    // Contrary to normal QStringListModel, this class does not
    // allow giving the strings in the model in constructor.
    // The reason is that for this model to work also on
    // MeeGo/Harmattan, the strings must be set after the
    // QML template has been loaded. Otherwise the signal
    // handlers in QML have not been registered. The default
    // role name used to display the roles is "name", which is
    // used both by Symbian and Harmattan Qt Compontents.
    MStringListModel(const QByteArray &displayRoleName = "name", QObject *parent = 0);

    // Overrides QStringListModel's function. Setting this will
    // emit stringAdded(QString) for each string in the list. If
    // there was non empty string list set to this model before,
    // stringsReset() signal is emitted before
    // stringAdded(QString) signals.
    void setStringList(const QStringList &strings);

signals:
    void stringAdded(const QString &newString);
    // Emitted when setStringList is called if already contained strings
    void stringsReset();

public slots:

};

#endif // MSTRINGLISTMODEL_H
