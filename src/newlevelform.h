#ifndef NEWLEVELFORM_H
#define NEWLEVELFORM_H

#include <QWidget>
#include <vector>

namespace Ui {
class NewLevelForm;
}

class NewLevelForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewLevelForm(const std::vector<QString> &backgrounds,
                          QWidget *parent = nullptr);
    ~NewLevelForm();

private:
    Ui::NewLevelForm *ui;
};

#endif // NEWLEVELFORM_H
