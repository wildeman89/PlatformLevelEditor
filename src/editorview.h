#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QGraphicsView>

class EditorView : public QGraphicsView
{
public:
    EditorView(QWidget *parent = nullptr);

protected:

    void dragEnterEvent(QDragEnterEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void dropEvent(QDropEvent *event);
};

#endif // EDITORVIEW_H
