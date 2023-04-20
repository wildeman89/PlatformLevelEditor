#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QGraphicsScene>

#define GRID_SIZE 10

class EditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit EditorScene(QObject *parent = nullptr);

protected:

    void drawBackground(QPainter *painter, const QRectF &rect);

    void keyPressEvent(QKeyEvent *event);

public slots:

    void deleteSelectedItems();
    void groupSelectedItems();
    void ungroupSelectedItems();

signals:

    void saveRequested();

};

#endif // EDITORSCENE_H
