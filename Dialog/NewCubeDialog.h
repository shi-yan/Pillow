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

    float getX() const;
    float getY() const;
    float getZ() const;
    float getLength() const;
    float getWidth() const;
    float getHeight() const;
    unsigned int getSegmentX() const;
    unsigned int getSegmentY() const;
    unsigned int getSegmentZ() const;

private:
    Ui::NewCubeDialog *ui;
};

#endif // NEWCUBEDIALOG_H
