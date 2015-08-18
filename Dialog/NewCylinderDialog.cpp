#include "NewCylinderDialog.h"
#include "ui_NewCylinderDialog.h"

NewCylinderDialog::NewCylinderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCylinderDialog)
{
    ui->setupUi(this);
}

NewCylinderDialog::~NewCylinderDialog()
{
    delete ui;
}

float NewCylinderDialog::getX() const
{
    return ui->xDoubleSpinBox->value();
}

float NewCylinderDialog::getY() const
{
    return ui->yDoubleSpinBox->value();
}

float NewCylinderDialog::getZ() const
{
    return ui->zDoubleSpinBox->value();
}

float NewCylinderDialog::getRadius() const
{
    return ui->radiusDoubleSpinBox->value();
}

float NewCylinderDialog::getHeight() const
{
    return ui->heightDoubleSpinBox->value();
}

int NewCylinderDialog::getAxis() const
{
    return ui->axisComboBox->currentIndex();
}

unsigned int NewCylinderDialog::getSegmentA() const
{
    return ui->segmentASpinBox->value();
}

unsigned int NewCylinderDialog::getSegmentR() const
{
    return ui->segmentRSpinBox->value();
}

unsigned int NewCylinderDialog::getSegmentH() const
{
    return ui->segmentHSpinBox->value();
}
