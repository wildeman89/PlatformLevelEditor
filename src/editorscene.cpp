#include "editorscene.h"

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

        }
    }

    QGraphicsScene::keyPressEvent(event);
}

void EditorScene::keyReleaseEvent(QKeyEvent *event)
{



    QGraphicsScene::keyReleaseEvent(event);
}
