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
