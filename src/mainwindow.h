#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
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

private:
    Ui::MainWindow *ui;

    QGraphicsScene *m_scene;

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

    void initTreeCategories();
    void loadConfigBackground();
    void loadConfigObjects();

    void disableEditor();
    void enableEditor();
    bool isEditing();

signals:

public slots:

    void quitSelected();
    void newLevelSelected();
    void newLevelFormFinished(const QString &name,
                              const QSizeF &size,
                              const QString &background);


};
#endif // MAINWINDOW_H
