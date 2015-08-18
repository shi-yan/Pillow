#ifndef NEWCYLINDERDIALOG_H
#define NEWCYLINDERDIALOG_H

#include <QDialog>

namespace Ui {
class NewCylinderDialog;
}

class NewCylinderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewCylinderDialog(QWidget *parent = 0);
    ~NewCylinderDialog();

    float getX() const;
    float getY() const;
    float getZ() const;
    float getRadius() const;
    float getHeight() const;
    int getAxis() const;
    unsigned int getSegmentA() const;
    unsigned int getSegmentR() const;
    unsigned int getSegmentH() const;


private:
    Ui::NewCylinderDialog *ui;
};

#endif // NEWCYLINDERDIALOG_H
