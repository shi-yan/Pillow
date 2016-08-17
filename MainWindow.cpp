#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GLCanvas.h"
#include "Dialog/NewCubeDialog.h"
#include "Dialog/NewCylinderDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionCreate_Cube, SIGNAL(triggered(bool)), this, SLOT(onCreateNewCube(bool)));
    connect(ui->actionCreate_Cylinder, SIGNAL(triggered(bool)), this, SLOT(onCreateNewCylinder(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCreateNewCube(bool)
{
    NewCubeDialog *newCubeDialog=new NewCubeDialog(this);
    if (newCubeDialog->exec() == QDialog::Accepted)
    {
        Scene::scene->newCube(newCubeDialog->getX(), newCubeDialog->getY(), newCubeDialog->getZ(),
                          newCubeDialog->getLength(), newCubeDialog->getWidth(), newCubeDialog->getHeight(),
                          newCubeDialog->getSegmentX(), newCubeDialog->getSegmentY(), newCubeDialog->getSegmentZ());
    }
    newCubeDialog->deleteLater();
}

void MainWindow::onCreateNewCylinder(bool)
{
    NewCylinderDialog *newCylinderDialog = new NewCylinderDialog(this);
    if (newCylinderDialog->exec() == QDialog::Accepted)
    {
        AxisMode axis = AxisMode::axisZ;
        switch(newCylinderDialog->getAxis())
        {
        case 0:
            axis = AxisMode::axisX;
            break;
        case 1:
            axis = AxisMode::axisY;
            break;
        case 2:
        default:
            axis = AxisMode::axisZ;
        }

        Scene::scene->newCylinder(newCylinderDialog->getX(), newCylinderDialog->getY(), newCylinderDialog->getZ(),
                              newCylinderDialog->getRadius(), newCylinderDialog->getHeight(), axis._value,
                              newCylinderDialog->getSegmentA(), newCylinderDialog->getSegmentR(), newCylinderDialog->getSegmentH());
    }
    newCylinderDialog->deleteLater();
}
