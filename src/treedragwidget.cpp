#include "treedragwidget.h"

#include <QMimeData>
#include <QMouseEvent>

TreeDragWidget::TreeDragWidget(QWidget *parent)
    : QTreeWidget{parent}
{
    setDragEnabled(true);
}


