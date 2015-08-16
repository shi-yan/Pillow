#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GLCanvas.h"
#include "Dialog/NewCubeDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionCreate_Cube, SIGNAL(triggered(bool)), this, SLOT(onCreateNewCube(bool)));
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
        theScene->newCube(newCubeDialog->getX(), newCubeDialog->getY(), newCubeDialog->getZ(),
                          newCubeDialog->getLength(), newCubeDialog->getWidth(), newCubeDialog->getHeight(),
                          newCubeDialog->getSegmentX(), newCubeDialog->getSegmentY(), newCubeDialog->getSegmentZ());
    }

}
