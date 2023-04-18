#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QGraphicsView>

class EditorView : public QGraphicsView
{
    Q_OBJECT

public:
    EditorView(QWidget *parent = nullptr);

protected:

    void dragEnterEvent(QDragEnterEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void dropEvent(QDropEvent *event);

signals:

    void itemDropped(const QString &name, const QPointF &pos);
};

#endif // EDITORVIEW_H
