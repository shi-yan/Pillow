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

float NewCubeDialog::getX() const
{
    return ui->xDoubleSpinBox->value();
}

float NewCubeDialog::getY() const
{
    return ui->yDoubleSpinBox->value();
}

float NewCubeDialog::getZ() const
{
    return ui->zDoubleSpinBox->value();
}

float NewCubeDialog::getLength() const
{
    return ui->lengthDoubleSpinBox->value();
}

float NewCubeDialog::getWidth() const
{
    return ui->widthDoubleSpinBox->value();
}

float NewCubeDialog::getHeight() const
{
    return ui->heightDoubleSpinBox->value();
}

unsigned int NewCubeDialog::getSegmentX() const
{
    return ui->segmentXSpinBox->value();
}

unsigned int NewCubeDialog::getSegmentY() const
{
    return ui->segmentYSpinBox->value();
}

unsigned int NewCubeDialog::getSegmentZ() const
{
    return ui->segmentZSpinBox->value();
}
