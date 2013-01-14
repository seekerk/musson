#ifndef ICONITEM_H
#define ICONITEM_H

/**
  \file iconitem.h
  \brief Icon delegate for ListModel.

  Delegate for QIcon class.

  \author Musson team

  \date 2011-10-03
  */

#include <QDeclarativeItem>
#include "musson_global.h"

/*!
 \brief A delegate of QIcon for QML.
 */
class musson_EXPORT IconItem : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon NOTIFY iconChanged)
public:
    /*!
     \brief Empty constructor.

     \param parent object
     */
    explicit IconItem(QDeclarativeItem *parent = 0);

    /*!
     \brief Overload of paint method.

     \param painter
     \param option
     \param parent widget
     */
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

    /*!
     \brief Get path to icon.

     \return path to icon
     */
    const QIcon &icon() const;

    /*!
     \brief Set path to icon.

     \param path to icon
     */
    void setIcon(const QIcon &icon);

signals:
    /*!
     \brief Signal. Emits when path to icon is changed.
     */
    void iconChanged();
public slots:

private:
    QIcon m_icon;

};

#endif // ICONITEM_H
