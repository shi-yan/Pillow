#include "NewCubeDialog.h"
#include "ui_NewCubeDialog.h"

NewCubeDialog::NewCubeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCubeDialog)
{
    ui->setupUi(this);
}

NewCubeDialog::~NewCubeDialog()
{
    delete ui;
}

float NewCubeDialog::getX()
{
    return ui->xDoubleSpinBox->value();
}

float NewCubeDialog::getY()
{
    return ui->yDoubleSpinBox->value();
}

float NewCubeDialog::getZ()
{
    return ui->zDoubleSpinBox->value();
}

float NewCubeDialog::getLength()
{
    return ui->lengthDoubleSpinBox->value();
}

float NewCubeDialog::getWidth()
{
    return ui->widthDoubleSpinBox->value();
}

float NewCubeDialog::getHeight()
{
    return ui->heightDoubleSpinBox->value();
}

unsigned int NewCubeDialog::getSegmentX()
{
    return ui->segmentXSpinBox->value();
}

unsigned int NewCubeDialog::getSegmentY()
{
    return ui->segmentYSpinBox->value();
}

unsigned int NewCubeDialog::getSegmentZ()
{
    return ui->segmentZSpinBox->value();
}
