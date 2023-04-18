#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QGraphicsScene>

#define GRID_SIZE 16

class EditorScene : public QGraphicsScene
{
public:
    explicit EditorScene(QObject *parent = nullptr);

    void drawBackground(QPainter *painter, const QRectF &rect) override;
};

#endif // EDITORSCENE_H
