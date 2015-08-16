#ifndef NEWCUBEDIALOG_H
#define NEWCUBEDIALOG_H

#include <QDialog>

namespace Ui {
class NewCubeDialog;
}

class NewCubeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewCubeDialog(QWidget *parent = 0);
    ~NewCubeDialog();

private:
    Ui::NewCubeDialog *ui;
};

#endif // NEWCUBEDIALOG_H
