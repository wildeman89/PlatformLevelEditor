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

protected:

    QVariant itemChange(GraphicsItemChange change,
                            const QVariant &value);
};

#endif // EDITOROBJECT_H
