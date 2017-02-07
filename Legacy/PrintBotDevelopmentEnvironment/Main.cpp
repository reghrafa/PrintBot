#include "mainwindow.h"
#include <QApplication>

#include "main.h"

int main(int argc, char *argv[])
{
//    qDebug() << " main-Funktion";
    QApplication application(argc, argv);
    MainWindow window;
    qDebug() << "_______________________________________";
    window.showMaximized();
//    window.show();
    return application.exec();
}
