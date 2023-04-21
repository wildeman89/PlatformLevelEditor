#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QGraphicsScene>

#define GRID_SIZE 10

class EditorScene : public QGraphicsScene
{
    Q_OBJECT

    std::vector<QGraphicsItem *> m_copy_items;

public:
    explicit EditorScene(QObject *parent = nullptr);

protected:

    void drawBackground(QPainter *painter, const QRectF &rect);

    void keyPressEvent(QKeyEvent *event);

public slots:

    void deleteSelectedItems();
    void copySelectedItems();
    void pasteSelectedItems();

signals:

    void saveRequested();

};

#endif // EDITORSCENE_H
