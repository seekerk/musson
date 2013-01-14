/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain additional
 ** rights.  These rights are described in the Nokia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Nokia at qt-info@nokia.com.
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef QDECLARATIVEFOLDERLISTMODEL_H
#define QDECLARATIVEFOLDERLISTMODEL_H

/**
  \file folderlistmodel.h
  \brief Model for representing list of files or directories.

  It is used for file browser in QML.

  \author Musson team

  \date 2011-10-03
  */

#include <qdeclarative.h>
#include <QStringList>
#include <QString>
#include <QAbstractListModel>

#ifndef QT_NO_DIRMODEL

QT_BEGIN_HEADER

QT_MODULE(Declarative)

class QDeclarativeContext;
class QModelIndex;

class QDeclarativeFolderListModelPrivate;

class QDeclarativeFolderListModel : public QAbstractListModel, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)

    Q_PROPERTY(QString folder READ folder WRITE setFolder NOTIFY folderChanged)
    Q_PROPERTY(QString parentFolder READ parentFolder NOTIFY folderChanged)
    Q_PROPERTY(QStringList nameFilters READ nameFilters WRITE setNameFilters)
    Q_PROPERTY(QStringList selectedItems READ selectedItems NOTIFY selectedChanged)
    Q_PROPERTY(SortField sortField READ sortField WRITE setSortField)
    Q_PROPERTY(bool sortReversed READ sortReversed WRITE setSortReversed)
    Q_PROPERTY(bool showDirs READ showDirs WRITE setShowDirs)
    Q_PROPERTY(bool showFiles READ showFiles WRITE setShowFiles)
    Q_PROPERTY(bool showDotAndDotDot READ showDotAndDotDot WRITE setShowDotAndDotDot)
    Q_PROPERTY(bool showOnlyReadable READ showOnlyReadable WRITE setShowOnlyReadable)
    Q_PROPERTY(int count READ count)

public:
    QDeclarativeFolderListModel(QObject *parent = 0);
    ~QDeclarativeFolderListModel();

    enum Roles { FileNameRole = Qt::UserRole+1, FilePathRole = Qt::UserRole+2 };

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    int count() const { return rowCount(QModelIndex()); }

    QString folder() const;
    void setFolder(const QString &folder);

    QString parentFolder() const;

    QStringList nameFilters() const;
    void setNameFilters(const QStringList &filters);

    QStringList selectedItems() const;

    enum SortField { Unsorted, Name, Time, Size, Type };
    SortField sortField() const;
    void setSortField(SortField field);
    Q_ENUMS(SortField)

    bool sortReversed() const;
    void setSortReversed(bool rev);

    bool showDirs() const;
    void setShowDirs(bool);
    bool showFiles() const;
    void setShowFiles(bool);
    bool showDotAndDotDot() const;
    void setShowDotAndDotDot(bool);
    bool showOnlyReadable() const;
    void setShowOnlyReadable(bool);

    Q_INVOKABLE bool isFolder(int index) const;

    virtual void classBegin();
    virtual void componentComplete();

Q_SIGNALS:
    void folderChanged();
    void selectedChanged();

private Q_SLOTS:
    void inserted(const QModelIndex &index, int start, int end);
    void removed(const QModelIndex &index, int start, int end);
    void dataChanged(const QModelIndex &start, const QModelIndex &end);

public Q_SLOTS:
    void refresh();
    void setSelected(QString file, bool isSelected);
    void resetSelected();
    bool isSelected(QString file);

private:
    Q_DISABLE_COPY(QDeclarativeFolderListModel)
    QDeclarativeFolderListModelPrivate *d;
    QStringList m_selectedFiles;
};


QML_DECLARE_TYPE(QDeclarativeFolderListModel)

QT_END_HEADER

#endif // QT_NO_DIRMODEL

#endif // QDECLARATIVEFOLDERLISTMODEL_H
