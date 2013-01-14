#include "iconitem.h"
#include <QPainter>
#include <QStyleOption>

IconItem::IconItem(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

void IconItem::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->save();
    if (!m_icon.isNull())
        painter->drawPixmap(option->rect, m_icon.pixmap(this->width(), this->height()));
    painter->restore();
}

void IconItem::setIcon(const QIcon &icon)
{
    m_icon = icon;
    emit iconChanged();
    update();
}

const QIcon& IconItem::icon() const
{
    return m_icon;
}
