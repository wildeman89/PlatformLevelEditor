#include "editorobject.h"

#include <QPainter>


EditorObject::EditorObject(const QString &label, QPixmap *pixmap)
    : m_label(label), m_pixmap(pixmap)
{
    setFlag(ItemIsMovable, true);
}

QRectF EditorObject::boundingRect() const
{
    return m_pixmap->rect();
}

void EditorObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->drawPixmap(boundingRect(), *m_pixmap, boundingRect());
}
