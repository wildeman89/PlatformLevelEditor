#include "editorscene.h"

#include <QPainter>

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
