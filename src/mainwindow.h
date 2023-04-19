#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editorscene.h"
#include "editorobject.h"

#include <vector>
#include <unordered_map>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTreeWidget>
#include <QFileIconProvider>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void writeLevelFile(const QString &path);
    bool readLevelFile(const QString &path);
    void clearLevel();

private:
    Ui::MainWindow *ui;

    EditorScene *m_scene;

    QString m_level_name;
    QSizeF m_level_size;
    QString m_level_background;

    std::vector<QString> m_backgrounds;
    bool m_editing_enabled;

    // tree categories
    QTreeWidgetItem *m_tree_begin_end;
    QTreeWidgetItem *m_tree_platforms;
    QTreeWidgetItem *m_tree_enemies;
    QTreeWidgetItem *m_tree_powerups;

    // default icons
    QFileIconProvider m_icon_provider;

    // pixmaps
    std::unordered_map<QString, QPixmap *> m_pixmaps;

    // object categories
    std::unordered_map<QString, std::string> m_categories;

    // objects
    std::vector<EditorObject *> m_objects;

    QString m_file_save_path;

    void initTreeCategories();
    void loadConfigBackground();
    void loadConfigObjects();

    void disableEditor();
    void enableEditor();
    bool isEditing();
    void askIfSave();

signals:

public slots:

    void quitSelected();
    void newLevelSelected();
    void saveSelected();
    void saveAsSelected();
    void loadSelected();


    void newLevelFormFinished(const QString &name,
                              const QSizeF &size,
                              const QString &background);

    void itemDropped(const QString &name, const QPointF &pos);

    void deleteObject(QGraphicsObject *obj);

};
#endif // MAINWINDOW_H
