#include "editorscene.h"
#include "editorobjectgroup.h"

#include <QPainter>
#include <QDebug>
#include <QKeyEvent>

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
        } else if(event->matches(QKeySequence::Cut)) {

        } else if(event->matches(QKeySequence::Paste)) {

        } else if(event->matches(QKeySequence::Save)) {

        } else if(event->matches(QKeySequence::Delete)) {

            // delete all selected items
            auto sel = selectedItems();
            for(auto s : sel) {
                removeItem(s);
            }

        } else if(event->key() == Qt::Key_G && event->modifiers()==Qt::ControlModifier) {

            // group selected items if there are more than 1
            auto sel = selectedItems();
            if(sel.size() > 1) {
                EditorObjectGroup *group = new EditorObjectGroup();
                for(auto s : sel) {
                    group->addToGroup(s);
                }
                addItem(group);
                clearSelection();
                group->setSelected(true);
                update();
            }



        } else if(event->key() == Qt::Key_U && event->modifiers()==Qt::ControlModifier) {

            // ungroup selected items if they are groups

            auto sel = selectedItems();
            for(auto s : sel) {

                EditorObjectGroup *group = qgraphicsitem_cast<EditorObjectGroup *>(s);
                if(!group) {
                    continue;
                }

                for(auto g : group->childItems()) {
                    group->removeFromGroup(g);
                    g->setSelected(true);
                }

                destroyItemGroup(group);
            }

            update();
        }
    }

    QGraphicsScene::keyPressEvent(event);
}

void EditorScene::keyReleaseEvent(QKeyEvent *event)
{



    QGraphicsScene::keyReleaseEvent(event);
}
