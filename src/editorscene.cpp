#include "editorscene.h"
#include "editorobject.h"

#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QCursor>
#include <QGraphicsView>
#include <QGraphicsItemGroup>

EditorScene::EditorScene(QObject *parent)
    : QGraphicsScene{parent}
{

}

void EditorScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QPen pen(Qt::black);
    painter->setPen(pen);

    qreal left = int(rect.left()) - (int(rect.left()) % GRID_SIZE);
    qreal top = int(rect.top()) - (int(rect.top()) % GRID_SIZE);
    QVector<QPointF> points;
    for (qreal x = left; x < rect.right(); x += GRID_SIZE){
        for (qreal y = top; y < rect.bottom(); y += GRID_SIZE){
            points.append(QPointF(x,y));
        }
    }
    painter->drawPoints(points.data(), points.size());
}

void EditorScene::keyPressEvent(QKeyEvent *event)
{

    if(event->type() == QKeyEvent::KeyPress) {

        if(event->matches(QKeySequence::Copy)) {
            copySelectedItems();
        } else if(event->matches(QKeySequence::Paste)) {
            pasteSelectedItems();
        } else if(event->matches(QKeySequence::Save)) {
            emit saveRequested();
        } else if(event->matches(QKeySequence::Delete)) {
            deleteSelectedItems();
        }
    }

    QGraphicsScene::keyPressEvent(event);
}

void EditorScene::deleteSelectedItems()
{
    auto sel = selectedItems();
    for(auto s : sel) {
        removeItem(s);
        delete s;
    }
}

void EditorScene::copySelectedItems()
{
    auto sel = selectedItems();
    for(auto s : sel) {
        m_copy_items.push_back(s);
    }
}

void EditorScene::pasteSelectedItems()
{
    QList<QGraphicsView*> v = views();
    QPoint origin = v[0]->mapFromGlobal(QCursor::pos());
    QPointF relativeOrigin = v[0]->mapToScene(origin);
    QGraphicsItemGroup *g = new QGraphicsItemGroup();

    for(auto it : m_copy_items) {

        EditorObject *obj = qgraphicsitem_cast<EditorObject *>(it);
        if(obj) {
            EditorObject *copy = new EditorObject(obj->label(), obj->pixmap());
            copy->setPos(obj->pos());
            g->addToGroup(copy);
        }
    }

    clearSelection();
    addItem(g);

    QPointF gpos = QPointF(g->boundingRect().x(),
                           g->boundingRect().y());

    QPointF off = relativeOrigin - gpos;

    g->setPos(off);

    for(auto it : g->childItems()) {
        g->removeFromGroup(it);
        it->setSelected(true);

    }

    destroyItemGroup(g);
}
