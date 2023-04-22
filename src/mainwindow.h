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
    QFileIconProvider m_icon_provider;
    std::unordered_map<QString, QPixmap *> m_pixmaps;
    QString m_file_save_path;

    void loadConfigBackground();
    void initTreeFromPath(const QString &path, QTreeWidgetItem *parent);
    void disableEditor();
    void enableEditor();
    bool isEditing();
    bool isDirty();
    void askIfSave();

signals:

public slots:

    void initTree();

    void quitSelected();
    void newLevelSelected();
    void saveSelected();
    void saveAsSelected();
    void loadSelected();
    void closeSelected();

    void createLevel(const QString &name,
                     const QSizeF &size,
                     const QString &background);

    void addItem(const QString &name, const QPointF &pos);

};
#endif // MAINWINDOW_H
