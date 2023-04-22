#include "newlevelform.h"
#include "ui_newlevelform.h"

#include <QMessageBox>

NewLevelForm::NewLevelForm(const std::vector<QString> &backgrounds,
                           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewLevelForm)
{
    ui->setupUi(this);

    QDoubleValidator *dval = new QDoubleValidator(this);
    dval->setBottom(0.00);
    dval->setDecimals(2);
    dval->setNotation(QDoubleValidator::StandardNotation);

    ui->levelwidth->setValidator(dval);
    ui->levelheight->setValidator(dval);

    int index = 0;
    for(const QString &val : backgrounds) {
        ui->backgrounds->addItem(val);
        ui->backgrounds->setItemIcon(index++, QIcon(val));
    }

    setHeight(ui->backgrounds->currentText());

    connect(ui->createbutton, &QPushButton::clicked, this, &NewLevelForm::createPressed);
    connect(ui->cancelbutton, &QPushButton::clicked, this, &NewLevelForm::cancelPressed);
    connect(ui->backgrounds, &QComboBox::currentTextChanged, this, &NewLevelForm::setHeight);
}

NewLevelForm::~NewLevelForm()
{
    delete ui;
}

void NewLevelForm::createPressed()
{
    if(ui->levelname->text().isEmpty()) {
        QMessageBox::warning(this, "Create Failed", "Level must have a name.");
        return;
    }

    if(ui->levelwidth->text().isEmpty()) {
        QMessageBox::warning(this, "Create Failed", "Level must have a width.");
        return;
    }

    if(ui->levelheight->text().isEmpty()) {
        QMessageBox::warning(this, "Create Failed", "Level must have a height.");
        return;
    }

    bool ok;
    qreal width = ui->levelwidth->text().toDouble(&ok);
    if(!ok) {
        QMessageBox::warning(this, "Create Failed", "Invalid width.");
        return;
    }

    qreal height = ui->levelheight->text().toDouble(&ok);
    if(!ok) {
        QMessageBox::warning(this, "Create Failed", "Invalid height.");
        return;
    }

    emit finished(ui->levelname->text(),
                  QSizeF(width,height),
                  ui->backgrounds->currentText());

    deleteLater();
}

void NewLevelForm::cancelPressed()
{
    deleteLater();
}

void NewLevelForm::setHeight(const QString &str)
{
    if(str.isEmpty()) {
        return;
    }
    QPixmap px(str);
    ui->levelheight->setText(QString::number(px.height()));
}
