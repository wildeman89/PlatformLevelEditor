#include "editorobject.h"

#include <QPainter>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

#define GRID_SIZE 10

QString EditorObject::label() const
{
    return m_label;
}

int EditorObject::type() const
{
    return Type;
}

EditorObject::EditorObject(const QString &label, QPixmap *pixmap)
    : m_label(label), m_pixmap(pixmap)
{
    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);}

QRectF EditorObject::boundingRect() const
{
    return m_pixmap->rect();
}

void EditorObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->drawPixmap(boundingRect(), *m_pixmap, boundingRect());

    if(isSelected()) {
        QPen pen(Qt::blue);
        pen.setWidth(4);
        painter->setPen(pen);
        painter->drawRect(QRectF(2,2, boundingRect().width() - 4, boundingRect().height() - 4));
    }
}

QVariant EditorObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        if(QApplication::mouseButtons() == Qt::LeftButton ){
            qreal xV = round(newPos.x()/GRID_SIZE)*GRID_SIZE;
            qreal yV = round(newPos.y()/GRID_SIZE)*GRID_SIZE;
            return QPointF(xV, yV);
        }
        else {
            return newPos;
        }
    }
    else {
        return QGraphicsItem::itemChange(change, value);
    }
}
