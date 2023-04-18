#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newlevelform.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define BORDER 256

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      m_scene(nullptr),
      m_editing_enabled(true)
{
    ui->setupUi(this);

    m_scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_scene);

    initTreeCategories();
    disableEditor();

    // load config files
    loadConfigBackground();

    connect(ui->actionNew_Level, &QAction::triggered, this, &MainWindow::newLevelSelected);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quitSelected);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTreeCategories()
{
    m_tree_level = new QTreeWidgetItem(ui->treeWidget);
    m_tree_level->setText(0, "Level");
    m_tree_level->setIcon(0, m_icon_provider.icon(QFileIconProvider::Folder));

    m_tree_platforms = new QTreeWidgetItem(ui->treeWidget);
    m_tree_platforms->setText(0, "Platforms");
    m_tree_platforms->setIcon(0, m_icon_provider.icon(QFileIconProvider::Folder));

    m_tree_enemies = new QTreeWidgetItem(ui->treeWidget);
    m_tree_enemies->setText(0, "Enemies");
    m_tree_enemies->setIcon(0, m_icon_provider.icon(QFileIconProvider::Folder));

    m_tree_powerups = new QTreeWidgetItem(ui->treeWidget);
    m_tree_powerups->setText(0, "Powerups");
    m_tree_powerups->setIcon(0, m_icon_provider.icon(QFileIconProvider::Folder));
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

void MainWindow::disableEditor()
{
    m_editing_enabled = false;
    ui->graphicsView->setEnabled(false);
    ui->treeWidget->setEnabled(false);
}

void MainWindow::enableEditor()
{
    m_editing_enabled = true;
    ui->graphicsView->setEnabled(true);
    ui->treeWidget->setEnabled(true);
}

bool MainWindow::isEditing()
{
    return m_editing_enabled;
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
    if(isEditing()) {

        // ask if they want to save
        // before opening another level

        m_scene->clear();
    }

    m_level_name = name;
    m_level_size = size;
    m_level_background = background;

    m_scene->setSceneRect(-BORDER, -BORDER,
                          size.width() + BORDER * 2,
                          size.height() + BORDER * 2);

    m_scene->addRect(QRectF(QPointF(0,0), size));

    enableEditor();
}

