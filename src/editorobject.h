#ifndef EDITOROBJECT_H
#define EDITOROBJECT_H

#include <QGraphicsObject>

class EditorObject : public QGraphicsObject
{
    Q_OBJECT

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

signals:

    void deleteObject(QGraphicsObject *obj);
};

#endif // EDITOROBJECT_H
