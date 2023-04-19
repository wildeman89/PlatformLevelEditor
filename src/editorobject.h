#ifndef EDITOROBJECT_H
#define EDITOROBJECT_H

#include <QGraphicsItem>

class EditorObject : public QGraphicsItem
{
    QString m_label;
    QPixmap *m_pixmap;

public:
    EditorObject(const QString &label, QPixmap *pixmap);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

    QString label() const;

protected:

    QVariant itemChange(GraphicsItemChange change,
                            const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // EDITOROBJECT_H