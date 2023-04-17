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

public slots:

    void createPressed();
    void cancelPressed();

signals:

    void finished(const QString &name,
                  const QSizeF &size,
                  const QString &background);

};

#endif // NEWLEVELFORM_H
