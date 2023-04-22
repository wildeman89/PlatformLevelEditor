#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newlevelform.h"
#include "editorobject.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>
#include <QDirIterator>

#define BORDER 100

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      m_scene(nullptr),
      m_editing_enabled(true)
{
    ui->setupUi(this);

    m_scene = new EditorScene(this);
    ui->graphicsView->setScene(m_scene);

    initTree();
    disableEditor();

    connect(ui->actionNew_Level, &QAction::triggered, this, &MainWindow::newLevelSelected);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveSelected);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAsSelected);
    connect(ui->actionLoad_Level, &QAction::triggered, this, &MainWindow::loadSelected);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::closeSelected);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quitSelected);
    connect(ui->graphicsView, &EditorView::itemDropped, this, &MainWindow::addItem);
    connect(m_scene, &EditorScene::saveRequested, this, &MainWindow::saveSelected);
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
    for(auto it : m_scene->items()) {

        EditorObject *object = qgraphicsitem_cast<EditorObject *>(it);
        if(!object) {
            continue;
        }

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
    QFile file(path);
    if(!file.open(QFile::ReadOnly)) {
        qDebug() << "error opening file";
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root_obj = doc.object();

    if(!root_obj.contains("name")) {
        qDebug() << "readLevelFile - missing level name";
        return false;
    }
    if(!root_obj["name"].isString()) {
        qDebug() << "readLevelFile - name must be a string";
        return false;
    }

    if(!root_obj.contains("background")) {
        qDebug() << "readLevelFile - missing level background";
        return false;
    }
    if(!root_obj["background"].isString()) {
        qDebug() << "readLevelFile - background must be a string";
        return false;
    }

    if(!root_obj.contains("width")) {
        qDebug() << "readLevelFile - missing level width";
        return false;
    }
    if(!root_obj["width"].isDouble()) {
        qDebug() << "readLevelFile - width must be a number";
        return false;
    }

    if(!root_obj.contains("height")) {
        qDebug() << "readLevelFile - missing level height";
        return false;
    }
    if(!root_obj["height"].isDouble()) {
        qDebug() << "readLevelFile - height must be a number";
        return false;
    }

    if(!root_obj.contains("objects")) {
        qDebug() << "readLevelFile - missing level objects";
        return false;
    }
    if(!root_obj["objects"].isArray()) {
        qDebug() << "readLevelFile - height must be an array";
        return false;
    }

    QJsonArray array = root_obj["objects"].toArray();

    for(QJsonValueRef val : array) {

        if(!val.isObject()) {
            qDebug() << "readLevelFile - objects array must contain objects";
            clearLevel();
            return false;
        }

        QJsonObject obj = val.toObject();

        if(!obj.contains("label")) {
            qDebug() << "readLevelFile - missing object label";
            clearLevel();
            return false;
        }
        if(!obj["label"].isString()) {
            qDebug() << "readLevelFile - object label must be a string";
            clearLevel();
            return false;
        }

        if(!obj.contains("x")) {
            qDebug() << "readLevelFile - missing level width";
            clearLevel();
            return false;
        }
        if(!obj["x"].isDouble()) {
            qDebug() << "readLevelFile - x must be a number";
            clearLevel();
            return false;
        }

        if(!obj.contains("y")) {
            qDebug() << "readLevelFile - missing level height";
            clearLevel();
            return false;
        }
        if(!obj["y"].isDouble()) {
            qDebug() << "readLevelFile - y must be a number";
            clearLevel();
            return false;
        }

        QString label = obj["label"].toString();
        QPointF pos(obj["x"].toDouble(), obj["y"].toDouble());

        addItem(label, pos);
    }

    createLevel(root_obj["name"].toString(),
                QSizeF(root_obj["width"].toDouble(), root_obj["height"].toDouble()),
                root_obj["background"].toString());

    return true;

}

void MainWindow::clearLevel()
{
    m_scene->clear();
    m_file_save_path.clear();
    disableEditor();
}

void MainWindow::loadConfigBackground()
{
}

void MainWindow::initTreeFromPath(const QString &path, QTreeWidgetItem *parent)
{
    QDirIterator dirs(path, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (dirs.hasNext()) {
        QString dir_name = dirs.next();

        QTreeWidgetItem *cat = new QTreeWidgetItem(parent);
        cat->setText(0, dir_name);
        cat->setIcon(0, m_icon_provider.icon(QFileIconProvider::Folder));

        QDir dir(dir_name);
        dir.setSorting(QDir::SortFlag::Name);
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
        QDirIterator files(dir, QDirIterator::Subdirectories);

        while (files.hasNext()) {

            QString file_path = files.next();
            auto fields = file_path.split(".");
            QString fname = fields[0];

            QTreeWidgetItem *item = new QTreeWidgetItem(cat);
            item->setText(0, fname);
            item->setIcon(0, QIcon(file_path));

            QPixmap *px = new QPixmap(file_path);
            m_pixmaps[fname] = px;
        }

        cat->sortChildren(0, Qt::SortOrder::AscendingOrder);
        initTreeFromPath(dir_name, cat);
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

void MainWindow::askIfSave()
{
    if(isEditing()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Save", "Would you like to save " + m_level_name + "?",
                                    QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            saveSelected();
        }
    }
}

void MainWindow::initTree()
{
    if(!QDir("res").exists()) {
        QDir().mkdir("res");
    }
    if(!QDir("res/background").exists()) {
        QDir().mkdir("res/background");
    }

    QTreeWidgetItem *cat = new QTreeWidgetItem(ui->treeWidget);
    cat->setText(0, "res");
    cat->setIcon(0, m_icon_provider.icon(QFileIconProvider::Folder));
    initTreeFromPath("res", cat);
}

void MainWindow::quitSelected()
{
    QApplication::quit();
}

void MainWindow::newLevelSelected()
{
    if(isEditing()) {
        askIfSave();
    }
    clearLevel();

    NewLevelForm *form = new NewLevelForm(m_backgrounds);
    connect(form, &NewLevelForm::finished, this, &MainWindow::createLevel);
    form->show();
}

void MainWindow::saveSelected()
{
    if(!isEditing()) {
        return;
    }

    if(m_file_save_path.isEmpty()) {
        saveAsSelected();
    } else {
        writeLevelFile(m_file_save_path);
    }
}

void MainWindow::saveAsSelected()
{
    if(!isEditing()) {
        return;
    }

    m_file_save_path = QFileDialog::getSaveFileName(this, tr("Save File"),".",
                               tr("Platform Level Editor Files (*.ple)"));
    writeLevelFile(m_file_save_path);
}

void MainWindow::loadSelected()
{
    if(isEditing()) {
        askIfSave();
    }
    clearLevel();

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), ".", tr("Platform Level Editor Files (*.ple)"));
    if(readLevelFile(fileName)) {
        enableEditor();
        m_file_save_path = fileName;
    }
}

void MainWindow::closeSelected()
{
    if(!isEditing()) {
        return;
    }

    askIfSave();
    clearLevel();
}

void MainWindow::createLevel(const QString &name,
                                      const QSizeF &size,
                                      const QString &background)
{
    m_level_name = name;
    m_level_size = size;
    m_level_background = background;

    m_scene->setSceneRect(-BORDER, -BORDER,
                          size.width() + BORDER * 2,
                          size.height() + BORDER * 2);

    m_scene->addRect(QRectF(QPointF(0,0), size));

    enableEditor();
}

void MainWindow::addItem(const QString &name, const QPointF &pos)
{
    if (auto search = m_pixmaps.find(name); search == m_pixmaps.end()) {
        return;
    }

    EditorObject *obj = new EditorObject(name, m_pixmaps[name]);
    qreal xV = round(pos.x()/GRID_SIZE)*GRID_SIZE;
    qreal yV = round(pos.y()/GRID_SIZE)*GRID_SIZE;
    obj->setPos(QPointF(xV, yV));
    m_scene->addItem(obj);
    m_scene->clearSelection();
}
