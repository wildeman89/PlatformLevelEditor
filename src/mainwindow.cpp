#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newlevelform.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // load config files
    loadConfigBackground();

    connect(ui->actionNew_Level, &QAction::triggered, this, &MainWindow::newLevelSelected);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quitSelected);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadConfigBackground()
{
    QFile file(":/res/config/backgrounds.json");
    if(!file.open(QFile::ReadOnly)) {
        throw std::invalid_argument("missing resource: /res/config/backgrounds.json");
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if(!obj.contains("backgrounds")) {
        throw std::invalid_argument("json error: /res/config/backgrounds.json - missing field: backgrounds");
    }

    if(!obj["backgrounds"].isArray()) {
        throw std::invalid_argument("json error: /res/config/backgrounds.json - field: backgrounds must be array");
    }

    QJsonArray array = obj["backgrounds"].toArray();

    for(const QJsonValue &val : array) {
        if(!val.isString()) {
            throw std::invalid_argument("json error: /res/config/backgrounds.json - field: backgrounds must contain strings");
        }
        m_backgrounds.push_back(val.toString());
    }
}

void MainWindow::quitSelected()
{
    QApplication::quit();
}

void MainWindow::newLevelSelected()
{
    NewLevelForm *form = new NewLevelForm(m_backgrounds);
    connect(form, &NewLevelForm::finished, this, &MainWindow::newLevelFormFinished);
    form->show();
}

void MainWindow::newLevelFormFinished(const QString &name,
                                      const QSizeF &size,
                                      const QString &background)
{


}

