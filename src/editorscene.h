#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QGraphicsScene>

#define GRID_SIZE 10

class EditorScene : public QGraphicsScene
{
public:
    explicit EditorScene(QObject *parent = nullptr);

protected:

    void drawBackground(QPainter *painter, const QRectF &rect);

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

};

#endif // EDITORSCENE_H
