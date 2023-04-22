#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QGraphicsView>

class EditorView : public QGraphicsView
{
    Q_OBJECT

    QGraphicsRectItem *m_selection_rect;

    bool m_is_selecting;
    bool m_is_shift;

public:
    EditorView(QWidget *parent = nullptr);
    ~EditorView();

protected:

    void dragEnterEvent(QDragEnterEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void dropEvent(QDropEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:

    void itemDropped(const QString &name, const QPointF &pos);
};

#endif // EDITORVIEW_H
