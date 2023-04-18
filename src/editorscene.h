#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QGraphicsScene>

class EditorScene : public QGraphicsScene
{
public:
    explicit EditorScene(QObject *parent = nullptr);
};

#endif // EDITORSCENE_H
