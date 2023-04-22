#include "editorview.h"
#include "editorobject.h"

#include <QMouseEvent>
#include <QMimeData>
#include <QStandardItemModel>
#include <QKeyEvent>


EditorView::EditorView(QWidget *parent)
    : QGraphicsView{parent},
      m_is_selecting(false),
      m_is_shift(false)
{
    setAcceptDrops(true);

    m_selection_rect = new QGraphicsRectItem(0,0,0,0);
    QBrush brush(QColor(0,0,255, 64));
    m_selection_rect->setBrush(brush);
    m_selection_rect->setZValue(1000);
}

EditorView::~EditorView()
{
    delete m_selection_rect;
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
            emit itemDropped(index.data().toString(), mapToScene(event->position().toPoint()));
        }
    }
}

void EditorView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        auto event_pos = mapToScene(event->position().toPoint());
        m_selection_rect->setPos(event_pos);
        scene()->addItem(m_selection_rect);
        m_is_selecting = true;
        event->accept();
    }

    QGraphicsView::mousePressEvent(event);
}

void EditorView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_is_selecting) {
        auto event_pos = mapToScene(event->position().toPoint());
        QPointF p2 = event_pos - m_selection_rect->pos();
        QRectF new_rect(0,0,
                        p2.x(),
                        p2.y());

        m_selection_rect->setRect(new_rect.normalized());
        scene()->update();
        event->accept();
    }

    QGraphicsView::mouseMoveEvent(event);
}

void EditorView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        if(m_is_selecting) {

            auto col = scene()->collidingItems(m_selection_rect);
            for(auto it : col) {
                if(it->type() == EditorObject::Type) {
                    it->setSelected(true);
                }
            }

            m_selection_rect->setRect(QRectF());
            scene()->removeItem(m_selection_rect);
            scene()->update();
            event->accept();
            m_is_selecting = false;
        }
    }

    QGraphicsView::mouseReleaseEvent(event);
}
