#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quitEditor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quitEditor()
{
    QApplication::quit();
}

