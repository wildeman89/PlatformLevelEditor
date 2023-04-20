#ifndef EDITOROBJECTGROUP_H
#define EDITOROBJECTGROUP_H

#include <QGraphicsItemGroup>

class EditorObjectGroup : public QGraphicsItemGroup
{
public:
    enum { Type = UserType + 2 };

    EditorObjectGroup();

    int type() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

protected:

    QVariant itemChange(GraphicsItemChange change,
                            const QVariant &value);
};

#endif // EDITOROBJECTGROUP_H
