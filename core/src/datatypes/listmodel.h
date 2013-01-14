#ifndef LISTMODEL_H
#define LISTMODEL_H

/**
  \file listmodel.h
  \brief Album delegate for ListModel.

  Delegate for Album datatype from musson-core library.

  \author Musson team

  \date 2011-10-03
  */

#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include "musson_global.h"

/*!
 \brief An abstract class for item delegates for ListModel.
 */
class musson_EXPORT ListItem: public QObject {
    Q_OBJECT

public:
    ListItem(QObject* parent = 0) : QObject(parent) {}
    virtual ~ListItem() {}
    virtual QString id() const = 0;
    virtual QVariant key() const = 0;
    virtual QVariant data(int role) const = 0;
    virtual QHash<int, QByteArray> roleNames() const = 0;

signals:
    void dataChanged();
};

/*!
 \brief Data model based on QAbstractListModel which is used in QML.
 */
class musson_EXPORT ListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    /*!
     \brief Empty constructor.

     \param parent object
     */
    ListModel(QObject* parent = 0);

    /*!
     \brief Creates an instance of ListModel class based on prototype.

     \param prototype of item delegates for model
     \param parent object
     */
    explicit ListModel(ListItem* prototype, QObject* parent = 0);
    ~ListModel();

    /*!
     \brief Overload of QAbstractListModel::rowCount() method.

     \param parent index
     \return count of rows
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /*!
     \brief Overload of data() method.

     \param index
     \param data role
     \return data depending of role
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /*!
     \brief Append row to model.

     \param item which will be appended
     */
    void appendRow(ListItem* item);

    /*!
     \brief Appends rows to model.

     \param list of items which will be appended
     */
    void appendRows(const QList<ListItem*> &items);

    /*!
     \brief Insert row to model.

     \param index of row behind which row will be inserted
     \param item which will be inserted
     */
    void insertRow(int row, ListItem* item);

    /*!
     \brief Remove row from model.

     \param index of row which row will be removed
     \param parent index
     */
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    /*!
     \brief Overload of QAbstractListModel::removeRows() method.

     \param index of start row
     \param count of rows which will be removed
     \param parent index
     */
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    /*!
     \brief Overload of QAbstractListModel::sort() method.

     \param number of column
     \param sorting order
     */
    void sort(int column, Qt::SortOrder order);

    /*!
     \brief Get item by index of row.

     \param index of row
     \return ListItem
     */
    ListItem* takeRow(int row);

    /*!
     \brief Get item by id.

     \param id of item
     \return ListItem
     */
    ListItem* find(const QString &id) const;

    /*!
     \brief Get index by item.

     \param item
     \return index
     */
    QModelIndex indexFromItem( const ListItem* item) const;

    /*!
     \brief Clear model.
     */
    void clear();

private slots:
    /*!
     \brief Signal. It is emmited when data in item is changed.
     */
    void handleItemChange();

public slots:
    void slotBeginReset();

private:
    ListItem* m_prototype;
    QList<ListItem*> m_list;
};

Q_DECLARE_METATYPE(ListModel*)

#endif // LISTMODEL_H
