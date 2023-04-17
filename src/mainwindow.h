#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

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

    std::vector<QString> m_backgrounds;

    void loadConfigBackground();

signals:

public slots:

    void quitSelected();
    void newLevelSelected();
    void newLevelFormFinished(const QString &name,
                              const QSizeF &size,
                              const QString &background);


};
#endif // MAINWINDOW_H
