#include "newlevelform.h"
#include "ui_newlevelform.h"

NewLevelForm::NewLevelForm(const std::vector<QString> &backgrounds,
                           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewLevelForm)
{
    ui->setupUi(this);

    for(const QString &val : backgrounds) {
        ui->backgrounds->addItem(val);
    }

}

NewLevelForm::~NewLevelForm()
{
    delete ui;
}
