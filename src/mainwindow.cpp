#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newlevelform.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonArray>
#include <QFileDialog>

#define BORDER 256

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      m_scene(nullptr),
      m_editing_enabled(true)
{
    ui->setupUi(this);

    m_scene = new EditorScene(this);
    ui->graphicsView->setScene(m_scene);

    initTreeCategories();
    disableEditor();

    // load config files
    loadConfigBackground();
    loadConfigObjects();

    connect(ui->actionNew_Level, &QAction::triggered, this, &MainWindow::newLevelSelected);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveSelected);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAsSelected);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quitSelected);
    connect(ui->graphicsView, &EditorView::itemDropped, this, &MainWindow::itemDropped);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeLevelFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::WriteOnly)) {
        qDebug() << "error opening file";
        return;
    }

    QJsonArray obj_arr;
    for(auto object : m_objects) {

        QJsonObject json_object;
        json_object["label"] = object->label();
        json_object["x"] = object->pos().x();
        json_object["y"] = object->pos().y();

        obj_arr.append(json_object);
    }

    QJsonObject root_obj;
    root_obj["name"] = m_level_name;
    root_obj["width"] = m_level_size.width();
    root_obj["height"] = m_level_size.height();
    root_obj["background"] = m_level_background;
    root_obj["objects"] = obj_arr;

    QByteArray data = QJsonDocument(root_obj).toJson();

    file.write(data);
    file.close();
}

bool MainWindow::readLevelFile(const QString &path)
{

}

void MainWindow::initTreeCategories()
{
    m_tree_begin_end = new QTreeWidgetItem(ui->treeWidget);
    m_tree_begin_end->setText(0, "Begin/End");
    m_tree_begin_end->setIcon(0, m_icon_provider.icon(QFileIconProvider::Folder));

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

void MainWindow::loadConfigObjects()
{
    QFile file(":/res/config/objects.json");
    if(!file.open(QFile::ReadOnly)) {
        throw std::invalid_argument("missing resource: /res/config/objects.json");
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root_obj = doc.object();

    if(!root_obj.contains("objects")) {
        throw std::invalid_argument("json error: /res/config/objects.json - missing field: objects");
    }

    if(!root_obj["objects"].isArray()) {
        throw std::invalid_argument("json error: /res/config/objects.json - field: objects must be array");
    }

    QJsonArray array = root_obj["objects"].toArray();

    for(const QJsonValue &val : array) {

        if(!val.isObject()) {
            throw std::invalid_argument("json error: /res/config/objects.json - field: objects must contain type: objects");
        }
        QJsonObject obj = val.toObject();

        if(!obj.contains("name")) {
            throw std::invalid_argument("json error: /res/config/objects.json - type: object missing field: name");
        }
        if(!obj.contains("path")) {
            throw std::invalid_argument("json error: /res/config/objects.json - type: object missing field: path");
        }
        if(!obj.contains("category")) {
            throw std::invalid_argument("json error: /res/config/objects.json - type: object missing field: category");
        }

        QString name = obj["name"].toString();
        QString path = obj["path"].toString();
        QString category = obj["category"].toString();

        QTreeWidgetItem *parent;

        if(category == "platform") {
            parent = m_tree_platforms;
        } else if(category == "enemy") {
            parent = m_tree_enemies;
        } else if(category == "powerup") {
            parent = m_tree_powerups;
        } else if(category == "begin-end") {
            parent = m_tree_begin_end;
        } else {
            throw std::invalid_argument("json error: /res/config/objects.json - bad category: " + category.toStdString());
        }

        QTreeWidgetItem *item = new QTreeWidgetItem(parent);
        item->setText(0, name);
        item->setIcon(0, QIcon(path));

        m_pixmaps[name] = new QPixmap(path);
        m_categories[name] = category.toStdString();
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

void MainWindow::saveSelected()
{
    if(m_file_save_path.isEmpty()) {
        saveAsSelected();
    } else {
        writeLevelFile(m_file_save_path);
    }
}

void MainWindow::saveAsSelected()
{
    m_file_save_path = QFileDialog::getSaveFileName(this, tr("Save File"),".",
                               tr("Platform Level Editor Files (*.ple)"));
    writeLevelFile(m_file_save_path);
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

void MainWindow::itemDropped(const QString &name, const QPointF &pos)
{
    EditorObject *obj = new EditorObject(name, m_pixmaps[name]);
    qreal xV = round(pos.x()/GRID_SIZE)*GRID_SIZE;
    qreal yV = round(pos.y()/GRID_SIZE)*GRID_SIZE;
    obj->setPos(QPointF(xV, yV));
    m_scene->addItem(obj);
    m_objects.push_back(obj);
    connect(obj, &EditorObject::deleteObject, this, &MainWindow::deleteObject);
    m_scene->clearSelection();
}

void MainWindow::deleteObject(QGraphicsObject *obj)
{
    m_scene->removeItem(obj);
    m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), obj), m_objects.end());
}

