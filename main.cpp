#include <QApplication>
#include "Scene.h"
#include "Screen.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow m;
    m.show();

    return a.exec();
}
