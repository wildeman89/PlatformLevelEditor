#include "editorobjectgroup.h"

#include <QPainter>
#include <QPen>
#include <QApplication>

#define GRID_SIZE 10

EditorObjectGroup::EditorObjectGroup()
{
    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
    setZValue(100);
}

int EditorObjectGroup::type() const
{
    return Type;
}

QRectF EditorObjectGroup::boundingRect() const
{
    return childrenBoundingRect();
}

void EditorObjectGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if(isSelected()) {
        QPen pen(Qt::blue);
        pen.setWidth(4);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
    }
}

QVariant EditorObjectGroup::itemChange(GraphicsItemChange change, const QVariant &value)
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
