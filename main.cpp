#include <QApplication>
#include "MainWindow.h"
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setMajorVersion( 4 );
    format.setMinorVersion( 1 );
    format.setProfile( QSurfaceFormat::CoreProfile );
    QSurfaceFormat::setDefaultFormat(format);


    QApplication a(argc, argv);

    MainWindow m;
    m.show();

    return a.exec();
}
