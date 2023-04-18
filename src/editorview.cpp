#include "editorview.h"

#include <QMouseEvent>
#include <QMimeData>
#include <QStandardItemModel>

EditorView::EditorView(QWidget *parent)
    : QGraphicsView{parent}
{
    setAcceptDrops(true);
}

void EditorView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void EditorView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void EditorView::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();

    if(event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        QStandardItemModel dmodel;
        if(dmodel.dropMimeData(event->mimeData(), event->dropAction(), 0, 0, QModelIndex())) {
            QModelIndex index = dmodel.index(0,0);


            qDebug() << index.data().toString();
            qDebug() << mapToScene(event->position().toPoint());
        }
    }
}
